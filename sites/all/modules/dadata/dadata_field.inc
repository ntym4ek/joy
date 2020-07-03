<?php

/**
 * @file
 * DaData field API hooks.
 */

/**
 * Implements hook_field_info().
 */
function dadata_field_info() {
    return array(
        'dadata_address' => array(
            'label' => t('DaData Address'),
            'description' => t('Address field with DaData suggestions.'),
            'default_widget' => 'dadata_address_comp',
            'default_formatter' => 'dadata_address',
        ),
        'dadata_fullname' => array(
            'label' => t('DaData Full name'),
            'description' => t('Full name field with DaData suggestions.'),
            'default_widget' => 'dadata_fullname_comp',
            'default_formatter' => 'dadata_fullname',
        ),
    );
}

/**
 * Implements hook_field_is_empty().
 */
function dadata_field_is_empty($item, $field) {
    return (!isset($item['value']) || $item['value'] === '');
}

/**
 * Implements hook_field_presave().
 */
function dadata_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
    if ($field['type'] == 'dadata_address') {
        foreach ($items as $delta => $item) {
            if (!empty($item['data'])) {
                $items[$delta]['data'] = serialize($item['data']);
                $items[$delta] += $item['data'];
            }
        }
    }
}

/**
 * Implements hook_field_formatter_info().
 */
