<?php

/**
 * @file
 * Admin UI.
 */

/**
 * Settings.
 */
function add_marketing_settings($form, &$form_state)
{
    $form['gtm_account_id'] = array(
        '#type' => 'textfield',
        '#title' => 'Идентификатор GTM',
        '#description' => '',
        '#default_value' => variable_get('gtm_account_id', null),
        '#required' => TRUE,
    );

    return system_settings_form($form);
}
