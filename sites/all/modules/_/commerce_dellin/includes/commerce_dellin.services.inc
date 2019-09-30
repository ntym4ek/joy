<?php

/**
 * @file
 * Shipping Service functions for the Commerce Delovie Linii module.
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
//function commerce_dellin_service_save($service) {
//  // Provide 'name' key and update record if already exists.
//  $op = drupal_write_record('commerce_dellin_service', $service, in_array($service['name'], array_keys(commerce_dellin_load_disabled_services())) ? array('name') : array());
//
//  // If insert was successful, notify other modules that a new service was
//  // created.
//  if ($op !== FALSE) {
//    module_invoke_all('commerce_dellin_service_insert', $service);
//  }
//
//  return $op;
//}

/**
 * Disables a service by deleting its database row and related rules.
 *
 * @param string $name
 *   The machine name of the package service to delete.
 */
//function commerce_dellin_service_delete($name) {
//  $shipping_service = commerce_shipping_service_load($name);
//
//  // Check if any line items depend on service.
//  if (commerce_dellin_service_line_item_count($name) > 0) {
//    // Change status to 'disabled' instead of deleting row.
//    $data = array(
//      'name' => $name,
//      'status' => 'disabled',
//    );
//    drupal_write_record('commerce_dellin_service', $data, 'name');
//  }
//  else {
//    // Delete row.
//    db_delete('commerce_dellin_service')
//      ->condition('name', $name)
//      ->execute();
//  }
//
//  rules_config_delete(array('commerce_shipping_service_' . $name));
//
//  watchdog('commerce_dellin', 'Removed shipping service: @title.', array('@title' => $shipping_service['title']), WATCHDOG_NOTICE);
//
//  // Notify other modules that this service was deleted.
//  module_invoke_all('commerce_dellin_service_delete', $shipping_service);
//}

/**
 * Counts and returns the number of line items for a specified service.
 */
//function commerce_dellin_service_line_item_count($service) {
//  $query = new EntityFieldQuery();
//
//  $query
//    ->entityCondition('entity_type', 'commerce_line_item', '=')
//    ->fieldCondition('commerce_shipping_service', 'value', $service, '=')
//    ->count();
//
//  return $query->execute();
//}

/**
 * Loads all services with 'enabled' status from the database.
 */
//function commerce_dellin_load_enabled_services() {
//  // Attempt to retrieve list from cache.
//  $services = &drupal_static(__FUNCTION__);
//  if (!isset($services)) {
//    $services = db_query("SELECT * FROM {commerce_dellin_service} WHERE status = 'enabled'")
//      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);
//  }
//
//  return $services;
//}

/**
 * Loads all services with 'disabled' status from the database.
 */
//function commerce_dellin_load_disabled_services() {
//  // Attempt to retrieve list from cache.
//  $services = &drupal_static(__FUNCTION__);
//  if (!isset($services)) {
//    $services = db_query("SELECT * FROM {commerce_dellin_service} WHERE status = 'disabled'")
//      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);
//  }
//
//  return $services;
//}

/**
 * Resets shipping service list, rebuilds default rules & menu items.
 */
//function commerce_dellin_rebuild_enabled_services() {
//  drupal_static_reset('commerce_dellin_load_enabled_services');
//  drupal_static_reset('commerce_dellin_load_disabled_services');
//  commerce_shipping_services_reset();
//  entity_defaults_rebuild();
//  rules_clear_cache(TRUE);
//  menu_rebuild();
//}

