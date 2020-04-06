/**
 * @file
 * jQuery code.
 * Based on code: Adrian "yEnS" Mato Gondelle, twitter: @adrianmg
 * Modifications for Drupal: Grzegorz Bartman grzegorz.bartman@openbit.pl
 */

// popupStatus - global var from strada.js

var overlay = '';
var throbber = '';


/**
 * Loading popup with jQuery.
 */
function popup_spinner_load_popup()
{
    // Loads popup only if it is disabled.
    if (popupStatus === 0) {
        jQuery("#popup-spinner-background").css({
            "opacity": "0.7",
        });
        jQuery("#popup-spinner-background").fadeIn("slow");
        jQuery("#page").toggleClass("blur");
        jQuery("#popup-spinner-window").fadeIn("slow");
        popupStatus = 1;
    }
}

/**
 * Disabling popup with jQuery.
 */
function popup_spinner_disable_popup()
{
    // Disables popup only if it is enabled.
    if (popupStatus == 1) {
        jQuery("#popup-spinner-background").fadeOut("slow").remove();
        jQuery("#popup-spinner-window").fadeOut("slow").remove();
        jQuery("#page").toggleClass("blur");
        popupStatus = 0;
        window.location.reload();
    }
}

/**
 * Centering popup.
 */
function popup_spinner_center_popup(width, height)
{
    // Request data for centering.
    var windowWidth = document.documentElement.clientWidth;
    var windowHeight = document.documentElement.clientHeight;

    var popupWidth = 0,
        popupHeight = 'auto',
        padding = 16,
        top = 0;
    if (windowHeight > 667) {
      padding = 0;
      top = (windowHeight - 667) / 2;
    }
    if (width > windowWidth - padding) { popupWidth  = windowWidth - padding; }
    else {
      if (typeof width == "undefined") { popupWidth = jQuery("#popup-spinner-window").width(); }
      else { popupWidth = width; }
      if (typeof height == "undefined") { popupHeight = jQuery("#popup-spinner-window").height(); }
      else { popupHeight = height; }
    }


    // Centering.
    jQuery("#popup-spinner-window").css({
        "width" : popupWidth + "px",
        "height" : popupHeight + "px",
        "top": (windowHeight / 3 - popupHeight / 2 > 0) ? windowHeight / 3 - popupHeight / 2 : top,
        "left": windowWidth / 2 - popupWidth / 2
    });
    // Only need force for IE6.
    jQuery("#popup-spinner-background").css({
        "height": windowHeight
    });

}

/**
 * Display popup message.
 */
function popup_spinner_display_popup(popup_spinner_title, popup_spinner_body, width, height, close)
{
    if (typeof close == "undefined") close = true;

    var close_markup = '';
    if ( close ) {
        close_markup = "<a id='popup-spinner-close'><i class=\"fal fa-times\"></i></a>";
    }
    if (!jQuery("div").is("#popup-spinner-window")) {
        jQuery('body').append("<div id='popup-spinner-window'>"
            + close_markup
            + "<div id='popup-spinner-content'>"
            +   "<div id='popup-spinner-spinner'>"
            +     "<div class='spinner'><img src='/sites/all/modules/_/popup_spinner/images/spinner.png' class='img-responsive'></div>"
            +     "<div class='pointer'><i class=\"fas fa-triangle\"></i></div>"
            +   "</div>"
            +   "<div id='popup-spinner-form' class='ajax-host'>"
            +       "<h2>Испытайте удачу!</h2>"
            +       "<div class='field field-promo'>"
            +           "<p>Введите номер телефона и вращайте</p>"
            +           "<h4>Колесо Фортуны</h4>"
            +           "<p>Получайте гарантированный подарок.</p>"
            +       "</div>"
            +       "<div class='field field-name'>"
            +          "<input type='text' id='name' placeholder='Ваше имя'/>"
            +       "</div>"
            +       "<div class='field field-phone'>"
            +           "<input type='text' id='phone' class=\"masked-phone\" placeholder='Телефон'/>"
            +        "</div>"
            +        "<div class='field actions'>"
            +           "<button class='btn btn-brand btn-wide' id='popup-spinner-submit' onclick='popup_spinner_form_submit(this);'>Вращать колесо</button>"
            +        "</div>"
            +       "<div class='field field-conditions'>"
            +           "<p> Вращая колесо, вы соглашаетесь с <a href='/info/koleso-fortuny'>условиями</a> акции. Доступно одно вращение</p>"
            +       "</div>"
            +   "</div>"
            + "</div></div>"
            + "<div id='popup-spinner-background'></div>");
    }

    // Loading popup.
    popup_spinner_center_popup(width, height);
    popup_spinner_load_popup();

    Drupal.attachBehaviors('#popup-spinner-window');

    // Closing popup.
    // Click the x event!
    if ( close ) {
        jQuery("#popup-spinner-close").click(function() {
            popup_spinner_disable_popup();
        });
        // Click out event!
        jQuery("#popup-spinner-background").click(function() {
            popup_spinner_disable_popup();
        });
        // Press Escape event!
        jQuery(document).keypress(function(e) {
            if (e.keyCode == 27 && popupStatus == 1) {
                popup_spinner_disable_popup();
            }
        });
    }
}

