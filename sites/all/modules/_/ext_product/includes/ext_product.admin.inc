<?php

/**
 * @file
 * Admin UI.
 */

/**
 * Settings.
 */
function ext_product_api_settings($form, &$form_state)
{
    $form['api_key'] = array(
        '#type' => 'textfield',
        '#title' => 'Ключ API',
        '#description' => 'Ключ для дступа к API сайта',
        '#default_value' => variable_get('api_key', null),
    );

    return system_settings_form($form);
}
