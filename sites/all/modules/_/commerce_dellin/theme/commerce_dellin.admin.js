/**
 * @file
 * Utility functions for Commerce Delovie Linii administration form.
 */

(function ($) {
  Drupal.behaviors.commerce_dellin = {
    attach: function (context, settings) {
      // toggle display of $, %, or entire input field when markup type changes
      $('#edit-commerce-dellin-rate-markup-type', context).change(function () {
        if (this.value === 'none') {
          $('.form-item-commerce-dellin-rate-markup .field-prefix').hide();
          $('.form-item-commerce-dellin-rate-markup .field-suffix').hide();
        }
        else if (this.value === 'percentage') {
          $('.form-item-commerce-dellin-rate-markup .field-prefix').hide();
          $('.form-item-commerce-dellin-rate-markup .field-suffix').show();
        }
        else {
          $('.form-item-commerce-dellin-rate-markup .field-prefix').show();
          $('.form-item-commerce-dellin-rate-markup .field-suffix').hide();
        }
      });
    }
  };
  // fire change event once to set default display
  $(document).ready(function () {
    $('#edit-commerce-dellin-rate-markup-type').change();
  });
}(jQuery));