/**
 * Helper function for get last element from object.
 * Used if on page is loaded more than one message.
 */
function popup_spinner_form_submit(el)
{
    // отправляем данные на сервер
    var settings = {
        url : Drupal.settings.popup_spinner.base_url + '/services/popup-spinner',
        submit: {
            data: {
                name : jQuery('#name').val(),
                phone : jQuery('#phone').val(),
            }
        }
    };
    var ajax = new Drupal.ajax(false, el, settings);
    ajax.eventResponse(ajax, {});
}

function popup_spinner_run_popup()
{
    // Get variables.
    var popup_spinner_title = Drupal.settings.popup_spinner.title,
        popup_spinner_body = Drupal.settings.popup_spinner.body,
        popup_spinner_width = Drupal.settings.popup_spinner.width,
        popup_spinner_height = Drupal.settings.popup_spinner.height;
        popup_spinner_close = Drupal.settings.popup_spinner.close;

    popup_spinner_display_popup(popup_spinner_title, popup_spinner_body, popup_spinner_width, popup_spinner_height, popup_spinner_close);
}



(function($) {
  // spin&win callback
  Drupal.ajax.prototype.commands.ajax_command_spin = function(ajax, response, status) {
    // blur form with ajax throbber
    overlay = $('<div class="overlay-disabled"></div>');
    throbber = $('<div class="ajax-progress-custom ajax-progress-throbber-custom"><div class="throbber">&nbsp;</div></div>');
    var throbber_host = $("#popup-spinner-submit").closest('.ajax-host');
    throbber_host.append(overlay);
    throbber_host.append(throbber);

    // spin it!
    var rand = Math.floor(Math.random() * (55 - 5 + 1)) + 5; // rand 5..55
    var deg = 360*10 - (response.gift.id - 1)*60 + (rand - 30);

    $(".spinner").css("transform", "rotate(" + deg + "deg)");

    function popup_spinner_award() {
      throbber.remove();
      overlay.remove();
      $("#popup-spinner-form").html(response.gift.output);
    }
    setTimeout(popup_spinner_award, 5500);
  };

  // error callback
  Drupal.ajax.prototype.commands.ajax_command_error = function(ajax, response, status) {
    $("#popup-spinner-submit").blur();
    if (response.error.id) {
      $("#" + response.error.id)
        .css("color", "red")
        .after('<div class="help-block"><span style="color: #ff958d;">' + response.error.text + '</span></div>')
        .keyup(function(){
          $(this).css("color", "#333").next(".help-block").remove();
        });
    }
  };


  Drupal.behaviors.popup_spinner = {
    attach: function(context) {
      $('body').once(function () {
        $("body").append(
          '<div id="gift" style="width: 70px;height: 70px;position: fixed; top: 45%; right: 30px;">' +
            '<img src="/sites/all/modules/_/popup_spinner/images/gift.png" class="img-responsive">' +
          '</div>'
        )
      });

      if (popupStatus === 0) {
        $('#gift').on('click', popup_spinner_run_popup);
      }

      $(".masked-phone").mask("9 (999) 999-9999");

    }
  };
})(jQuery);
