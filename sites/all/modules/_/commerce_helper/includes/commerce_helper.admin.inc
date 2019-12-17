<?php

/**
 * @file
 * Admin UI.
 */

/**
 * Settings.
 */
function commerce_helper_settings($form, &$form_state)
{
    $form['free_shipping_sum'] = array(
        '#type' => 'textfield',
        '#title' => 'Бесплатная доставка от суммы',
        '#description' => 'Минимальная сумма заказа для обнуления стоимости доставки. 0 - отключено',
        '#default_value' => variable_get('free_shipping_sum', 0),
        '#required' => TRUE,
    );
    $form['free_shipping_show_sum'] = array(
        '#type' => 'textfield',
        '#title' => 'Сообщение о бесплатной доставке',
        '#description' => 'Сумма остатка до бесплатной доставки при которой выводить сообщение. 1 - выводить при любом остатке, 0 - не выводить никогда',
        '#default_value' => variable_get('free_shipping_show_sum', 0),
        '#required' => TRUE,
    );

    $form['free_shipping_weight'] = array(
        '#type' => 'textfield',
        '#title' => 'Бесплатная доставка по весу',
        '#description' => 'Вес в КГ до которого доставка может быть бесплатной. 0 - вес не учитывается',
        '#default_value' => variable_get('free_shipping_weight', 0),
        '#required' => TRUE,
    );

    $form['courier_delivery_cost'] = array(
        '#type' => 'textfield',
        '#title' => 'Стоимость доставки курьером',
        '#description' => 'Сумма прибавляется к стоимости доставки до ПВЗ. 1 - стоимость будет браться с сервера Boxberry',
        '#default_value' => variable_get('courier_delivery_cost', 150),
        '#required' => TRUE,
    );

    // получить список регионов
    $options = [];
    if ($vid = taxonomy_vocabulary_machine_name_load('regions')->vid) {
        $tree = taxonomy_get_tree($vid);
        foreach ($tree as $term) {
            $options[$term->tid] = $term->name;
        }
    }
    $form['non_free_shipping_regions'] = array(
        '#type' => 'select',
        '#title' => 'Платная доставка',
        '#chosen' => TRUE,
        '#multiple' => TRUE,
        '#options' => $options,
        '#description' => 'Список регионов, для которых не действует бесплатная доставка',
        '#default_value' => variable_get('non_free_shipping_regions', []),
    );

    return system_settings_form($form);
}
