<?php

/**
 * @file
 * Settings forms the for Commerce Delovie Linii module.
 */


module_load_include('inc', 'commerce_dellin', 'includes/commerce_dellin.services');

/**
 * Form callback: builds the settings form.
 *
 * Used to collect API credentials and enable supported shipping services.
 */
function commerce_dellin_settings_form($form, &$form_state) {
  $api_key = variable_get('commerce_dellin_api_key', '');

  $form['sellonline'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delovie Linii web services Settings'),
    '#collapsible' => TRUE,
  );

  $form['sellonline']['commerce_dellin_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('Enter your <strong>Production</strong> API Key Number. See <a href="@url">this page</a> for instructions on how to get one (follow steps 1 to 3).', array(
      '@url' => 'https://www.dellin.ca/cpo/mc/business/productsservices/developers/services/gettingstarted.jsf',
    )),
    '#required' => TRUE,
    '#default_value' => $api_key,
  );

  $form['sellonline']['commerce_dellin_origin'] = array(
    '#type' => 'textfield',
    '#title' => t('Origin Postal Code'),
    '#description' => t('The postal code to ship packages from.'),
    '#required' => TRUE,
    '#default_value' => variable_get('commerce_dellin_origin', ''),
    '#size' => 7,
    '#maxlength' => 7,
  );

  $date_format = variable_get('commerce_dellin_delivery_date_format', '');
  if (!in_array($date_format, array_keys(system_get_date_types()))) {
    $date_format = '';
  }
  $form['sellonline']['commerce_dellin_delivery_date_format'] = array(
    '#type' => 'radios',
    '#title' => t('Delivery Date Estimate'),
    '#description' => t('Select a display format for the estimated delivery date returned by Delovie Linii. See !url to modify available date formats.', array(
      '!url' => l(t('this page'), 'admin/config/regional/date-time'),
    )),
    '#options' => commerce_dellin_option_list_date_formats(),
    '#default_value' => $date_format,
  );

  $form['sellonline']['commerce_dellin_turnaround'] = array(
    '#type' => 'textfield',
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_dellin_delivery_date_format"]' => array('value' => FALSE),
      ),
    ),
    '#title' => t('Turnaround Time'),
    '#description' => t('Amount of time to add to the delivery date estimate.'),
    '#default_value' => variable_get('commerce_dellin_turnaround', ''),
    '#field_suffix' => t('hours'),
    '#size' => 4,
    '#maxlength' => 4,
  );

  $form['sellonline']['commerce_dellin_cache_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Cached rates timeout'),
    '#description' => t('Enter the number of seconds after which cached shipping rates should no longer be considered valid.') . '<br />' . t('Set to 0 during testing to avoid caching failed rate request responses.'),
    '#default_value' => variable_get('commerce_dellin_cache_timeout', 60),
    '#field_suffix' => t('seconds'),
    '#size' => 4,
    '#maxlength' => 4,
  );

  $form['sellonline']['commerce_dellin_log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'request' => t('API request messages'),
      'response' => t('API response messages'),
    ),
    '#default_value' => variable_get('commerce_dellin_log', array()),
  );

  $form['markup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Markup / Handling Fees'),
    '#collapsible' => TRUE,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'commerce_dellin') . '/theme/commerce_dellin.admin.css',
      ),
      'js' => array(
        drupal_get_path('module', 'commerce_dellin') . '/theme/commerce_dellin.admin.js',
      ),
    ),
  );

  $rate_markup = variable_get('commerce_dellin_rate_markup', array(
    'type' => 'none',
    'amount' => 0,
  ));

  // These two fields get combined together with #prefix, #suffix, and css.
  $form['markup']['commerce_dellin_rate_markup_type'] = array(
    '#type' => 'select',
    '#title' => t('Rate Markup'),
    '#prefix' => '<div class="rate-markup-field">',
    '#options' => array(
      'none'        => t('None'),
      'flat'        => t('Flat rate'),
      'package'     => t('Flat rate per package'),
      'percentage'  => t('Percentage of shipping rate'),
    ),
    '#default_value' => $rate_markup['type'],
  );
  $form['markup']['commerce_dellin_rate_markup'] = array(
    '#type' => 'textfield',
    '#states' => array(
      'invisible' => array(
        ':input[name="commerce_dellin_rate_markup_type"]' => array('value' => 'none'),
      ),
    ),
    '#suffix' => '</div>',
    '#field_prefix' => '$',
    '#field_suffix' => '%',
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => $rate_markup['amount'],
  );

  $weight_markup = variable_get('commerce_dellin_weight_markup', array(
    'weight' => 0,
    'unit' => 'g',
  ));

  // These two fields get combined together with #prefix, #suffix, and css.
  $form['markup']['commerce_dellin_weight_markup'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight Markup'),
    '#prefix' => '<div class="weight-markup-field">',
    '#size' => 6,
    '#maxlength' => 6,
    '#default_value' => $weight_markup['weight'],
  );
  $form['markup']['commerce_dellin_weight_markup_unit'] = array(
    '#type' => 'select',
    '#suffix' => '</div>',
    '#description' => t('An amount of weight to add to each package.'),
    // Get an options list for weight units.
    // Reverse the array so that metric comes first (smallest to largest).
    '#options' => array_reverse(physical_weight_unit_options()),
    '#default_value' => $weight_markup['unit'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#suffix' => l(t('Cancel'), 'admin/commerce/config/shipping/methods'),
  );

  return $form;
}

/**
 * Validate handler: validates turnaround time and origin postal code.
 */
