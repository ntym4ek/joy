/**
 * @file
 * Utility functions for Commerce RussianPost administration form.
 */

(function ($) {
  Drupal.behaviors.commerce_russianpost = {
    attach: function (context, settings) {
      // toggle display of $, %, or entire input field when markup type changes
      $('#edit-commerce-russianpost-rate-markup-type', context).change(function () {
        if (this.value === 'none') {
          $('.form-item-commerce-russianpost-rate-markup .field-prefix').hide();
          $('.form-item-commerce-russianpost-rate-markup .field-suffix').hide();
        }
        else if (this.value === 'percentage') {
          $('.form-item-commerce-russianpost-rate-markup .field-prefix').hide();
          $('.form-item-commerce-russianpost-rate-markup .field-suffix').show();
        }
        else {
          $('.form-item-commerce-russianpost-rate-markup .field-prefix').show();
          $('.form-item-commerce-russianpost-rate-markup .field-suffix').hide();
        }
      });
    }
  };
  // fire change event once to set default display
  $(document).ready(function () {
    $('#edit-commerce-russianpost-rate-markup-type').change();
  });
}(jQuery));
