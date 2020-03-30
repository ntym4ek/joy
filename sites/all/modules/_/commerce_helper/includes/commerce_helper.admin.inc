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
    $regions = [];
    if ($vid = taxonomy_vocabulary_machine_name_load('regions')->vid) {
        $tree = taxonomy_get_tree($vid);
        foreach ($tree as $term) {
            $regions[$term->tid] = $term->name;
        }
    }

    // вкладки с настройками регионов
    $form['vertical_tabs'] = array('#type' => 'vertical_tabs');

    $form['fieldset1'] = array('#type' => 'fieldset', '#title' => 'Тарифная зона 1', '#group' => 'vertical_tabs');
    $form['fieldset1']['zone1_amount'] = array(
        '#type' => 'textfield',
        '#size' => 10,
        '#title' => 'Бесплатно от суммы',
        '#default_value' => variable_get('zone1_amount', 0),
    );
    $form['fieldset1']['zone1_regions'] = array(
        '#type' => 'select',
        '#title' => 'Регионы',
        '#chosen' => TRUE,
        '#multiple' => TRUE,
        '#options' => $regions,
        '#description' => 'Список регионов, для которых действует условие бесплатной доставки. Если список пустой, то условия будут действовать для всех регионов не вошедших в Зону 2 и 3.',
        '#default_value' => variable_get('zone1_regions', []),
    );

    $form['fieldset2'] = array('#type' => 'fieldset', '#title' => 'Тарифная зона 2', '#group' => 'vertical_tabs');
    $form['fieldset2']['zone2_amount'] = array(
        '#type' => 'textfield',
        '#title' => 'Бесплатно от суммы',
        '#default_value' => variable_get('zone2_amount', 0),
    );
    $form['fieldset2']['zone2_regions'] = array(
        '#type' => 'select',
        '#title' => 'Регионы',
        '#chosen' => TRUE,
        '#multiple' => TRUE,
        '#options' => $regions,
        '#description' => 'Список регионов, для которых действует условие бесплатной доставки.',
        '#default_value' => variable_get('zone2_regions', []),
    );

    $form['fieldset3'] = array('#type' => 'fieldset', '#title' => 'Тарифная зона 3', '#group' => 'vertical_tabs');
    $form['fieldset3']['zone3_amount'] = array(
        '#type' => 'textfield',
        '#title' => 'Бесплатно от суммы',
        '#default_value' => variable_get('zone3_amount', 0),
    );
    $form['fieldset3']['zone3_regions'] = array(
        '#type' => 'select',
        '#title' => 'Регионы',
        '#chosen' => TRUE,
        '#multiple' => TRUE,
        '#options' => $regions,
        '#description' => 'Список регионов, для которых действует условие бесплатной доставки.',
        '#default_value' => variable_get('zone3_regions', []),
    );

  $form['fieldset4'] = array('#type' => 'fieldset', '#title' => 'Киров', '#group' => 'vertical_tabs');
  $form['fieldset4']['zone4_amount'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => 'Бесплатно от суммы',
    '#default_value' => variable_get('zone4_amount', 0),
  );
  $form['fieldset4']['zone4_regions'] = array(
    '#type' => 'hidden',
    '#value' => [624],
  );

  $form['fieldset5'] = array('#type' => 'fieldset', '#title' => 'Кирово-Чепецк', '#group' => 'vertical_tabs');
  $form['fieldset5']['zone5_amount'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => 'Бесплатно от суммы',
    '#default_value' => variable_get('zone5_amount', 0),
  );
  $form['fieldset4']['zone5_regions'] = array(
    '#type' => 'hidden',
    '#value' => [625],
  );

    return system_settings_form($form);
}
