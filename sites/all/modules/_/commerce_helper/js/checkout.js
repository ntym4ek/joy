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
                    onSelect: select,
                    scrollOnFocus: false, // убирает проблему с дрожанием экрана на iphone
                    onSelectNothing: nothingSelected,
                };
                city.suggestions(options);

                // один раз определить значение для данной страницы
                $('body').once(function () {
                    if ($.cookie('user_region')) {
                        // проверить наличие куки с регионом
                        var location = JSON.parse($.cookie('user_region'));
                        if (!$('.commerce-checkout-form-checkout .has-error').is('div')) {
                            city.suggestions().setSuggestion(location);
                            updateFields(location);
                        }
                    }
                });



                // обработчик на выбор населенного пункта
                function select(suggestion) {
                    if (suggestion.data.postal_code) {
                        updatePage(suggestion);
                    } else {
                        Drupal.getPostalCode(suggestion).done(function(result) {
                            updatePage(result);
                        });
                    }
                }

                // обработчик, когда ничего не выбрано из списка
                // выбрать первый вариант
                function nothingSelected(aux) {
                    // если ничего не выбрано, но есть подсказки, вставить первую
                    // актуально при Автозаполнеии в браузере
                    var city = $(this).suggestions();
                    if (city.suggestions[0] !== undefined) {
                        select(city.suggestions[0]);
                    }
                }

                // выбор региона для страницы Чекаута
                function updatePage(suggestion) {
                    $.cookie('user_region', JSON.stringify(suggestion), {path: "/"});
                    // если сменили город, то очистить поле с данными пункта выдачи
                    $.cookie('user_boxberry', null, {path: "/"});
                    city.suggestions().setSuggestion(suggestion);
                    updateFields(suggestion);
                }

                function clearFields() {
                    $('.field-name-field-city').find('input[name*="field_city"]').val('');
                    $('.field-name-field-city').addClass('has-error');
                }
                function updateFields(suggestion) {
                    if (suggestion.data.postal_code) {
                        $('.field-name-field-city').removeClass('has-error');
                    } else {
                        clearFields();
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