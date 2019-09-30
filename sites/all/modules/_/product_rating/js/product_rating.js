(function ($) {
    Drupal.behaviors.product_rating = {
        attach: function (context, settings) {

            // вставка отзывов в popup
            $("[data-toggle=popover].btn-rating").each(function(i, obj) {
                $(this).popover({
                    html: true,
                    title: function() {
                        return $('.reviews-title').clone();
                    },
                    content: function() {
                        return $('.reviews-content').html();
                    }
                });
            });

        }
    };
})(jQuery);