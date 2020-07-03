<?php

/**
 * @file
 * Contains the resource callbacks for orders.
 */


/**
 * Returns a collection of customer profiles.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_customer_profile_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  return commerce_services_index_entities('commerce_customer_profile', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index customer profiles.
 */
function commerce_services_customer_profile_index_access() {
  // Because the entity access control system will filter any result sets,
  // there's no reason to limit a particular user's access. Users with the
  // administer customer profile permission will have access to view any customer profile but
  // other users will only have access to customer profiles on orders they can view.
  return TRUE;
}

/**
 * Returns a single customer profile.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_customer_profile_retrieve($profile_id, $expand_entities, $flatten_fields) {
  return commerce_services_retrieve_entity('commerce_customer_profile', $profile_id, $expand_entities, $flatten_fields);
}

/**
 * Determines an API user's access to retrieve a given customer profile.
 *
 * @param $profile_id
 *   The ID of the customer profile to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the customer profile.
 */
function commerce_services_customer_profile_retrieve_access($profile_id) {
  // Attempt to load the customer profile.
  if ($customer_profile = commerce_customer_profile_load($profile_id)) {
    // And perform the view access check.
    if (commerce_customer_profile_access('view', $customer_profile)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Customer profile not found'), 404);
  }
}

/**
 * Creates a new customer profile on an order.
 */
function commerce_services_customer_profile_create($data, $flatten_fields) {
  // Ensure the create request specifies a valid customer profile type.
  if (empty($data['type']) || !in_array($data['type'], array_keys(commerce_customer_profile_types()))) {
    return services_error(t('You must specify a valid customer profile type'), 400);
  }

  global $user;

  // Create the new customer profile.
  $customer_profile = commerce_customer_profile_new($data['type'], $user->uid);

  // Remove the type from the data array since they've been set.
  unset($data['type']);

  // Set the field and property data and save the customer profile.
  commerce_services_set_field_values('commerce_customer_profile', $customer_profile, $data, $flatten_fields);
  commerce_customer_profile_save($customer_profile);

  // Add simplified fields to the customer profile object for certain field types.
  commerce_services_decorate_entity('commerce_customer_profile', $customer_profile);

  // Flatten field value arrays if specified. This must be the last operation
  // performed as it breaks the standard field data model. An entity whose
  // fields have thus been flattened is no longer wrappable or writable.
  if ($flatten_fields == 'true') {
    $customer_profile = clone($customer_profile);
    commerce_services_flatten_fields('commerce_customer_profile', $customer_profile);
  }

  return $customer_profile;
}

/**
 * Determines an API user's access to create new customer profiles.
 */
function commerce_services_customer_profile_create_access($data) {
  // If the user has access to update the order...
  if (commerce_customer_profile_access('create')) {
    return TRUE;
  }
  else {
    return services_error(t('Access to this operation not granted'), 401);
  }
}

/**
 * Updates a customer profile.
 *
 * This callback does not use commerce_services_update_entity(), because we must
 * ensure that the unit price is rebased if its amount or currency code change
 * but the data array is not updated to include the requisite price component.
 */
function commerce_services_customer_profile_update($profile_id, $data, $flatten_fields) {
  // Load the customer profile.
  $customer_profile = commerce_customer_profile_load($profile_id);

  // Set field values using the data provided.
  commerce_services_set_field_values('commerce_customer_profile', $customer_profile, $data, $flatten_fields);

  // Save the updates to the customer profile.
  commerce_customer_profile_save($customer_profile);

  // Add simplified fields to the customer profile for certain field types.
  commerce_services_decorate_entity('commerce_customer_profile', $customer_profile);

  // Flatten field value arrays if specified. This must be the last operation
  // performed as it breaks the standard field data model. An entity whose
  // fields have thus been flattened is no longer wrappable or writable.
  if ($flatten_fields == 'true') {
    $customer_profile = clone($customer_profile);
    commerce_services_flatten_fields('commerce_customer_profile', $customer_profile);
  }

  return $customer_profile;
}

/**
 * Determines an API user's access to update a given customer profile.
 *
 * @param $profile_id
 *   The ID of the customer profile to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the customer profile.
 */
function commerce_services_customer_profile_update_access($profile_id) {
  // Attempt to load the customer profile.
  watchdog('commerce_services', $profile_id);
  if ($customer_profile = commerce_customer_profile_load($profile_id)) {
    // If the user has access to perform the operation...
    if (commerce_customer_profile_access('update', $customer_profile)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Customer profile not found'), 404);
  }
}

/**
 * Deletes a customer profile.
 *
 * @param $profile_id
 *   The ID of the customer profile to be deleted.
 */
function commerce_services_customer_profile_delete($profile_id) {
  commerce_customer_profile_delete($profile_id);
}

/**
 * Determines an API user's access to delete a given customer profile.
 *
 * @param $profile_id
 *   The ID of the customer profile to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the customer profile.
 */
function commerce_services_customer_profile_delete_access($profile_id) {
  // Attempt to load the customer profile.
  if ($customer_profile = commerce_customer_profile_load($profile_id)) {
    // If the user has access to perform the operation...
    if (commerce_customer_profile_access('delete', $customer_profile)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Customer profile not found'), 404);
  }
}
