(function ($) {
    Drupal.behaviors.bootstrapIFEC = {
        attach: function (context, settings) {
            if (settings.bootstrap && settings.bootstrap.formHasError) {
                var $context = $(context);
                $context.find('.form-item.has-error:not(.form-type-password.has-feedback)').once('ifec-error', function () {
                    var $formItem = $(this);
                    var $formDescription = $formItem.find('.help-block span');
                    var $input = $formItem.find(':input');
                    $input.on('keyup focus blur', function () {
                        var value = $input.val() || false;
                        $formDescription.css('display', [value ? 'none' : 'inline']);
                    });
                });
            }
        }
    };
})(jQuery);