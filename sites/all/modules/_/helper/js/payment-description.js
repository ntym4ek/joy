(function ($) {
  Drupal.behaviors.payment_description = {
    attach: function (context, settings) {
      var $paymentDetails = $('#payment-details', context);
      if ($paymentDetails.length) {
        var $inputWrapper = $paymentDetails.closest('fieldset.form-wrapper').find('input:checked').parent();
        $paymentDetails.appendTo($inputWrapper);
      }
    }
  };
})(jQuery);
