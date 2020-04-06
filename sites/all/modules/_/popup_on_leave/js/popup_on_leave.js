/**
 * @file
 * jQuery code.
 * Based on code: Adrian "yEnS" Mato Gondelle, twitter: @adrianmg
 * Modifications for Drupal: Grzegorz Bartman grzegorz.bartman@openbit.pl
 */

// popupStatus - global var from strada.js

/**
 * Loading popup with jQuery.
 */
function popup_on_leave_load_popup()
{
    // Loads popup only if it is disabled.
    if (popupStatus === 0) {
        jQuery("#popup-message-background").css({
            "opacity": "0.7",
        });
        jQuery("#popup-message-background").fadeIn("slow");
        jQuery("#page").toggleClass("blur");
        jQuery("#popup-message-window").fadeIn("slow");
        popupStatus = 1;
    }
}

/**
 * Disabling popup with jQuery.
 */
function popup_on_leave_disable_popup()
{
    // Disables popup only if it is enabled.
    if (popupStatus == 1) {
        jQuery("#popup-message-background").fadeOut("slow").remove();
        jQuery("#popup-message-window").fadeOut("slow").remove();
        jQuery("#page").toggleClass("blur");
        popupStatus = 0;
    }
}

/**
 * Centering popup.
 */
function popup_on_leave_center_popup(width, height)
{
    // Request data for centering.
    var windowWidth = document.documentElement.clientWidth;
    var windowHeight = document.documentElement.clientHeight;

    var popupWidth = 0;
    if (typeof width == "undefined") {
        popupWidth = jQuery("#popup-message-window").width();
    }
    else {
        popupWidth = width;
    }
    var popupHeight = 0;
    if (typeof height == "undefined") {
        popupHeight = jQuery("#popup-message-window").height();
    }
    else {
        popupHeight = height;
    }

    // Centering.
    jQuery("#popup-message-window").css({
        "width" : popupWidth + "px",
        "height" : popupHeight + "px",
        "top": (windowHeight / 3 - popupHeight / 2 > 80) ? windowHeight / 3 - popupHeight / 2 : 80,
        "left": windowWidth / 2 - popupWidth / 2
    });
    // Only need force for IE6.
    jQuery("#popup-message-background").css({
        "height": windowHeight
    });

}

/**
 * Display popup message.
 */
function popup_on_leave_display_popup(popup_on_leave_title, popup_on_leave_body, width, height, close)
{
    if (typeof close == "undefined") close = true;

    var close_markup = '';
    if ( close ) {
        close_markup = "<a id='popup-message-close'><i class=\"fal fa-times\"></i></a>";
    }
    if (!jQuery("div").is("#popup-message-window")) {
        jQuery('body').append("<div id='popup-message-window'>"
            + close_markup
            + "<div id='popup-message-content'>"
            +   "<div id='popup-message-form'>"
            +       "<div class='field-promo'>"
            +           "<p>Вы покидаете наш сайт."
            +           "<br />Что помешало Вам совершить покупку?</p>"
            +       "</div>"
            +       "<div class='field-text'>"
            +          "<label for='text'>Сообщение</label>"
            +          "<textarea id='text' />"
            +       "</div>"
            +       "<div class='field-name'>"
            +          "<label for='name'>Ваше имя</label>"
            +          "<input type='text' id='name'/>"
            +       "</div>"
            +       "<div class='field-response'>"
            +           "<label for='response'><input type='checkbox' id='response'/> жду обратного звонка</label>"
            +       "</div>"
            +       "<div class='field-phone'>"
            +           "<label for='phone'>Телефон</label>"
            +           "<input type='text' id='phone'/>"
            +        "</div>"
            +         "<div class='field-promo'>"
            +           "<p>Спасибо, что помогаете нам стать лучше!</p>"
            +         "</div>"
            +        "<div class='actions'>"
            +           "<button class='btn btn-brand btn-wide' id='submit' onclick='popup_on_leave_form_submit();'>Отправить</button>"
            +        "</div>"
            +   "</div>"
            + "</div></div>"
            + "<div id='popup-message-background'></div>");
    }

    // Loading popup.
    popup_on_leave_center_popup(width, height);
    popup_on_leave_load_popup();

    Drupal.attachBehaviors('#popup-message-window');

    // Closing popup.
    // Click the x event!
    if ( close ) {
        jQuery("#popup-message-close").click(function() {
            popup_on_leave_disable_popup();
        });
        // Click out event!
        jQuery("#popup-message-background").click(function() {
            popup_on_leave_disable_popup();
        });
        // Press Escape event!
        jQuery(document).keypress(function(e) {
            if (e.keyCode == 27 && popupStatus == 1) {
                popup_on_leave_disable_popup();
            }
        });
    }
}

