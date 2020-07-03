<?php

/**
 * @file
 * DaData Form API element types.
 */

/**
 * Implements hook_element_info().
 */
function dadata_element_info() {
    $types = array();

    // Textfield with dadata suggestions.
    foreach (_dadata_types() as $type) {
        $types['dadata_' . $type] = array(
            '#input' => TRUE,
            '#process' => array('dadata_textfield_process'),
            '#element_validate' => array('dadata_textfield_validate'),
            '#maxlength' => 255,
            '#theme' => array('textfield'),
            '#theme_wrappers' => array('form_element'),
            // Try to correct using Rest API via validation.
            // @todo Remove or Add support for additional parameters (#dadata_params).
            '#dadata_validate' => FALSE,
            // Customize suggestions using additional parameters for jquery plugin.
            '#dadata_params' => array(),
        );
    }

    // Textfield(s) with dadata address suggestions
    // and hidden data with address components.
    $types['dadata_address_comp'] = array(
        '#process' => array('dadata_address_comp_process'),
        // Can be a textfield or textarea.
        '#base_type' => 'textfield',
        '#element_validate' => array('dadata_address_validate'),
        '#theme_wrappers' => array('container'),
        '#dadata_validate' => FALSE,
        '#dadata_params' => array(),
    );

    // Textfield with dadata fio suggestions and hidden components.
    $types['dadata_fullname_comp'] = array(
        '#input' => TRUE,
        '#process' => array('dadata_fullname_comp_process'),
        '#element_validate' => array('dadata_fullname_comp_validate'),
        // Can be an array containing required components, TRUE means required only
        // value.
        '#required' => FALSE,
        // Correct via Rest API on validation.
        '#dadata_validate' => FALSE,
        '#autocomplete_path' => FALSE,
        '#default_value' => array(
            // Base value component - full name textfield.
            'value' => '',
            // Components: surname, name, patronymic and gender.
            'surname' => '',
            'name' => '',
            'patronymic' => '',
            'gender' => NULL,
        ),
        // Surname component, default hidden.
        '#surname' => array(),
        // Name component, default hidden.
        '#name' => array(),
        // Patronymic component, default hidden.
        '#patronymic' => array(),
        // Gender component, default radios (male|female).
        '#gender' => array(),
    );

    return $types;
}

/**
 * Processes dadata textfield element.
 */
function dadata_textfield_process($element, &$form_state, $complete_form) {
    $element['#dadata_type'] = str_replace('dadata_', '', $element['#type']);
    $element['#type'] = 'textfield';
    $element += element_info($element['#type']);
    $element['#attached']['library'][] = array('dadata', 'dadata.suggestions');
    $element['#attributes']['class'][] = 'dadata-' . $element['#dadata_type'];
    if (!empty($element['#dadata_params'])) {
        $element['#attached']['js'][] = array(
            'type' => 'setting',
            'data' => array('dadata_' . $element['#id'] => $element['#dadata_params']),
        );
    }
    return $element;
}

/**
 * Validates dadata textfield element.
 */
function dadata_textfield_validate($element, &$form_state) {
    $value = trim($element['#value']);
    if ('' != $value && $element['#dadata_validate']) {
        try {
            $result = dadata_request($element['#dadata_type'], $value);
            if ($result) {
                $item = reset($result);
                form_set_value($element, $item['value'], $form_state);
            }
        }
        catch (Exception $e) {
        }
    }
    return $element;
}

/**
 * Processes dadata_address_comp element.
 */
function dadata_address_comp_process($element, &$form_state, $complete_form) {
    // Ensure that children appear as sub-keys of this element.
    $element['#tree'] = TRUE;
    $blacklist = array(
        // Make form_builder() regenerate child properties.
        '#parents',
        '#id',
        '#name',
        // Do not copy this #process function to prevent form_builder()
        // from recursing infinitely.
        '#process',
        // Ensure proper ordering of children.
        '#weight',
        // Properties already processed for the parent element.
        '#prefix',
        '#suffix',
        '#attached',
        '#processed',
        '#theme_wrappers',
//        '#attributes',
        '#element_validate',
    );
    // Move this element into sub-element 'value'.
    unset($element['value']);
    foreach (element_properties($element) as $key) {
        if (!in_array($key, $blacklist)) {
            $element['value'][$key] = $element[$key];
        }
    }

    $element['value']['#type'] = in_array($element['#base_type'], array('textfield', 'textarea')) ? $element['#base_type'] : 'textfield';
    // Increase maxlength for textfield (default 128).
    if ('textfield' == $element['value']['#type'] && !isset($element['value']['#maxlength'])) {
        $element['value']['#maxlength'] = 255;
    }
    $element['value'] += element_info($element['#base_type']);

    // Add hidden sub-element 'data' to store complete response of suggestions.
    $element['data'] = element_info('hidden');

    $element['#attributes']['class'][] = 'dadata-address-comp';
    $element['#attached']['library'][] = array('dadata', 'dadata.suggestions');
    if (!empty($element['#dadata_params'])) {
        $element['#attached']['js'][] = array(
            'type' => 'setting',
            'data' => array('dadata_' . $element['#id'] => $element['#dadata_params']),
        );
    }

    return $element;
}

/**
 * Validates dadata_address/dadata_address_comp element.
 */
