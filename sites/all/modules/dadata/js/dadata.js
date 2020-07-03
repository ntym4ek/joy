/**
 * @file
 * Attaches the behaviors for the dadata module.
 */

(function ($) {

    Drupal.behaviors.dadata = {
        attach: function(context, settings) {
            var dadata_element_options = function (element, options) {
                var element_options = {}, sid = 'dadata_' + element.attr('id');
                if (settings[sid] != undefined) {
                    element_options = settings[sid];
                }
                return $.extend({}, options, settings.dadata, element_options);
            };

            // Process textfield elements
            var availableTypes = {
                "address": "ADDRESS",
                "party": "PARTY",
                "fullname": "NAME",
                "email": "EMAIL"
            };
            for (var dt in availableTypes) {
                $('.dadata-' + dt).once(function() {
                    var $this = $(this);
                    $this.suggestions(dadata_element_options($this, {
                        type: availableTypes[dt],
                        constraints: false
                    }));
                });
            }

            $('.dadata-address-comp').once(function() {
                var $this = $(this),
                    $value_element = $this.find('input[name$="[value]"], textarea[name$="[value]"]'),
                    $data_element = $this.find('input[name$="[data]"]');
                var options = {
                    type: "ADDRESS",
                    constraints: false,
                    onSelect: function(suggestion) {
                        $data_element.val(JSON.stringify(suggestion.data));
                    }
                };
                $value_element.suggestions(dadata_element_options($this, options));
            });

            $('.dadata-fullname-comp').once(function() {
                var $this = $(this);
                $this.find("input[name$='[value]']").suggestions({
                    serviceUrl: settings.dadata.serviceUrl,
                    token: settings.dadata.token,
                    type: "NAME",
                    onSelect: function(suggestion) {
                        $this.find("input[name$='[surname]']").val(suggestion.data.surname);
                        $this.find("input[name$='[name]']").val(suggestion.data.name);
                        $this.find("input[name$='[patronymic]']").val(suggestion.data.patronymic);
                        var $gender = $this.find("[name$='[gender]']");
                        if ($gender.length > 1) { // radios
                            $gender.each(function() {
                                this.checked = ($(this).val() == suggestion.data.gender);
                            });
                        }
                        else { // hidden, select
                            $gender.val(suggestion.data.gender);
                        }
                    }
                });
            });

        }
    };

})(jQuery);