/**
 * Helper function for get last element from object.
 * Used if on page is loaded more than one message.
 */
function popup_on_leave_form_submit()
{
    // отправляем данные на сервер
    var settings = {
        url : Drupal.settings.popup_on_leave.base_url + '/services/popup-on-leave',
        submit: {
            data: {
                text : jQuery('#text').val(),
                name : jQuery('#name').val(),
                checkbox : jQuery('#response').is(":checked"),
                phone : jQuery('#phone').val(),
            }
        }
    };
    var ajax = new Drupal.ajax(false, false, settings);
    ajax.eventResponse(ajax, {});

    // обновляем страницу
    var content = "<div class='thanx'><h3>Спасибо!</h3><p>Мы обязательно рассмотрим Ваше сообщение.</p></div>";
    jQuery('#popup-message-content').fadeOut(function(){
        jQuery(this).html(content).fadeIn();
    });

}

function popup_on_leave_run_popup()
{
    // Get variables.
    var popup_on_leave_title = Drupal.settings.popup_on_leave.title,
        popup_on_leave_body = Drupal.settings.popup_on_leave.body,
        popup_on_leave_width = Drupal.settings.popup_on_leave.width,
        popup_on_leave_height = Drupal.settings.popup_on_leave.height;
        popup_on_leave_close = Drupal.settings.popup_on_leave.close;

    popup_on_leave_display_popup(popup_on_leave_title, popup_on_leave_body, popup_on_leave_width, popup_on_leave_height, popup_on_leave_close);
}

(function($) {
    Drupal.behaviors.popup_on_leave = {
        attach: function(context) {

                if (popupStatus === 0) {
                    var popup_on_leave_fire = function(e) {
                        if (e.clientY < 10) {

                            var timestamp = (+new Date());
                            var popup_on_leave_cookie = jQuery.cookie("popup_on_leave_displayed");
                            var popup_on_leave_cookie_count = jQuery.cookie("popup_on_leave_displayed_count");
                            var cart_items_count = parseInt(jQuery(".bubble.bubble-red").html());
                            var is_user_signed_in = Drupal.settings.popup_on_leave.uid > 0,
                                delay = Drupal.settings.popup_on_leave.delay ? Drupal.settings.popup_on_leave.delay * 1000 : 0;

                            // показать если есть товары в корзине
                            // и зареган и и ранее не показывалось
                            // или пользователь не авторизован и истёк период "молчания"
                            popup_on_leave_cookie = parseInt(popup_on_leave_cookie, 10);
                            if (!popup_on_leave_cookie) popup_on_leave_cookie = 0;
                            if (!popup_on_leave_cookie_count) popup_on_leave_cookie_count = 1;
                            else popup_on_leave_cookie_count = popup_on_leave_cookie_count + 1;
                            var show_popup = (cart_items_count > 0)
                                && ((is_user_signed_in && popup_on_leave_cookie_count < 4) || !is_user_signed_in)
                                && timestamp > popup_on_leave_cookie + delay;

                            if (show_popup) {
                                popup_on_leave_run_popup();
                                jQuery.cookie("popup_on_leave_displayed", timestamp, {path: '/'});
                                jQuery.cookie("popup_on_leave_displayed_count", popup_on_leave_cookie_count, {path: '/'});
                            }
                        }
                    };

                    $(document).on('mouseleave', popup_on_leave_fire);
                }
        }
    };
})(jQuery);