function commerce_dellin_settings_form_validate($form, &$form_state) {

  // Check markup = valid percentage or currency amount.
  $rate_markup = $form_state['values']['commerce_dellin_rate_markup'];
  if ($rate_markup != '') {
    switch ($form_state['values']['commerce_dellin_rate_markup_type']) {
      case 'none':
        form_set_value($form['markup']['commerce_dellin_rate_markup'], 0, $form_state);
        break;

      case 'percentage':
        // Check for and remove percent sign.
        $rate_markup = str_replace('%', '', $form_state['values']['commerce_dellin_rate_markup']);
        if (!is_numeric($rate_markup) || ($rate_markup < 0)) {
          form_set_error('commerce_dellin_rate_markup', t('You must enter a positive numeric value for rate markup / handling percentage.'));
        }
        form_set_value($form['markup']['commerce_dellin_rate_markup'], $rate_markup, $form_state);
        break;

      default:
        if (!is_numeric($form_state['values']['commerce_dellin_rate_markup']) || ($rate_markup < 0)) {
          form_set_error('commerce_dellin_rate_markup', t('You must enter a positive numeric value for rate markup / handling amount.'));
        }
    }
  }

  // Weight markup = positive float value.
  if (!is_numeric($form_state['values']['commerce_dellin_weight_markup']) || $form_state['values']['commerce_dellin_weight_markup'] < 0) {
    form_set_error('commerce_dellin_weight_markup', t('You must enter a positive numeric value for weight markup.'));
  }

  // Turnaround = positive integer.
  if ($form_state['values']['commerce_dellin_turnaround'] && !ctype_digit($form_state['values']['commerce_dellin_turnaround'])) {
    form_set_error('commerce_dellin_turnaround', t('If you enter a value for <em>Turnaround Time</em> it must be a whole number.'));
  }

  // Valid Russian postal code.
  $origin = $form_state['values']['commerce_dellin_origin'];
  // Russian postal codes: 6 digits
  if ($origin && !preg_match('/^[0-9]{6}$/', $origin)) {
    form_set_error('commerce_dellin_origin', t('The value you entered for <em>Origin Postal Code</em> is not a valid Russian postal code.'));
  }
  else {
    // Update value with formatted version.
    form_set_value($form['sellonline']['commerce_dellin_origin'], $origin, $form_state);
  }

}

/**
 * Submit handler: saves settings and enables/deletes selected services.
 */
function commerce_dellin_settings_form_submit($form, &$form_state) {
  // Save settings.
  variable_set('commerce_dellin_api_key', $form_state['values']['commerce_dellin_api_key']);
  variable_set('commerce_dellin_origin', $form_state['values']['commerce_dellin_origin']);
  variable_set('commerce_dellin_turnaround', $form_state['values']['commerce_dellin_turnaround']);
  variable_set('commerce_dellin_delivery_date_format', $form_state['values']['commerce_dellin_delivery_date_format']);
  variable_set('commerce_dellin_cache_timeout', $form_state['values']['commerce_dellin_cache_timeout']);
  variable_set('commerce_dellin_log', $form_state['values']['commerce_dellin_log']);
  variable_set('commerce_dellin_rate_markup', array(
    'type'  => $form_state['values']['commerce_dellin_rate_markup_type'],
    'amount' => (float) $form_state['values']['commerce_dellin_rate_markup'],
  ));
  variable_set('commerce_dellin_weight_markup', array(
    'weight' => (float) $form_state['values']['commerce_dellin_weight_markup'],
    'unit'  => $form_state['values']['commerce_dellin_weight_markup_unit'],
  ));

  drupal_set_message(t('Delovie Linii Shipping configuration saved.'));

  $form_state['redirect'] = 'admin/commerce/config/shipping/methods';
}

/**
 * Builds the form for deleting services.
 */
function commerce_dellin_service_delete_form($form, &$form_state, $shipping_service) {
  $form_state['shipping_service'] = $shipping_service;

  $form = confirm_form($form,
    t('Remove Delovie Linii service'),
    'admin/commerce/config/shipping/services/dellin',
    t('Are you sure you want to remove the <strong>@title</strong> service?', array('@title' => $shipping_service['title'])),
    t('Remove'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for commerce_dellin_service_delete_form().
 */
function commerce_dellin_service_delete_form_submit($form, &$form_state) {
  commerce_dellin_service_delete($form_state['shipping_service']['name']);

  // Clear service cache, rebuild default rules & menu items.
  commerce_dellin_rebuild_enabled_services();

  drupal_set_message(t('The shipping service: <strong>@title</strong> has been removed.', array('@title' => $form_state['shipping_service']['title'])));

  $form_state['redirect'] = 'admin/commerce/config/shipping/services/dellin';
}

/**
 * Displays the delete confirmation form for an existing service.
 *
 * @param string $name
 *   The machine-name of the service to delete.
 */
function commerce_dellin_service_delete_page($name) {
  return drupal_get_form('commerce_dellin_service_delete_form', commerce_shipping_service_load($name));
}

/**
 * Returns an option list of date format types.
 *
 * @return array
 *   An associative array keyed by machine name.
 */
function commerce_dellin_option_list_date_formats() {
  $date_options_list = array('' => t('Do not display'));

  foreach (system_get_date_types() as $type) {
    $date_options_list[$type['type']] = t('<strong>!title:</strong> !format', array(
      '!title' => $type['title'],
      '!format' => format_date(REQUEST_TIME, $type['type']),
    ));
  }

  return $date_options_list;
}
