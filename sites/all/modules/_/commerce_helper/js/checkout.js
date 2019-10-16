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
                    onSelect: updatePage,
                    scrollOnFocus: false, // убирает проблему с дрожанием экрана на iphone
                    onSelectNothing: nothingSelected,
                };
                city.suggestions(options);

                // один раз определить значение для данной страницы
                $('body').once(function () {
                    if ($.cookie('user_region')) {
                        // проверить наличие куки с регионом для страницы чекаута
                        var location = JSON.parse($.cookie('user_region'));
                        if (!$('.commerce-checkout-form-checkout .has-error').is('div')) {
                            if (location.data.postal_code !== undefined && location.data.postal_code !== null) {
                                city.suggestions().setSuggestion(location);
                                updateFields(location);
                            } else {
                                clearFields();
                                $.cookie('user_region', null);
                                // попробовать установить глобальный для сайта регион
                                if (location = JSON.parse($.cookie('user_region'))) {
                                    if (location.data.postal_code !== undefined && location.data.postal_code !== null) {
                                        city.suggestions().setSuggestion(location);
                                        updateFields(location);
                                    }
                                }
                            }
                        }
                    }
                });

                function nothingSelected(aux) {
                    // если ничего не выбрано, но есть подсказки, вставить первую
                    // актуально при Автозаполнеии в браузере
                    var city = $(this).suggestions();
                    if (city.suggestions[0] !== undefined) {
                        updatePage(city.suggestions[0]);
                    }
                }

                // выбор региона для страницы Чекаута
                function updatePage(suggestion) {
                    // ставис короткую куку в день на эту страницу
                    $.cookie('user_region', JSON.stringify(suggestion), {expire: 86400, path: "/checkout"});
                    // если сменили город, то очистить поле с данными
                    $(".field-name-field-data textarea").val('');

                    city.suggestions().setSuggestion(suggestion);
                    updateFields(suggestion);
                }

                function clearFields() {
                    $('.field-name-field-city').find('input[name*="field_city"]').val('');
                    $('.field-name-field-city').addClass('has-error');
                    $('.field-name-field-zipcode-calc').find('input[name*="field_zipcode_calc"]').val('');
                    $('.field-name-field-region').find('input[name*="field_region"]').val('');
                    $.cookie('boxberry_locality', null);
                }
                function updateFields(suggestion) {
                    if (suggestion.data.postal_code) {
                        $('.field-name-field-city').removeClass('has-error');
                        $('.field-name-field-zipcode-calc').find('input[name*="field_zipcode_calc"]').val(suggestion.data.postal_code);
                        $('.field-name-field-region').find('input[name*="field_region"]').val(join([suggestion.data.region, suggestion.data.region_type_full], ' '));
                        // сохраняем для Boxberry (чтобы он не обновляется при пересчёте)
                        $.cookie('boxberry_locality', JSON.stringify(suggestion.unrestricted_value), {expire: 86400, path: "/checkout"});
                    } else {
                        clearFields();
                    }

                    // обновляем форму
                    $(':input[id^="edit-commerce-shipping-recalc"]').click();
                }

                function join(arr /*, separator */) {
                    var separator = arguments.length > 1 ? arguments[1] : ", ";
                    return arr.filter(function (n) {
                        return n
                    }).join(separator);
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
            if ($('.field-name-field-city').find('input[name*="field_city"]').val() === "") $('.field-name-field-city').addClass('has-error');

            /** -------------------- Обработка нажатия Оформить заказ --------------------------------- */
            $('.checkout-continue').once(function() {
                $('.checkout-continue').on('click', function(){
                    $(this).text('Отправляем заказ...');
                    // активируем достижение цели в Метрике
                    ym(47689555, 'reachGoal', 'knopkaPodtverdit');
                });
            });
        }
    };
})(jQuery);