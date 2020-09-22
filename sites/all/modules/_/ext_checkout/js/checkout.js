(function ($) {
    Drupal.behaviors.Checkout = {
        attach: function (context, settings) {

            // убираем стандартный обработчик кнопки recalculate
            // так как он влияет на ajax-loader
            if (Drupal.behaviors.commerceShipping) {
                delete Drupal.behaviors.commerceShipping.attach;
                Drupal.behaviors.commerceShipping.detach;
            }

            /** -------------------- Ввод региона с автодополнением -------------------------------------- */
            if ($('.field-name-field-city input').is('input')) {
                var city = $('.field-name-field-city input');

                var options = {
                    token: "9948c242cbb5e110b4c488f61fe347c9fd038640",
                    type: "ADDRESS",
                    hint: false,
                    constraints: false,
                    count: 25,
                    bounds: "city-settlement",
                    formatResult: Drupal.formatResult,
                    onSuggestionsFetch: Drupal.removeDistricts,
                    onSelect: onSelect,
                    scrollOnFocus: false, // убирает проблему с дрожанием экрана на iphone
                    onSelectNothing: nothingSelected,
                };
                city.suggestions(options);

                // один раз определить значение для данной страницы
                $('body').once(function () {
                    if ($.cookie('user_region')) {
                        // проверить наличие куки с регионом
                        var user_region = JSON.parse($.cookie('user_region'));
                        if (!$('.commerce-checkout-form-checkout .has-error').is('div')) {
                            updatePage(user_region);
                        }
                    } else {
                      Drupal.getCityByIp().done(onSelect);
                    }
                });



                // обработчик на выбор населенного пункта
                function onSelect(suggestion) {
                    // массив запросов, выполнение которых нужно дождаться
                    var promises_arr = [];
                    if (!suggestion.data.postal_code) {
                        promises_arr.push(Drupal.getPostalCode(suggestion));
                    }
                    promises_arr.push(Drupal.loadDelivery(suggestion));

                    // действие по окончании выполнения запросов
                    $.when.apply($, promises_arr)
                        .done(function(arg1, arg2) {
                            arg1 = arg1 ? arg1.data : {};
                            arg2 = arg2 ? arg2.data : {};
                            $.extend(suggestion.data, arg1, arg2);
                            updatePage(suggestion);
                        });

                }

                // обработчик, когда ничего не выбрано из списка
                // выбрать первый вариант
                function nothingSelected(aux) {
                    // если ничего не выбрано, но есть подсказки, вставить первую
                    // актуально при Автозаполнеии в браузере
                    var city = $(this).suggestions();
                    if (city.suggestions[0] !== undefined) {
                        onSelect(city.suggestions[0]);
                    }
                }

                // выбор региона для страницы Чекаута
                function updatePage(suggestion) {
                    $.cookie('user_region', JSON.stringify(suggestion), {path: "/"});
                    city.suggestions().setSuggestion(suggestion);
                    updateFields(suggestion);
                }

                function updateFields(suggestion) {
                    var field_data = $(".field-name-field-data textarea").val();
                    var data = {};
                    if (field_data) {
                        data = JSON.parse(field_data);
                    }

                    // если сменили город, очистить поле с данными пункта выдачи при несовпадении
                    if (!data.region || data.region.data.kladr_id != suggestion.data.kladr_id) {
                        delete data.boxberry;
                        delete data.cdek;
                        $(".field-name-field-data textarea").val(JSON.stringify(data));
                    }

                    // без индекса адрес не принимаем
                    if (suggestion.data.postal_code) {
                        $('.field-name-field-city').removeClass('has-error');

                        data.region = suggestion;
                        $(".field-name-field-data textarea").val(JSON.stringify(data));
                        $(".field-name-field-region input").val(suggestion.data.region_with_type);
                        $(".field-name-field-zipcode-calc input").val(suggestion.data.postal_code);
                    } else {
                        $('.field-name-field-city').addClass('has-error').find('input[name*="field_city"]').val('');

                        delete data.region;
                        $(".field-name-field-data textarea").val(JSON.stringify(data));
                        $(".field-name-field-region input").val("");
                        $(".field-name-field-region input").val("");
                        $(".field-name-field-zipcode-calc input").val("");
                    }


                    // обновляем форму
                    $(':input[id^="edit-commerce-shipping-recalc"]').click();
                }
            }


            /** -------------------- Сворачивающийся список товаров ----------------------------------- */
            $('#collapseCartList').once( function () {
                $('#collapseCartList').on('show.bs.collapse', function () {
                    $('.folder .unfold').css('display', 'none');
                    $('.folder .fold').css('display', 'inline');
                });
                $('#collapseCartList').on('hide.bs.collapse', function () {
                    $('.folder .fold').css('display', 'none');
                    $('.folder .unfold').css('display', 'inline');
                });
            });

          /** -------------------- Ввод с маской ---------------------------------------------------- */
          $(".masked-phone").mask("9 (999) 999-9999");

            /** -------------------- Населенный пункт ------------------------------------------------- */
            // если расчёт доставки не выполнен, подсветить поле выбора населенного пункта
            if ($('.field-name-field-city').find('input[name*="field_city"]').val() === "") {
              $('.field-name-field-city').addClass('has-error');
            }

            /** -------------------- Обработка нажатия Оформить заказ --------------------------------- */
            $('.page-cart .checkout-continue').once(function() {
                $(this).on('click', function(){
                    $(this).text($(this).data('processing-label'));
                    // активируем достижение цели в Метрике
                    ym(47689555, 'reachGoal', 'knopkaOformit');
                });
            });

            $('.page-checkout .checkout-continue').once(function() {
                $(this).on('click', function(){
                    $(this).text('Отправляем заказ...');
                    // активируем достижение цели в Метрике
                    ym(47689555, 'reachGoal', 'knopkaPodtverdit');
                });
            });

          /** -------------------- Сохранение пользовательских данных в кукис ------------------------- */
          var fields = ['username', 'phone', 'zipcode', 'addr', 'office', 'callme'];

          // при открытии страницы, проверить наличие ранее сохранённых данных
          var address = JSON.parse($.cookie('user_address'));
          if (!address) address = {};

          $('.ch-form-container').once(function() {
            $.each(fields, function(index, value) {
              var $field = $('[name="commerce_shipping[service_details][' + value + ']"]');
              if ($field.length) {
                if (!$field.val() && address[value]) {
                  $field.val(address[value]);
                }

                // повесить обработчик, для обновления данных в куке
                $field.blur(function () {
                  var address = JSON.parse($.cookie('user_address'));
                  if (!address) address = {};
                  address[value] = $field.val();
                  $.cookie('user_address', JSON.stringify(address), {path: "/"});
                });
              }
            });
          });

          // заполнить поле ввода телефона в корзине из куки
          if (address['phone']) {
            $('.ch-info-container [name=phone]').val(address['phone']);
          }
        }
    };
})(jQuery);
