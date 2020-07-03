<?php

/**
 * @file
 * DaData Webform components.
 */

/**
 * Textfield component is used as base for DaData components.
 */
module_load_include('inc', 'webform', 'components/textfield');

/**
 * Specify the default properties of a component.
 */
function _webform_defaults_dadata_fullname() {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_defaults_textfield();
}

/**
 * Generate the form for editing a component.
 */
function _webform_edit_dadata_fullname($component) {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_edit_textfield($component);
}

/**
 * Render a Webform component to be part of a form.
 */
function _webform_render_dadata_fullname($component, $value = NULL, $filter = TRUE) {
    //module_load_include('inc', 'webform', 'components/textfield');
    $element = _webform_render_textfield($component, $value, $filter);
    $element['#type'] = 'dadata_fullname';
    return $element;
}

/**
 * Display the result of a submission for a component.
 */
function _webform_display_dadata_fullname($component, $value, $format = 'html') {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_display_textfield($component, $value, $format);
}

/**
 * Calculate and returns statistics about results for this component.
 */
function _webform_analysis_dadata_fullname($component, $sids = array(), $single = FALSE) {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_analysis_textfield($component, $sids);
}

/**
 * Return the result of a component value for display in a table.
 */
function _webform_table_dadata_fullname($component, $value) {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_table_textfield($component, $value);
}

/**
 * Return the header for this component to be displayed in a CSV file.
 */
function _webform_csv_headers_dadata_fullname($component, $export_options) {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_csv_headers_textfield($component, $export_options);
}

/**
 * Format the submitted data of a component for CSV downloading.
 */
function _webform_csv_data_dadata_webform($component, $export_options, $value) {
    //module_load_include('inc', 'webform', 'components/textfield');
    return _webform_csv_data_textfield($component, $export_options, $value);
}
