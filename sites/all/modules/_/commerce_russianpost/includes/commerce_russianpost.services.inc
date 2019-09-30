<?php

/**
 * @file
 * Shipping Service functions for the Commerce RussianPost module.
 */


/**
 * Saves an enabled service.
 *
 * @param array $service
 *   An array containing 'name', 'title', 'weight', and 'status' of the service.
 *
 * @return int
 *   The return value of the call to drupal_write_record(), FALSE on failure or
 *   SAVED_NEW / SAVED_UPDATED on success.
 */
function commerce_russianpost_service_save($service) {
  // Provide 'name' key and update record if already exists.
  $op = drupal_write_record('commerce_russianpost_service', $service, in_array($service['name'], array_keys(commerce_russianpost_load_disabled_services())) ? array('name') : array());

  // If insert was successful, notify other modules that a new service was
  // created.
  if ($op !== FALSE) {
    module_invoke_all('commerce_russianpost_service_insert', $service);
  }

  return $op;
}

/**
 * Disables a service by deleting its database row and related rules.
 *
 * @param string $name
 *   The machine name of the package service to delete.
 */
function commerce_russianpost_service_delete($name) {
  $shipping_service = commerce_shipping_service_load($name);

  // Check if any line items depend on service.
  if (commerce_russianpost_service_line_item_count($name) > 0) {
    // Change status to 'disabled' instead of deleting row.
    $data = array(
      'name' => $name,
      'status' => 'disabled',
    );
    drupal_write_record('commerce_russianpost_service', $data, 'name');
  }
  else {
    // Delete row.
    db_delete('commerce_russianpost_service')
      ->condition('name', $name)
      ->execute();
  }

  rules_config_delete(array('commerce_shipping_service_' . $name));

  watchdog('commerce_russianpost', 'Removed shipping service: @title.', array('@title' => $shipping_service['title']), WATCHDOG_NOTICE);

  // Notify other modules that this service was deleted.
  module_invoke_all('commerce_russianpost_service_delete', $shipping_service);
}

/**
 * Counts and returns the number of line items for a specified service.
 */
function commerce_russianpost_service_line_item_count($service) {
  $query = new EntityFieldQuery();

  $query
    ->entityCondition('entity_type', 'commerce_line_item', '=')
    ->fieldCondition('commerce_shipping_service', 'value', $service, '=')
    ->count();

  return $query->execute();
}

/**
 * Generates a machine name from a RussianPost service code.
 *
 * @param string $service_code
 *   A RussianPost service code (e.g. "USA.SP.AIR").
 *
 * @return string
 *   The generated machine name.
 */
function commerce_russianpost_generate_service_machine_name($service_code) {
  // Replace . with _ and convert to lower case.
  $service_code = strtolower(strtr($service_code, '.', '_'));

  // Strip any disallowed characters.
  $service_code = preg_replace('/[^a-z0-9_]+/', '', $service_code);

  return 'rp_' . $service_code;
}

/**
 * Loads all services with 'enabled' status from the database.
 */
function commerce_russianpost_load_enabled_services() {
  // Attempt to retrieve list from cache.
  $services = &drupal_static(__FUNCTION__);
  if (!isset($services)) {
    $services = db_query("SELECT * FROM {commerce_russianpost_service} WHERE status = 'enabled'")
      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);
  }

  return $services;
}

/**
 * Loads all services with 'disabled' status from the database.
 */
function commerce_russianpost_load_disabled_services() {
  // Attempt to retrieve list from cache.
  $services = &drupal_static(__FUNCTION__);
  if (!isset($services)) {
    $services = db_query("SELECT * FROM {commerce_russianpost_service} WHERE status = 'disabled'")
      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);
  }

  return $services;
}

/**
 * Resets shipping service list, rebuilds default rules & menu items.
 */
function commerce_russianpost_rebuild_enabled_services() {
  drupal_static_reset('commerce_russianpost_load_enabled_services');
  drupal_static_reset('commerce_russianpost_load_disabled_services');
  commerce_shipping_services_reset();
  entity_defaults_rebuild();
  rules_clear_cache(TRUE);
  menu_rebuild();
}

/**
 * Fetches list of all available Shipping Services from RussianPost.
 *
 * @return array
 *   An associative array keyed by the RussianPost service code.
 */
function commerce_russianpost_services_list() {
  $services_arr = &drupal_static(__FUNCTION__);

  // If the services haven't been loaded yet, do so now.
  if (!isset($services_arr)) {
    // Try to retrieve service list from cache.
    $cached_services = cache_get('commerce_russianpost_services_list');
    if ($cached_services && (!empty($cached_services->data))) {
      return $cached_services->data;
    }


    // Build the rate request for services
    //module_load_include('inc', 'commerce_russianpost', 'includes/commerce_russianpost.rates');
    //$rate_request = commerce_russianpost_build_rate_request('services');
    //$response = commerce_russianpost_api_request($rate_request, t('Requesting shipping service list from RussianPost'));МждМешокМ	Международный мешок М
    $services = array(
        'ПростаяБандероль', 'ЗаказнаяБандероль', 'ЗаказнаяБандероль1Класс', 'ЦеннаяБандероль', 'ЦеннаяПосылка', 'ЦеннаяБандероль1Класс', 'EMS', 'ПосылкаОнлайн', 'КурьерОнлайн', 'ПростоеПисьмо', 'ЗаказноеПисьмо', 'ЦенноеПисьмо', 'ПростойМультиконверт', 'ЗаказнойМультиконверт',
        'МждМешокМА', 'МждМешокМАвиа', 'МждМешокМЗаказной', 'МждМешокМАвиаЗаказной', 'МждБандероль', 'МждБандерольАвиа', 'МждБандерольЗаказная', 'МждБандерольАвиаЗаказная', 'EMS_МждДокументы', 'EMS_МждТовары',
    );
    $services_arr = array(
      // Средство доставки сообщений об ошибках сервиса
        'message' => array('name' => 'Messages'),
    );
    foreach ($services as $service) {
      $key = transliteration_get($service, '?', language_default('language'));
      $services_arr[$key] = array('name' => $service);
    }

    // Cache service list.
    cache_set('commerce_russianpost_services_list', $services_arr, 'cache', CACHE_TEMPORARY);
  }

  return $services_arr;
}