function dadata_field_formatter_info() {
    return array(
        'dadata_fullname' => array(
            'label' => t('DaData Full name'),
            'field types' => array('dadata_fullname'),
            // @todo Set up for display with prefix Mr./Ms., ...
            'settings' => array('view' => 'full'),
        ),
        'dadata_address' => array(
            'label' => t('DaData Address'),
            'field types' => array('dadata_address'),
            // @todo Use as is or customize with html and address component tokens
            'settings' => array(),
        ),
    );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function dadata_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
    $display = $instance['display'][$view_mode];
    $settings = $display['settings'];
    $element = array();

    if ('dadata_fullname' == $display['type']) {
        $element['view'] = array(
            '#type' => 'radios',
            '#options' => array(
                'full' => t('Display full name'),
                'short' => t('Display surname and initials'),
            ),
            '#default_value' => $settings['view'],
        );
    }

    return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function dadata_field_formatter_settings_summary($field, $instance, $view_mode) {
    $display = $instance['display'][$view_mode];
    $settings = $display['settings'];

    $summary = '';

    if ('dadata_fullname' == $display['type']) {
        $summary = $settings['view'] == 'short' ? t('Display surname and initials') : t('Display full name');
    }

    return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function dadata_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $element = array();
    switch ($display['type']) {
        case 'dadata_fullname':
            foreach ($items as $delta => $item) {
                $value = $item['value'];
                if ($display['settings']['view'] == 'short' && !empty($item['surname'])) {
                    $value = trim($item['surname'] . ' '
                        . (!empty($item['name']) ? drupal_substr($item['name'], 0, 1) . '.' : '')
                        . (!empty($item['patronymic']) ? drupal_substr($item['patronymic'], 0, 1) . '.' : ''));
                }
                $element[$delta] = array('#markup' => check_plain($value));
            }
            break;

        case 'dadata_address':
            foreach ($items as $delta => $item) {
                $element[$delta] = array('#markup' => check_plain($item['value']));
            }
            break;
    }

    return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function dadata_field_widget_info() {
    return array(
        'dadata_address' => array(
            'label' => t('DaData Address'),
            'field types' => array('text'),
            'settings' => array('size' => '60', 'bounds' => ''),
        ),
        'dadata_address_comp' => array(
            'label' => t('DaData Address'),
            'field types' => array('dadata_address'),
            'settings' => array(),
        ),
        'dadata_fullname' => array(
            'label' => t('DaData Full name'),
            'field types' => array('text'),
            'settings' => array('size' => '60'),
        ),
        'dadata_fullname_comp' => array(
            'label' => t('DaData Full name'),
            'field types' => array('dadata_fullname'),
            'settings' => array(
                'value' => array('size' => '60', 'required' => FALSE),
                'gender' => array(
                    'type' => 'hidden',
                    'title' => t('Gender'),
                    'required' => FALSE,
                ),
            ),
        ),
        'dadata_email' => array(
            'label' => t('DaData E-mail'),
            'field types' => array('text', 'email'),
            'settings' => array('size' => '60'),
        ),
    );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function dadata_field_widget_settings_form($field, $instance) {
    $form = array();
    $widget = $instance['widget'];
    $settings = $widget['settings'];

    switch ($widget['type']) {
        case 'dadata_fullname':
        case 'dadata_email':
            $form['size'] = array(
                '#type' => 'textfield',
                '#title' => t('Size of textfield'),
                '#default_value' => $settings['size'],
                '#required' => TRUE,
                '#element_validate' => array('element_validate_integer_positive'),
            );
            break;

        case 'dadata_address':
            $form['size'] = array(
                '#type' => 'textfield',
                '#title' => t('Size of textfield'),
                '#default_value' => $settings['size'],
                '#required' => TRUE,
                '#element_validate' => array('element_validate_integer_positive'),
            );
            $form['bounds'] = array(
                '#type' => 'select',
                '#title' => t('Bounds'),
                '#options' => array(
                    'region-area' => t('Region'),
                    'city-settlement' => t('City/settlement'),
                    'street' => t('Street'),
                    'house' => t('House'),
                ),
                '#default_value' => isset($settings['bounds']) ? $settings['bounds'] : '',
                '#empty_option' => t('- None -'),
            );
            break;

        case 'dadata_fullname_comp':
            $form['value'] = array(
                '#type' => 'fieldset',
                '#title' => t('Full name'),
            );
            $form['value']['size'] = array(
                '#type' => 'textfield',
                '#title' => t('Size of textfield'),
                '#default_value' => $settings['value']['size'],
                '#required' => TRUE,
                '#element_validate' => array('element_validate_integer_positive'),
            );
            $form['value']['required'] = array(
                '#type' => 'checkbox',
                '#title' => t('Required field'),
                '#default_value' => $settings['value']['required'],
                '#states' => array(
                    'visible' => array(
                        ':input[name="instance[required]"]' => array('checked' => TRUE),
                    ),
                ),
            );

            $form['gender'] = array(
                '#type' => 'fieldset',
                '#title' => t('Gender'),
            );
            $form['gender']['type'] = array(
                '#type' => 'select',
                '#title' => t('Type'),
                '#options' => array(
                    'radios' => 'radios',
                    'select' => 'select',
                    'hidden' => 'hidden',
                ),
                '#default_value' => $settings['gender']['type'],
            );
            $form['gender']['title'] = array(
                '#type' => 'textfield',
                '#title' => t('Title'),
                '#default_value' => $settings['gender']['title'],
                '#states' => array(
                    'invisible' => array(
                        ':input[name$="gender][type]"]' => array('value' => 'hidden'),
                    ),
                ),
            );
            $form['gender']['required'] = array(
                '#type' => 'checkbox',
                '#title' => t('Required field'),
                '#default_value' => $settings['gender']['required'],
                '#states' => array(
                    'visible' => array(
                        ':input[name="instance[required]"]' => array('checked' => TRUE),
                    ),
                ),
            );
            break;

        case 'dadata_address_comp':
            break;
    }

    return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function dadata_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
    switch ($instance['widget']['type']) {
        case 'dadata_fullname':
            $element += array(
                '#type' => 'dadata_fullname',
                '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
                '#size' => $instance['widget']['settings']['size'],
            );
            $element = array('value' => $element);
            break;

        case 'dadata_fullname_comp':
            if ($element['#required']) {
                if (!is_array($element['#required'])) {
                    $element['#required'] = array();
                }
                if ($instance['widget']['settings']['value']['required']) {
                    $element['#required'][] = 'value';
                }
                if ($instance['widget']['settings']['gender']['required']) {
                    $element['#required'][] = 'gender';
                }
            }
            $element += array(
                '#type' => 'dadata_fullname_comp',
                '#default_value' => isset($items[$delta]) ? $items[$delta] : NULL,
                '#gender' => array(
                    '#type' => $instance['widget']['settings']['gender']['type'],
                    '#title' => check_plain($instance['widget']['settings']['gender']['title']),
                ),
                '#size' => $instance['widget']['settings']['value']['size'],
            );
            break;

        case 'dadata_address':
            $element += array(
                '#type' => 'dadata_address',
                '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
                '#size' => $instance['widget']['settings']['size'],
            );
            if (!empty($instance['widget']['settings']['bounds'])) {
                $element['#dadata_params']['bounds'] = $instance['widget']['settings']['bounds'];
            }
            $element = array('value' => $element);
            break;

        case 'dadata_address_comp':
            $element += array(
                '#type' => 'dadata_address_comp',
                '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : NULL,
            );
            break;

        case 'dadata_email':
            $key = $element['#columns'][0];
            $element += array(
                '#type' => 'dadata_email',
                '#default_value' => isset($items[$delta][$key]) ? $items[$delta][$key] : NULL,
                '#size' => $instance['widget']['settings']['size'],
            );
            $element = array($key => $element);
            break;
    }

    return $element;
}