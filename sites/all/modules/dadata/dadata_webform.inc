<?php

/**
 * @file
 * DaData Webform components.
 */

/**
 * Define Webform components.
 * @see hook_webform_component_info()
 */
function dadata_webform_component_info() {
    $components = array();
    /*$components['dadata_address'] = array(
      'label' => t('DaData Address'),
      'description' => t('Address field with DaData suggestions.'),
      'file' => 'components/dadata_fullname.inc',
    );
    $components['dadata_company'] = array(
      'label' => t('DaData Company'),
      'description' => t('Company field with DaData suggestions.'),
      'file' => 'components/dadata_fullname.inc',
    );*/
    $components['dadata_fullname'] = array(
        'label' => t('DaData Full name'),
        'description' => t('Full name field with DaData suggestions.'),
        'file' => 'components/dadata_fullname.inc',
    );
    return $components;
}