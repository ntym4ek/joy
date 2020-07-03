<?php

/**
 * @file
 * Admin UI.
 */

/**
 * Settings.
 */
function dadata_settings($form, &$form_state) {
    $form['dadata_token'] = array(
        '#type' => 'textfield',
        '#title' => t('API key'),
        '#default_value' => variable_get('dadata_token', ''),
        '#required' => TRUE,
        '#description' => t('Register on the <a href="@site">DaData.ru</a> and generate key in <a href="@profile">profile</a>.',
            array(
                '@site' => url(DADATA_URL, array('external' => TRUE)),
                '@profile' => url(DADATA_URL . '/profile', array('external' => TRUE)),
            )
        ),
    );

    $form['dadata_jquery_plugin'] = array(
        '#type' => 'fieldset',
        '#title' => t('jQuery plugin'),
    );
    $form['dadata_jquery_plugin']['dadata_jquery_plugin_external'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use external JS/CSS'),
        '#default_value' => variable_get('dadata_jquery_plugin_external', 0),
        '#description' => t('Include required js/css library files from DaData.ru.'),
    );
    $form['dadata_jquery_plugin']['dadata_jquery_plugin_service_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Service URL'),
        '#default_value' => variable_get('dadata_jquery_plugin_service_url', 'https://dadata.ru/api/v2'),
        '#element_validate' => array('dadata_element_validate_url'),
        '#description' => t('The URL of service for jQuery plugin. Should be absolute (beginning with a scheme http/https).'),
    );
    $form['dadata_jquery_plugin']['dadata_jquery_plugin_params'] = array(
        '#type' => 'fieldset',
        '#title' => t('Default params'),
        '#tree' => TRUE,
        '#description' => t('You can override these params by specifying them directly for the element.'),
    );
    $params = variable_get('dadata_jquery_plugin_params', array());
    $form['dadata_jquery_plugin']['dadata_jquery_plugin_params']['triggerSelectOnSpace'] = array(
        '#type' => 'checkbox',
        '#title' => 'triggerSelectOnSpace',
        '#default_value' => isset($params['triggerSelectOnSpace']) ? $params['triggerSelectOnSpace'] : 1,
        '#description' => t('Automatically correct text as you type.'),
    );
    $form['dadata_jquery_plugin']['dadata_jquery_plugin_params']['count'] = array(
        '#type' => 'select',
        '#title' => 'count',
        '#options' => drupal_map_assoc(range(1, 20)),
        '#default_value' => isset($params['count']) ? $params['count'] : 10,
        '#description' => t('The maximum number of suggestions in the dropdown list.'),
    );
    // @todo ...

    $form['dadata_rest_api'] = array(
        '#type' => 'fieldset',
        '#title' => t('REST API'),
    );
    $form['dadata_rest_api']['dadata_rest_api_validate'] = array(
        '#type' => 'checkbox',
        '#title' => t('Validate DaData field values using REST API'),
        '#default_value' => variable_get('dadata_rest_api_validate', 1),
    );
    $form['dadata_rest_api']['dadata_rest_api_url'] = array(
        '#type' => 'textfield',
        '#title' => t('REST API URL'),
        '#default_value' => variable_get('dadata_rest_api_url', 'https://dadata.ru/api/v2/suggest'),
        '#element_validate' => array('dadata_element_validate_url'),
        '#description' => t('The URL of REST API service. Should be absolute (beginning with a scheme http/https).'),
        '#states' => array(
            'visible' => array(
                'input[name="dadata_rest_api_validate"]' => array('checked' => TRUE),
            ),
        ),
    );

    return system_settings_form($form);
}

/**
 * Form element validation handler for URL.
 */
function dadata_element_validate_url($element, &$form_state) {
    $value = $element['#value'];
    if ($value !== '' && !valid_url($value, TRUE)) {
        form_error($element, t('%name must be a valid absolute URL.', array('%name' => $element['#title'])));
    }
}