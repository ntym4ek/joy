(function ($) {
    Drupal.behaviors.commerceAcf = {
        attach: function (context, settings) {
            $("[id^=edit-submit]").hide();

            $('input.quantity-input', context).each(function() {
                var $input = $(this);

                // сохранить колво для последующего сравнения
                $(this).data("value", $(this).val());

                // обновлять только если значение отличается
                $(this).on("change", function() {
                    if ($(this).data("value") !== $(this).val()) {
                        Drupal.commerceAcf.quantityChanged(this);
                    }
                });

                $input.keyup(function() {
                    Drupal.commerceAcf.quantityChanged(this);
                });

                if ($input.hasClass('quantity-input-disabled')) {
                    $input.attr('readonly', true);
                }
                if ($input.hasClass('quantity-input-spinner')) {
                    $input.spinner({
                        min: 1,
                        stop: function(event, ui) {
                            $(event.target).change();
                            $(event.target).keyup();
                        }
                    })
                }
            });

            $('input.delete-line-item', context).click(function (event) {
                var $input = $(this);
                var $cartViews = $input.closest('.view-commerce-cart-form');

                if ($cartViews.length) {
                    $cartViews.addClass('view-commerce-cart-form-loading');
                }
            });
        }
    };

    Drupal.commerceAcf = {
        timer: false,

        quantityChanged: function(element) {
            clearTimeout(Drupal.commerceAcf.timer);
            Drupal.commerceAcf.timer = setTimeout(function() {
                var $input = $(element);

                $input.closest('.view-commerce-cart-form').addClass('view-commerce-cart-form-loading');
                $input.trigger('quantityChanged');

                if ($input.hasClass('quantity-input-spinner')) {
                    $input.spinner('disable');
                }
            }, 400);
        }
    };
})(jQuery);
