(function ($, Drupal, window, document, undefined) {

    Drupal.behaviors.boxberry = {
        attach: function (context, settings) {

            $('#boxberry_link').once(function () {
                $('#boxberry_link').bind('click', function () {
                    boxberry_open();
                });
            });

            function boxberry_open() {
                var user_region = JSON.parse($.cookie('user_region'));
                var city = user_region.value;
                var products_total = Drupal.settings.ext_checkout.products_total;
                var products_weight = Drupal.settings.ext_checkout.products_weight*1000;

                boxberry.open(callback_function,"1$qeQK1y01FnbNoAqY8dMwu21099cJ6zlc", city, "", products_total, products_weight, 0 );
            }

            function callback_function(result) {
                var data = JSON.parse($(".field-name-field-data textarea").val());
                if (!data.boxberry) data.boxberry = {};
                data.boxberry.point_data = result;
                $(".field-name-field-data textarea").val(JSON.stringify(data));

                // обновление страницы
                $("[id^='edit-commerce-shipping-recalc']").click();
            }

        }
    }

})(jQuery, Drupal, this, this.document);
