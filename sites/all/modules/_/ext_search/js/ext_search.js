(function($){

    Drupal.behaviors.extSearch = {
        attach: function (context, settings) {

            // переопределяем функцию из SearchAPI LiveResults
            // отключавшую нажатие на сабмит при появлении результатов
            Drupal.autocompleteSubmitLiveResults = function () {
            };

        }
    };

})(jQuery);