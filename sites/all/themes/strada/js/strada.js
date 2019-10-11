/**
 *
 */
(function ($, Drupal, window, document, undefined) {

    Drupal.behaviors.strada = {
        attach: function (context, settings) {

            /* -------------------- AJAX throbber ---------------------------------------------------- */
            // замена страндартной и bootstrap функций AJAX лоадера
            // https://gist.github.com/hedleysmith/260a427f1e48f456bb56
            // http://xandeadx.ru/blog/drupal/753
            // https://www.drupaler.by/kak-sozdat-polzovatelskiy-ajax-progress-type
            // gif - https://loading.io

            $("body").once(function() {
                $("body").data("ajax-count", 0);

                if (Drupal.ajax !== undefined) {
                    var beforeSend = Drupal.ajax.prototype.beforeSend;
                    var $overlay = $('<div class="overlay-disabled"></div>');
                    var $throbber = $('<div class="ajax-progress-custom ajax-progress-throbber-custom"><div class="throbber">&nbsp;</div></div>');

                    Drupal.ajax.prototype.beforeSend = function (response, options) {
                        //beforeSend.call(this, response, options);

                        // будем менять смещение относительно начала документа,
                        // если подложка накладывается на весь экран
                        var host_offset = 0;

                        // trobber может быть показан локально
                        // для класса view-item
                        // и для вручную проставленного класса ajax-host
                        var $host = $(this.element).closest('.view-item');
                        if ($host.length == 0) { $host = $(this.element).closest('.ajax-host'); }
                        if ($host.length == 0) {
                            $host = $('body');
                            host_offset = pageYOffset;
                        }

                        // it can be few ajax events simultaneously
                        var ajaxcount = $("body").data("ajax-count");
                        if (ajaxcount === 0) {
                            $host.append($overlay);
                            $overlay.css('top', host_offset);

                            $host.append($throbber);
                            $throbber.css('top', host_offset);

                            // корректировка положения подложки и крутяшки при смещении
                            if ($host == $('body')) {
                                window.onscroll = function () {
                                    $overlay.css('top', host_offset);
                                    $throbber.css('top', host_offset);
                                }
                            }
                        }
                        $("body").data("ajax-count", ajaxcount + 1);
                    };

                    var success = Drupal.ajax.prototype.success;
                    Drupal.ajax.prototype.success = function (response, status) {
                        success.call(this, response, status);

                        var ajaxcount = $("body").data("ajax-count");
                        if (ajaxcount === 1) {
                            $throbber.remove();
                            $overlay.remove();
                        }
                        if (ajaxcount > 0) {
                            $("body").data("ajax-count", ajaxcount - 1);
                        }
                    };

                    var error = Drupal.ajax.prototype.error;
                    Drupal.ajax.prototype.error = function (response, uri) {
                        error.call(this, response, uri);

                        var ajaxcount = $("body").data("ajax-count");
                        if (ajaxcount === 1) {
                            $throbber.remove();
                            $overlay.remove();
                        }
                        if (ajaxcount > 0) {
                            $("body").data("ajax-count", ajaxcount - 1);
                        }
                    };
                }
            });

            /** -------------------- Меню Помощь, раскрывающееся подменю ------------------------------ */
            $('i.fa-caret-right').on('click', function () {
                if ($(this).closest('li').hasClass('expanded')) {
                    $(this).closest('li').removeClass('expanded');
                } else {
                    $(this).closest('li').addClass('expanded');
                }
            });

            /** -------------------- Переключение радио кнопок ---------------------------------------- */
            $(".form-radios .control-label").bind('click', function() {
                var radio = $(this).closest('.radio');
                var radios = $(this).closest('.form-radios');

                radio.addClass('active');
                radios.find('.radio').each(function() {
                    var a = $(this);
                    if (this != radio[0]) {
                        $(this).removeClass('active');
                    }
                });

            });

            /** ------------------------------------------ Регион (автоопределение) -------------------------------------------- */
            //$.cookie('user_region', null);

            // если в шапке присутствует выбор региона
            if ($('#user_region').is('a')) {

                // если кука в неправильном формате - сбросить
                var user_region = JSON.parse($.cookie('user_region'));
                if (user_region && user_region.data == undefined) {
                    $.cookie('user_region', null, {path: "/"});
                }

                // определение региона
                if ($.cookie('user_region')) {
                    _set_user_region(JSON.parse($.cookie('user_region')));
                }
                else {
                    detect();
                }

                function detect() {
                    detectAddress()
                        .done(function (response) {
                            if (response.location) {
                                $.cookie('user_region', JSON.stringify(response.location), {path: "/"});
                                _set_user_region(response.location);
                            }
                        })
                        .fail(function (jqXHR, textStatus, errorThrown) {
                            console.log(textStatus);
                            console.log(errorThrown);
                        });
                }

                function detectAddress() {
                    var token = "9948c242cbb5e110b4c488f61fe347c9fd038640";
                    var serviceUrl = "https://suggestions.dadata.ru/suggestions/api/4_1/rs/iplocate/address";
                    var params = {
                        type: "GET",
                        contentType: "application/json",
                        headers: {"Authorization": "Token " + token}
                    };
                    return $.ajax(serviceUrl, params);
                }

                function _set_user_region(location) {
                    $("#user_region").html(location.data.settlement ? location.data.settlement : location.data.city);
                }


                /** ------------------------------------------ Регион (форма выбора) -------------------------------------------- */
                // based on JQuery UI attached by autodialog module
                //
                $('.select-region').once(function () {
                    $('.select-region').bind('click', function () {
                        // инициализация и открытие диалога
                        var city = "";
                        if ($.cookie('user_region')) {
                            var location = JSON.parse($.cookie('user_region'));
                            city = location.data.settlement ? location.data.settlement : location.data.city
                        }

                        var html = '<div class="select-region-form">\n' +
                            '    <div class="sr-intro">\n' +
                            '        <img src="/sites/all/themes/strada/images/map.jpg">\n' +
                            '        <h3>Вам показаны товары с доставкой в <span>' + city + '</span></h3>\n' +
                            '    </div>\n' +
                            '    <div class="sr-input">\n' +
                            '        <input type="text" placeholder="Укажите другой регион" />\n' +
                            '    </div>\n' +
                            '    <button class="btn btn-lg btn-primary select-region-close">Продолжить с новым регионом</button>\n' +
                            '    <a href="#" class="btn btn-link select-region-close">закрыть окно</a>\n' +
                            '</div>';

                        var $dialog = $(html).appendTo('body');

                        $dialog.dialog({
                            autoOpen: false,
                            width: 550,
                            height: 550,
                            modal: true,
                            dialogClass: "no-close",
                            open: function() {
                                // Close dialog when prooer target clicked
                                $('.ui-widget-overlay').bind('click', function (event) {
                                    if ($(event.target).hasClass('ui-widget-overlay')) {
                                        $dialog.dialog('destroy');
                                        $dialog.remove();
                                    }
                                });
                            }
                        });

                        $('.select-region-close').bind('click', function () {
                            $dialog.dialog('destroy');
                            $dialog.remove();
                        });

                        Drupal.attachBehaviors($dialog);

                        $dialog.dialog('open');
                    });
                });
            }

                // настройка подсказок в Диалоге или в поле Чекаута -----------------------------------
            if ($('#user_region').is('a')) {
                var city = $('.select-region-form input');
                var options = {
                    token: "9948c242cbb5e110b4c488f61fe347c9fd038640",
                    type: "ADDRESS",
                    hint: false,
                    constraints: false,
                    count: 10,
                    bounds: "city-settlement",
                    formatResult: Drupal.formatResult,
                    onSuggestionsFetch: Drupal.removeDistricts,
                    onSelect: updateRegion,
                    scrollOnFocus: false // убирает проблему с дрожанием экрана на iphone
                };

                city.suggestions(options);
            }

            Drupal.formatResult = function(value, currentValue, suggestion) {
                var addressValue = makeAddressString(suggestion.data);
                suggestion.value = addressValue;

                return addressValue;
            };

            Drupal.removeDistricts = function (suggestions) {
                return suggestions.filter(function (suggestion) {
                    return suggestion.data.city_district === null;
                });
            };

            function updateRegion(location) {
                $.cookie('user_region', JSON.stringify(location), {path: "/"});
                _set_user_region(location);
            }

            function join(arr /*, separator */) {
                var separator = arguments.length > 1 ? arguments[1] : ", ";
                return arr.filter(function (n) {
                    return n
                }).join(separator);
            }

            // составление строки списка
            function makeAddressString(address) {
                var not = [];
                if ((address.settlement || address.city) && address.region != address.city) not.unshift([join([address.region_type, address.region], " ")]);
                if (address.area) not.unshift(join([address.area_type, address.area], " "));

                var city = join([address.city_type, address.city], " ");
                var main = address.settlement ? join([address.settlement_type, address.settlement], " ") : city;

                if (address.settlement) not.unshift(join([address.area_type, address.area], " "));

                return '<div class="main">' + main + '</div>' +
                    '<div class="not">' + join(not) + '</div>';
            }


            /* ------------------------------------------ Анимация добавления в корзину ----------------------------- */
            $('a.btn-add-to-cart').on('click', function(){

                var that = $(this).closest('.product').find('.p-image img');
                var cart = $("#cart");
                var w = that.width();

                that.clone()
                    .css({'width' : w,
                        'position' : 'absolute',
                        'z-index' : '9999',
                        top: that.offset().top,
                        left:that.offset().left})
                    .appendTo("body")
                    .animate({opacity: 0.05,
                        left: cart.offset()['left'],
                        top: cart.offset()['top'],
                        width: 20}, 1000, function() {
                        $(this).remove();
                    });
            });

            /* ------------------------------------------ Header ---------------------------------------------------- */
            $("body").once('header-scroll', function() {
                window.onscroll = function () {
                    var fixedTop = 0;
                    if ($('body').hasClass('admin-menu')) fixedTop = 28;
                    if ($(window).scrollTop() > 27) {
                        if ($('.line-2').css('position') == 'absolute') {
                            $('.line-2').css('top', fixedTop);
                            $('.line-2').css('position', 'fixed');
                            $('.line-2').css('box-shadow', '1px 1px 4px #ddd');
                        }
                    } else {
                        if ($('.line-2').css('position') == 'fixed') {
                            $('.line-2').css('top', 27);
                            $('.line-2').css('position', 'absolute');
                            $('.line-2').css('box-shadow', 'none');
                        }
                    }
                }
            });

            /* ------------------------------------------ Main Menu ------------------------------------------------- */
            $(".main-menu ul.level-2 > li > a").hover(function () {
                var hoveredLI = this.closest('li');
                $(hoveredLI).closest("ul.level-2").find("li").each(function(index, el) {
                    if (el !== hoveredLI) {
                        $(el).removeClass("visible");
                    }
                });
                $(hoveredLI).addClass("visible");

                mainMenuResize(hoveredLI);
            });

            // мобильная кнопка для закрытия меню третьего уровня
            $('.main-menu ul.level-2 > li.expanded > a').bind('click', function(e) {
                var hoveredLI = this.closest('li');
                $(hoveredLI).addClass("visible");
                e.stopPropagation();
                e.preventDefault();
            });

            // мобильная кнопка для закрытия меню третьего уровня
            $('.back-button').bind('click', function(e) {
                var hoveredLI = this.closest('li');
                $(hoveredLI).removeClass("visible");
                e.stopPropagation();
            });

            // затемнение
            $(".main-menu > li").on("show.bs.dropdown", function () {
                var docHeight = $(document).height() + 50;
                var docWidth = $(document).width();
                var winHeight = $(window).height();
                if( docHeight < winHeight ) { docHeight = winHeight; }
                $("#modalBackdrop").css("top", "125px").css("height", docHeight + "px").css("width", docWidth + "px").show();
            });
            $(".main-menu > li").on("shown.bs.dropdown", function () {
                mainMenuResize($(".level-2-item.visible"));
            });
            $(".main-menu > li").on("hidden.bs.dropdown", function () {
                $("#modalBackdrop").hide();
            });


            function mainMenuResize(el) {
                var maxHeight = $(el).find(".level-3-wrapper").outerHeight();
                var minHeight = $(el).closest(".level-2").outerHeight();
                $(el).closest(".dropdown-menu").height(minHeight < maxHeight ? maxHeight : minHeight);
            }

            /* ------------------------------------------ User  Menu ----------------------------------------------- */
            $(".user-menu .dropdown").hover(function() {
                    $(this).addClass("open");
                },
                function() {
                    $(this).removeClass("open");
                });



            /* -------------------- Плавный скролл к якорю --------------------------------------------- */
            var hash = location.hash;

            if (hash) {
                var anchor = hash.split('scroll-to-')[1];
                var timerId = setTimeout(scrollToAnchor, 400, [anchor]);
            }
            function scrollToAnchor(anchor) {
                $('html, body').animate({
                    scrollTop: $('#' + anchor).offset().top - 50
                }, 700);
            }

            /* ----------------- Карусель с товаром (flexslider) ------------------------------------- */
            // проверка на наличие слайдера
            if (settings.flexslider !== undefined) {
                var $window = $(window);

                // начальные установки слайдера
                settings.flexslider.optionsets.product_carousel.maxItems = getGridSize();
                settings.flexslider.optionsets.product_carousel.minItems = getGridSize();

                // отслеживать событие ресайза
                $window.resize(function () {
                    scaleFlexslider();
                });
                var timerId = setTimeout(scaleFlexslider, 400);
            }

            // расчёт количества товаров в слайдере
            // ex - 1 товар, sm - 4 товара, остальные - 5
            function getGridSize() {
                return (window.innerWidth < 768) ? 2 : (window.innerWidth < 992) ? 4 : 5;
            }
            function scaleFlexslider() {
                $('.flexslider').each(function() {
                    $(this).flexslider({
                        minItems: getGridSize(),
                        maxItems: getGridSize(),
                    });
                });
            }

            /* -------------------- Фикс AJAX капчи ---------------------------------------------------- */
            // http://xandeadx.ru/blog/drupal/910
            if ('grecaptcha' in window && context !== document) {
                $('.g-recaptcha:empty', context).each(function () {
                    grecaptcha.render(this, $(this).data());
                });
            }

            /* ------------------------------------- e-address decode ---------------------------------- */
            /* <a href="e(supp/ort[s1]kcc/c[s2]ru)" class="eAddr-encoded eAddr-html"></a> */
            $("a.eAddr-encoded").each(function() {
                var $href = $(this).attr("href");
                var $pattern = /e\((.*)\)/;
                var $match = $pattern.exec($href);
                if ($match) {
                    var $eAddr = $match[1];
                    if ($eAddr) {
                        $eAddr = $eAddr.replace(/\//g, "");
                        $eAddr = $eAddr.replace(/\[s1\]/, "@");
                        $eAddr = $eAddr.replace(/\[s2\]/, ".");
                        $(this).attr("href", "mailto:" + $eAddr).removeClass("eAddr-encoded");
                        if ($(this).hasClass("eAddr-html")) {
                            $(this).html($eAddr).removeClass("eAddr-html");
                        }
                    }
                }
            });

            /* ------------------------------------- Phone popover ---------------------------------------- */
            $('#phone_popover').popover({
                html: true,
                trigger: 'focus',
                placement: 'bottom',
                title: 'Служба поддержки',
                content: function () {
                    var button = checkTime ('08:00:00', '17:00:00') ? '<br /><br /><a href="tel:" class="btn btn-brand btn-block">Позвонить</a>' : '';
                    return 'Время работы:<br />Пн - Пт &nbsp;&nbsp; 08:00 - 17:00<br /><small style="color: #ccc;">(по московскому времени)</small>' + button;
                },

            });
            function checkTime (beg, end) {
                var s = 60,
                    d = ':',
                    b = beg.split (d), b = b [0]* s * s + b [1] * s + +b [2],
                    e = end.split (d), e = e [0]* s * s + e [1] * s + +e [2],
                    t = new Date, t = t.getHours () * s * s + t.getMinutes () * s + t.getSeconds ();
                return (t >= b && t <= e);
            }
        }
    };

})(jQuery, Drupal, this, this.document);

