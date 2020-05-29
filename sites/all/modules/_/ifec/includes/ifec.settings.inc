<?php

/**
 * @file
 * Admin settings pages.
 */

/**
 * IFE settings form.
 */
function ifec_settings_form($form, $form_state) {
  $form = array();

  $form['#variable_edit_form'] = TRUE;

  // General options.
  $form['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General options'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
  );

  $form['general_settings']['ifec_show_form_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show form_ids on form'),
    '#description' => t('This option will print the form_id on the form for users with the administer inline form errors permissions'),
    '#default_value' => variable_get('ifec_show_form_ids', 0),
  );

  $form['general_settings']['ifec_show_everywhere'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show everywhere'),
    '#description' => t('This option will show inline form errors on all forms. You can ignore all settings in the form conversion fieldset.'),
    '#default_value' => variable_get('ifec_show_everywhere', 0)
  );

  $form['general_settings']['ifec_display'] = array(
    '#type' => 'select',
    '#title' => t('Default display settings'),
    '#description' => t('What do you want to do with the original messages block at the top of the page?'),
    '#options' => array(
      t('Leave the messages in place (default Drupal behavior)'),
      t('Show an alternate error message (a general error message of your choice)'),
      t('Remove all messages (Show nothing)'),
    ),
    '#default_value' => variable_get('ifec_display', IFEC_MESSAGE_ALTERNATE),
  );

  $form['general_settings']['ifec_general_message'] = array(
    '#type' => 'textarea',
    '#title' => t('General error message'),
    '#description' => t('A general error message to display at the top of the page (default Drupal messages display). For use with the option "Show an alternate error message".'),
    '#default_value' => variable_get('ifec_general_message', 'Please correct all highlighted errors and try again.'),
    '#required' => TRUE,
  );

  // The form_id's.
  $form['form_ids'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form conversion'),
    '#collapsed' => FALSE,
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#theme' => 'ifec_settings_form_ids',
  );

  $form_ids = ifec_load_form_ids(TRUE);
  foreach ($form_ids as $form_id) {
    $form['form_ids'][$form_id->form_id] = array();
    $form['form_ids'][$form_id->form_id]['form_id'] = array('#markup' => $form_id->form_id);
    $form['form_ids'][$form_id->form_id]['field_types'] = array('#markup' => t('All fields will be converted'));
    $form['form_ids'][$form_id->form_id]['status'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => $form_id->status,
    );
    $form['form_ids'][$form_id->form_id]['display'] = array(
      '#type' => 'select',
      '#options' => ifec_message_display_options(),
      '#default_value' => $form_id->display,
    );
  }

  $form['form_ids']['new_form_id']['form_id'] = array(
    '#type' => 'textfield',
    '#description' => t('The "*" character is a wildcard. Ex. webform_*.'),
    '#size' => 20,
  );
  $form['form_ids']['new_form_id']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => 1,
  );
  $form['form_ids']['new_form_id']['field_types'] = array('#value' => t('All fields will be converted'));
  $form['form_ids']['new_form_id']['display'] = array(
    '#type' => 'select',
    '#options' => ifec_message_display_options(),
    '#default_value' => IFEC_MESSAGE_DEFAULT,
  );

  $form['#submit'][] = 'ifec_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Returns a list of display option for error messages.
 */
function ifec_message_display_options() {
  return array(
    IFEC_MESSAGE_DEFAULT => t('Default'),
    IFEC_MESSAGE_LEAVE => t('Leave the messages in place'),
    IFEC_MESSAGE_ALTERNATE => t('Show an alternate error messages'),
    IFEC_MESSAGE_REMOVE => t('Remove all message'),
  );
}

/**
 * IFE settings form validations.
 */
function ifec_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $new_form_id = trim($values['form_ids']['new_form_id']['form_id']);

  // Check if form_id already exists.
  if ($new_form_id && ifec_form_id_load($new_form_id)) {
    form_set_error('form_ids][new_form_id][form_id', t('The form %form_id has already been added.', array('%form_id' => $new_form_id)));
  }

  // Check if the form_id is valid.
  if (!preg_match('/^[a-z0-9_*]*$/', $new_form_id)) {
    form_set_error('form_ids][new_form_id][form_id', t('The form_id %form_id is invalid.', array('%form_id' => $new_form_id)));
  }
}

/**
 * IFE settings form submit.
 */
function ifec_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Write form_ids to the database.
  $form_ids = $values['form_ids'];
  array_pop($form_ids);

  foreach ($form_ids as $form_id => $options) {
    $object = $options;
    $object['form_id'] = $form_id;

    drupal_write_record('ifec', $object, array('form_id'));
  }

  // Create new form_id.
  if ($values['form_ids']['new_form_id']['form_id']) {
    $values['form_ids']['new_form_id']['form_id'] = trim($values['form_ids']['new_form_id']['form_id']);
    drupal_write_record('ifec', $values['form_ids']['new_form_id']);
    drupal_set_message(t('The form %form_id has been added.', array('%form_id' => $values['form_ids']['new_form_id']['form_id'])));
  }

  drupal_set_message(t('The settings have been saved'));
  ifec_load_form_ids(TRUE);
}

/**
 * Confirm delete form for IFE form_ids.
 */
function ifec_form_id_delete_form($form, $form_state, $form_id) {
  $form = array();
  $form['fid'] = array(
    '#value' => $form_id->form_id,
    '#type' => 'hidden',
  );

  return confirm_form(
    $form,
    t('Are your sure you want to delete the form %form_id', array('%form_id' => $form_id->form_id)),
    'admin/config/user-interface/ifec',
    t('This action cannot be undone.'),
    t('Delete'));
}

/**
 * Submit for confirm delete form for IFE form_ids.
 */
function ifec_form_id_delete_form_submit($form, &$form_state) {
  $form_id = $form_state['values']['fid'];
  // @todo Please review the conversion of this statement to the D7 database API syntax.
  /* db_query("DELETE FROM {ife} WHERE form_id = '%s'", $form_id) */
  db_delete('ifec')
    ->condition('form_id', $form_id)
    ->execute();
  drupal_set_message(t('The form %form_id has been deleted.', array('%form_id' => $form_id)));

  ifec_load_form_ids(TRUE);
  drupal_goto('admin/config/user-interface/ifec');
}
