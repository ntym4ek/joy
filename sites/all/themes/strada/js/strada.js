/**
 *
 */
var token = "9948c242cbb5e110b4c488f61fe347c9fd038640";

// глобальная переменная для всплывающих окон
var popupStatus = 0;

(function ($, Drupal, window, document, undefined) {
    Drupal.behaviors.strada = {
        attach: function (context, settings) {

          // device detection
          var isMobile = false;
          if(/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|ipad|iris|kindle|Android|Silk|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(navigator.userAgent)
            || /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(navigator.userAgent.substr(0,4))) {
            isMobile = true;
          }

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
                        if ($host.length == 0) {
                            $host = $(this.element).closest('.ajax-host');
                        }
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
                    if (this != radio[0]) {
                        $(this).removeClass('active');
                    }
                });

            });

            /** ------------------- Регион (автоопределение) -------------------------------------------- */
            // если в шапке присутствует выбор региона
            if ($('#user_region').is('a')) {

                // если кука в неправильном формате - сбросить
                var user_region = JSON.parse($.cookie('user_region'));
                if (user_region && user_region.data === undefined) {
                    $.cookie('user_region', null, {path: "/"});
                }

                // определение региона
                if ($.cookie('user_region')) {
                    var user_region = JSON.parse($.cookie('user_region'));
                    $("#user_region").html(user_region.data.settlement ? user_region.data.settlement : user_region.data.city);
                }
                else {
                    Drupal.getCityByIp().done(onSelect);
                }




                /** ------------------------------------------ Регион (форма выбора) -------------------------------------------- */
                // based on JQuery UI attached by autodialog module
                $('.select-region').once(function () {
                    $('.select-region').bind('click', function () {
                        // инициализация и открытие диалога
                        var city = "";
                        if ($.cookie('user_region')) {
                            var user_region = JSON.parse($.cookie('user_region'));
                            city = user_region.data.settlement ? user_region.data.settlement : user_region.data.city
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
                            // для корзины после смены региона - обновить
                            if (window.location.pathname === "/cart") {
                                window.location.reload();
                            }
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
                    token: token,
                    type: "ADDRESS",
                    hint: false,
                    constraints: false,
                    count: 10,
                    bounds: "city-settlement",
                    formatResult: Drupal.formatResult,
                    onSuggestionsFetch: Drupal.removeDistricts,
                    onSelect: onSelect,
                    scrollOnFocus: false // убирает проблему с дрожанием экрана на iphone
                };

                city.suggestions(options);
            }

            function onSelect(suggestion) {
                // ждём выполнения всех запросов и пишем куку
                $.when(Drupal.loadDelivery(suggestion), Drupal.getPostalCode(suggestion))
                    .done(function(arg1, arg2) {
                        $.extend(suggestion.data, arg1.data, arg2.data);
                        $.cookie('user_region', JSON.stringify(suggestion), {path: "/"});
                        var user_region = JSON.parse($.cookie('user_region'));
                        $("#user_region").html(user_region.data.settlement ? user_region.data.settlement : user_region.data.city);
                    });
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
                    if ($(window).scrollTop() > 40) {
                        if ($('.line-2').css('position') == 'absolute') {
                            $('.line-2').css('top', fixedTop);
                            $('.line-2').css('position', 'fixed');
                            $('.line-2').css('box-shadow', '1px 1px 4px #ddd');
                        }
                    } else {
                        if ($('.line-2').css('position') == 'fixed') {
                            $('.line-2').css('top', 28);
                            $('.line-2').css('position', 'absolute');
                            $('.line-2').css('box-shadow', 'none');
                        }
                    }
                }
            });

            /* ------------------------------------------ Main Menu ------------------------------------------------- */
            // выравнивание выпадающих элементов для правых пунктов меню
            function alignMenu() {
              var coordsParent = $(".main-menu").offset();
              var widthParent = $(".main-menu").width();
              $(".main-menu .level-1-item").each(function () {
                var drop = $(this).find(".dropdown-menu");
                if (drop.length > 0) {
                  var coordsChild = drop.offset();
                  var widthChild = drop.width();
                  var title = $(this).find("a").attr("title");
                  if (coordsChild.left + widthChild > coordsParent.left + widthParent - 10) {
                    drop.css("right", "0").css("left", "initial");
                  }
                }
              });
            }
            setTimeout(alignMenu, 500);

            // только для десктопа
            // клик по родительской категории тоже работает как переход
            if (!isMobile) {
              $(".main-menu .level-1-item.dropdown")
                .mouseenter(function () {
                  $(this).siblings().removeClass("open");
                  $(this).addClass("open");
                })
                .mouseleave(function () {
                  $(this).removeClass("open");
                });

              $(".main-menu .level-1-item").click(function () {
                if ($(this).hasClass('open')) {
                  location.href = $(this).find('> a').attr('href');
                }
              });
            }

            /* ------------------------------------------ Main Menu mobile ------------------------------------------------- */
            var menu = $('.main-menu-mobile');
            menu.find("ul.level-2 > li > a").hover(function () {
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
            menu.find("ul.level-2 > li.expanded > a").bind('click', function(e) {
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
            menu.find("> li").on("show.bs.dropdown", function () {
                var docHeight = $(document).height() + 50;
                var docWidth = $(document).width();
                var winHeight = $(window).height();
                if( docHeight < winHeight ) { docHeight = winHeight; }
                $("#modalBackdrop").css("top", "125px").css("height", docHeight + "px").css("width", docWidth + "px").show();
            });
            menu.find("> li").on("shown.bs.dropdown", function () {
                mainMenuResize($(".level-2-item.visible"));
            });
            menu.find("> li").on("hidden.bs.dropdown", function () {
                $("#modalBackdrop").hide();
            });


            function mainMenuResize(el) {
                var maxHeight = $(el).find(".level-3-wrapper").outerHeight();
                var minHeight = $(el).closest(".level-2").outerHeight();
                $(el).closest(".dropdown-menu").height((minHeight < maxHeight ? maxHeight : minHeight));
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

            /* ------------------------------------- мобильная строка поиска ------------------------------ */
            $('.navbar-search-mobile a').on('click', function(){
                $('.navbar-search .navbar-search-pane').addClass('open');
            });
            $('.navbar-search-pane-close a').on('click', function(){
                $('.navbar-search .navbar-search-pane').removeClass('open');
            });


            /* ------------------------------------- Phone popover ---------------------------------------- */
            // $('#phone_popover').popover({
            //     html: true,
            //     trigger: 'focus',
            //     placement: 'bottom',
            //     title: 'Служба поддержки',
            //     content: function () {
            //         var button = checkTime ('08:00:00', '17:00:00') ?
            //             '<br /><br /><a href="tel:88005502885" class="btn btn-brand btn-block">Позвонить</a>' :
            //             '<br /><br /><button class="btn btn-brand btn-block disabled">Не работает</button>';
            //         return 'Время работы:<br />Пн - Пт &nbsp;&nbsp; 08:00 - 17:00<br /><small style="color: #ccc;">(по московскому времени)</small>' + button;
            //     },
            //
            // });
            // function checkTime (beg, end) {
            //     var s = 60,
            //         d = ':',
            //         b = beg.split (d), b = b [0]* s * s + b [1] * s + +b [2],
            //         e = end.split (d), e = e [0]* s * s + e [1] * s + +e [2],
            //         t = new Date, t = t.getHours () * s * s + t.getMinutes () * s + t.getSeconds ();
            //     return (t >= b && t <= e);
            // }
        }
    };

    /** -------------------- функции для работы с DaData ---------------------------------------------------------------- */
    /* --------------------- геолокация --------------------------------------------- */
    Drupal.getCityByIp = function () {
        var promise = $.Deferred();
        detectAddress()
            .done(function (response) {
                if (response) { promise.resolve(response.location); }
            });
        return promise;
    };
    function detectAddress() {
        var serviceUrl = "https://suggestions.dadata.ru/suggestions/api/4_1/rs/iplocate/address";
        var params = {
            type: "GET",
            contentType: "application/json",
            headers: {"Authorization": "Token " + token}
        };
        return $.ajax(serviceUrl, params);
    }


    /* --------------------- определить коды Доставок --------------------------------- */
    Drupal.loadDelivery = function (suggestion) {
        var promise = $.Deferred();
        fetchDelivery(suggestion.data.kladr_id)
            .done(function(response) {
                if (response.suggestions.length) { promise.resolve(response.suggestions[0]); } else promise.resolve({});
            });
        return promise;
    };
    function fetchDelivery(kladr_id) {
        var serviceUrl = "https://suggestions.dadata.ru/suggestions/api/4_1/rs/findById/delivery";
        var request = {"query": kladr_id};
        var params = {
            type: "POST",
            contentType: "application/json",
            headers: {"Authorization": "Token " + token},
            data: JSON.stringify(request)
        };
        return $.ajax(serviceUrl, params);
    }

    /* --------------------- определить индекс ---------------------------------------- */
    Drupal.getPostalCode = function (suggestion) {
        var promise = $.Deferred();
        suggest(suggestion.unrestricted_value, 1)
            .done(function(response) {
                if (response.suggestions.length) { promise.resolve(response.suggestions[0]); } else promise.resolve({});
            });
        return promise;
    };
    function suggest(query, count) {
        var serviceUrl = "https://suggestions.dadata.ru/suggestions/api/4_1/rs/suggest/address";
        var request = {
            query: query,
            count: count
        };
        var params = {
            type: "POST",
            contentType: "application/json",
            headers: {
                Authorization: "Token " + token
            },
            data: JSON.stringify(request)
        };
        return $.ajax(serviceUrl, params);
    }

    /* --------------------- форматирование ---------------------------------------- */
    Drupal.formatResult = function(value, currentValue, suggestion) {
        var addressValue = makeAddressString(suggestion.data);
        suggestion.value = addressValue;

        return addressValue;
    };

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

    function join(arr /*, separator */) {
        var separator = arguments.length > 1 ? arguments[1] : ", ";
        return arr.filter(function (n) {
            return n
        }).join(separator);
    }

    /* --------------------- удалить районы ---------------------------------------- */
    Drupal.removeDistricts = function (suggestions) {
        return suggestions.filter(function (suggestion) {
            return suggestion.data.city_district === null;
        });
    };

})(jQuery, Drupal, this, this.document);

