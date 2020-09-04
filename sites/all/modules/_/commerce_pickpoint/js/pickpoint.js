(function ($, Drupal, window, document, undefined) {

    Drupal.behaviors.pickpoint = {
        attach: function (context, settings) {

          //
          $("#pickpoint_link").once(function () {
            $("#pickpoint_link").bind("click", function () {
              pickpoint_open();
            });
          });

          function pickpoint_open() {
            var user_region = JSON.parse($.cookie('user_region'));
            var params = {
              ikn: "9990813812",
              city: (user_region.data.settlement ? user_region.data.settlement : user_region.data.city),
            };
            // var products_total = $(".pickpoint-order-amount").val();
            // var products_weight = $(".pickpoint-order-weight").val();

            PickPoint.open(callback_function, params);
          }

          function callback_function(result) {
            var data = JSON.parse($(".field-name-field-data textarea").val());
            if (!data.pickpoint) data.pickpoint = {};
            data.pickpoint.point = result;
            $(".field-name-field-data textarea").val(JSON.stringify(data));

            // обновление страницы
            $("[id^='edit-commerce-shipping-recalc']").click();
          }

        }
    }

})(jQuery, Drupal, this, this.document);
