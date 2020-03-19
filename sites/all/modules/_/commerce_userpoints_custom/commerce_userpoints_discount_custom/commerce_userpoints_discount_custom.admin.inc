<?php
/**
 * Form callback for commerce_userpoints_discount_custom_menu().
 */
function commerce_userpoints_discount_custom_settings_form($form, $form_state)
{
    $form = array();
    $options = userpoints_get_categories();

    $form['commerce_userpoints_discount_custom_default_termid'] = array(
        '#type' => 'select',
        '#title' => t('Set the default userpoint discount category.'),
        '#options' => $options,
        '#default_value' => variable_get('commerce_userpoints_discount_custom_default_termid', userpoints_get_default_tid()),
    );

    $form['commerce_userpoints_discount_custom_total_price_components'] = array(
        '#type' => 'select',
        '#title' => t('Allowed price components'),
        '#default_value' => variable_get('commerce_userpoints_discount_custom_total_price_components', array()),
        '#description' => t('Select the list of price component the credit can cover.'),
        '#options' => commerce_price_component_titles(),
        '#multiple' => TRUE,
        '#size' => min(12, count(commerce_price_component_titles())),
        '#required' => TRUE,
    );

    $form['commerce_userpoints_discount_custom_order_total_cover_max_percent'] = array(
        '#type' => 'textfield',
        '#title' => 'Максимальный процент',
        '#description' => 'Процент от общей стоимости товаров, который может быть оплачен баллами',
        '#default_value' => variable_get('commerce_userpoints_discount_custom_order_total_cover_max_percent', '100'),
        '#size' => 5,
        '#required' => TRUE,
    );

    $form['commerce_userpoints_cashback_percent'] = array(
        '#type' => 'textfield',
        '#title' => 'Кэшбек',
        '#description' => 'Процент от общей стоимости товаров, который вернётся в виде баллов',
        '#default_value' => variable_get('commerce_userpoints_cashback_percent', '0'),
        '#size' => 5,
        '#required' => TRUE,
    );

    return system_settings_form($form);
}

