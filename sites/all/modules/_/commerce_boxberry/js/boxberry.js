(function ($, Drupal, window, document, undefined) {

    Drupal.behaviors.boxberry = {
        attach: function (context, settings) {

            $('#boxberry_link').once(function () {
                $('#boxberry_link').bind('click', function () {
                    boxberry_open();
                });
            });

            function boxberry_open() {
                // var city = $.cookie('boxberry_locality') ? JSON.parse($.cookie('boxberry_locality')) : '';
                var user_region = JSON.parse($.cookie('user_region'));
                var city = user_region.value;
                var order_amount = $(".boxberry-order-amount").val();
                var order_weight = $(".boxberry-order-weight").val();

                boxberry.open(callback_function,"1$qeQK1y01FnbNoAqY8dMwu21099cJ6zlc", city, "", order_amount, order_weight, 0 );
            }

            function callback_function(result) {
                $.cookie('user_boxberry', JSON.stringify(result), {path: "/"});
                // для валидации на сервере
                $('.boxberry-set').val('1');
                // обновление страницы
                $("[id^='edit-commerce-shipping-recalc']").click();
            }

        }
    }

})(jQuery, Drupal, this, this.document);
