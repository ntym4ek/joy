(function ($, Drupal, window, document, undefined) {

    Drupal.behaviors.boxberry = {
        attach: function (context, settings) {

            $('#boxberry_link').once(function () {
                $('#boxberry_link').bind('click', function () {
                    boxberry_open();

                });
            });

            function boxberry_open() {
                var city = $.cookie('boxberry_locality') ? JSON.parse($.cookie('boxberry_locality')) : '';
                var order_amount = $(".boxberry-order-amount").val();
                var order_weight = $(".boxberry-order-weight").val();

                boxberry.open(callback_function,"1$qeQK1y01FnbNoAqY8dMwu21099cJ6zlc", city, "", order_amount, order_weight, 0 );
            }

            function callback_function(result) {
                var data = {};
                try {
                    data = JSON.parse($(".field-name-field-data textarea").val());
                }
                catch(error) {}
                data.boxberry = result;
                $(".field-name-field-data textarea").val(JSON.stringify(data));

                $('.boxberry-set').val('1');

                $("[id^='edit-commerce-shipping-recalc']").click();
            }

        }
    }

})(jQuery, Drupal, this, this.document);