function dadata_address_validate($element, &$form_state) {
    $comp = ('dadata_address_comp' == $element['#type']);
    $value = trim($comp ? $element['value']['#value'] : $element['#value']);
    if ('' != $value && ($element['#dadata_validate'] || ($comp && empty($element['data']['#value'])))) {
        try {
            $result = dadata_request('address', $value);
            if ($result) {
                $item = reset($result);
                if ($comp) {
                    form_set_value($element['data'], $item['data'], $form_state);
                }
                else {
                    form_set_value($element, $item['value'], $form_state);
                }
            }
        }
        catch (Exception $e) {
        }
    }
    elseif ($comp && !empty($element['data']['#value'])) {
        form_set_value($element['data'], drupal_json_decode($element['data']['#value']), $form_state);
    }
    return $element;
}

/**
 * Processes dadata_fullname element.
 */
function dadata_fullname_process($element, &$form_state, $complete_form) {
    $element['#type'] = 'textfield';
    $element += element_info($element['#type']);
    $element['#attached']['library'][] = array('dadata', 'dadata.suggestions');
    $element['#attributes']['class'][] = 'dadata-fullname';
    return $element;
}

/**
 * Validation handler for the dadata_fullname element.
 */
function dadata_fullname_validate($element, &$form_state) {
    $value = trim($element['#value']);
    if ('' != $value && $element['#dadata_validate']) {
        try {
            $result = dadata_request('fullname', $value);
            if ($result) {
                $item = reset($result);
                if (drupal_strtolower($value) == drupal_strtolower($item['value'])) {
                    form_set_value($element, $item['value'], $form_state);
                }
            }
        }
        catch (Exception $e) {
        }
    }
    return $element;
}

/**
 * Process callback for the dadata_fullname_comp element.
 */
function dadata_fullname_comp_process($element, &$form_state, $complete_form) {
    // Init base component - Full name value.
    $element['value'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($element['#default_value']['value']) ? $element['#default_value']['value'] : '',
    );
    // Copy properties from original element.
    foreach (array('title', 'size', 'maxlength', 'description') as $prop) {
        $prop = '#' . $prop;
        if (isset($element[$prop])) {
            $element['value'][$prop] = $element[$prop];
        }
    }
    // Don't check max length of source element.
    unset($element['#maxlength']);
    // Process Full name components: surname, name, patronymic.
    $components = array(
        'surname' => t('Surname'),
        'name' => t('Name'),
        'patronymic' => t('Patronymic'),
    );
    foreach ($components as $component => $title) {
        $element[$component] = array('#default_value' => isset($element['#default_value'][$component]) ? $element['#default_value'][$component] : '');
        $element[$component] += $element['#' . $component];
        $element[$component] += array(
            '#type' => 'hidden',
            '#title' => $title,
        );
    }
    // Add gender.
    $element['gender'] = array('#default_value' => isset($element['#default_value']['gender']) ? $element['#default_value']['gender'] : NULL);
    $element['gender'] += $element['#gender'];
    $element['gender'] += array(
        '#type' => 'radios',
        '#title' => t('Gender'),
    );
    if ('radios' == $element['gender']['#type'] || 'select' == $element['gender']['#type']) {
        $element['gender']['#options'] = array(
            'MALE' => t('Male'),
            'FEMALE' => t('Female'),
        );
    }
    // Process #required.
    if ($element['#required']) {
        if (!is_array($element['#required'])) {
            $element['#required'] = array('value');
        }
        foreach ($element['#required'] as $component) {
            if (isset($element[$component])) {
                $element[$component]['#required'] = TRUE;
            }
        }
    }
    // Wrap components.
    return array(
            '#type' => 'container',
            '#tree' => TRUE,
            '#attached' => array(
                'library' => array(
                    array('dadata', 'dadata.suggestions'),
                ),
            ),
            '#attributes' => array(
                'class' => array('dadata-fullname-comp'),
            ),
            '#theme_wrappers' => array('container'),
        ) + $element;
}

/**
 * Validation handler for the dadata_fullname_comp element.
 */
function dadata_fullname_comp_validate($element, &$form_state) {
    // No js (somthing wrong) or need correction? try to make dadata request.
    if ($element['#dadata_validate'] &&
        (!empty($element['value']['#value']) && empty($element['surname']['#value']) && empty($element['gender']['#value']))) {
        try {
            $result = dadata_request('fullname', $element['value']['#value']);
            if ($result) {
                $item = reset($result);
                form_set_value($element['value'], $item['value'], $form_state);
                foreach (element_children($element) as $component) {
                    if (isset($item['data'][$component])) {
                        form_set_value($element[$component], $item['data'][$component], $form_state);
                    }
                }
            }
        }
        catch (Exception $e) {
        }
    }
    if ($element['#required']) {
        foreach ($element['#required'] as $component) {
            if (empty($element['#value'][$component])) {
                form_error($element[$component], t('@name field is required.', array('@name' => $element[$component]['#title'])));
            }
        }
    }
    switch ($element['gender']['#value']) {
        case 'MALE':
            form_set_value($element['gender'], DADATA_GENDER_MALE, $form_state);
            break;

        case 'FEMALE':
            form_set_value($element['gender'], DADATA_GENDER_FEMALE, $form_state);
            break;

        default:
            form_set_value($element['gender'], DADATA_GENDER_UNKNOWN, $form_state);
    }
    return $element;
}