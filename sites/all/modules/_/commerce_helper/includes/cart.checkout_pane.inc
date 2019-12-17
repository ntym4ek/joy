<?php

/**
 * Checkout pane callback: returns the contents View for inclusion in the checkout form.
 */
function cart_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {

    $order = commerce_order_load($order->order_id);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    if (!empty($form_state['triggering_element'])
        && strpos($form_state['triggering_element']['#name'], 'delete-line-item') !== false) {
        $lid = str_replace('delete-line-item-', '', $form_state['triggering_element']['#name']);
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $lid);
        $line_item_wrapper->delete();

        // чистим кеш сохраненных расчётов - не нужно, тк кэш индивидуален для суммы-веса, а он изменился
//        cache_clear_all('russianpost_service_rate', 'cache', TRUE);
//        cache_clear_all('transport_service_rate', 'cache', TRUE);
    }

    $checkout_form = array(
        '#title' => '',
        '#type' => 'fieldset',
        '#weight' => -2,
        '#attributes' => array('class' => array('checkout-cart')),
        '#prefix' => '<div id="checkout-cart-ajax-wrapper">',
        '#suffix' => '</div>',
    );


//    $checkout_form['table']['header'] = array(
//        '#type' => 'value',
//        '#value' => array('Товар', 'Количество', 'Цена', 'Сумма', 'Действие'),
//    );

    if ($order_wrapper->commerce_line_items->count()) {

        $order_total = $weight = $volume = 0;
        foreach ($order_wrapper->commerce_line_items as $item_wrapper) {
            if ($item_wrapper->type->value() != 'shipping' && $item_wrapper->line_item_label->value() != t('Gift')) {
                $lid = $item_wrapper->line_item_id->value();
                $qty = $item_wrapper->quantity->value();
                // если нажата кнопка Обновления количества или Подтверждения заказа
                if (!empty($form_state['triggering_element'])
                    && ($form_state['triggering_element']['#name'] == 'update-cart' || $form_state['triggering_element']['#name'] == 'op')
                ) {
                    $new_qty = $form_state['input']['cart']['c-' . $lid]['qty'];

                    if ($qty != $new_qty) {
                        $qty = $new_qty;
                        $item_wrapper->quantity = $qty;
                        $item_wrapper->save();
                        $order_wrapper->save();  // тоже сохраняем для обновления информации о сумме заказа
                    }
                }

                $price = $item_wrapper->commerce_unit_price->amount->value() / 100;
                $total = $item_wrapper->commerce_total->amount->value() / 100;
                $balance = $item_wrapper->commerce_total->value();

                $commerce_product_wrapper = $item_wrapper->commerce_product;
                $pid = $commerce_product_wrapper->product_id->value();
                $title = $commerce_product_wrapper->title->value();
                $image = $commerce_product_wrapper->field_p_image->value();


                $nid = _get_product_display_by_product_id($pid);

                $checkout_form['c-' . $lid] = array(
                    '#type' => 'container',
                    '#attributes' => array('class' => array('cc-line-item')),
                );

                $checkout_form['c-' . $lid]['image'] = array(
                    '#markup' => '<div class="cc-image"><img src="' . image_style_url('thumbnail', $image[0]['uri']) . '" /></div>',
                );

                $checkout_form['c-' . $lid]['product'] = array(
                    '#markup' => '<a href="' . url('node/' . $nid) . '" target="_blank">' . $title . '</a>',
                    '#prefix' => '<div class="cc-product">',
                    '#suffix' => '</div>',
                );

                $checkout_form['c-' . $lid]['qty'] = array(
                    '#type' => 'textfield',
                    '#default_value' => (integer)$qty,
                    '#attributes' => array('class' => array('qty-input')),
                    '#prefix' => '<div class="cc-qty"><div class="input-group"><div class="input-group-btn"><button class="btn btn-default qty-minus" type="button"><i class="fa fa-minus" aria-hidden="true"></i></button></div>',
                    '#suffix' => '<div class="input-group-btn"><button class="btn btn-default qty-plus" type="button"><i class="fa fa-plus" aria-hidden="true"></i></button></div></div></div>',
                );

                $checkout_form['c-' . $lid]['price'] = array(
                    '#markup' => commerce_currency_format($price, $balance['currency_code'], NULL, FALSE),
                    '#prefix' => '<div class="cc-price">',
                    '#suffix' => '</div>',
                );

                $checkout_form['c-' . $lid]['total'] = array(
                    '#markup' => commerce_currency_format($total, $balance['currency_code'], NULL, FALSE),
                    '#prefix' => '<div class="cc-total">',
                    '#suffix' => '</div>',
                );

                $checkout_form['c-' . $lid]['delete'] = array(
                    '#type' => 'button',
                    '#value' => '<i class="fa fa-trash" aria-hidden="true"></i>',
                    '#name' => 'delete-line-item-' . $lid,
                    '#ajax' => array(
                        'callback' => 'cart_ajax_refresh',
                    ),
                    '#prefix' => '<div class="cc-delete">',
                    '#suffix' => '</div>',
                    //'#attributes' => array('class' => array('btn-xs')),
                );

                // считаем суммы веса, объёма, стоимости
                $order_total += $total;
                $weight = commerce_physical_order_weight($order, 'kg');
                $volume = commerce_physical_order_volume($order, 'm');

            }
        }

        // вывести в корзине вес и объём товара
        $physical = '<div class="physical-total">' . t('Weight') . ': ' . (empty($weight) ? '- ' : '<span class="value">' . $weight['weight'] . '</span> ' . t($weight['unit'])) . '&nbsp;&nbsp;&nbsp;' . t('Volume') . ': <span class="value">' . (float)number_format($volume['volume'], 4) . '</span> ' . t('m<span class="sup">3</span>') . '</div>';
        $total = '<div class="order-total">' . commerce_currency_format($order_total, $balance['currency_code'], NULL, FALSE) . '</div>';
        $form_state['data']['products_amount'] = $order_total;
        $form_state['data']['products_currency'] = $balance['currency_code'];

        $checkout_form['order_total'] = array(
            '#markup' => '<div class="order-summary">' . $physical . $total . '</div>',
        );

        $checkout_form['cart-submit'] = array(
            '#type' => 'button',
            '#value' => 'Обновить',
            '#name' => 'update-cart',
            '#limit_validation_errors' => false,
            '#ajax' => array(
                'callback' => 'cart_ajax_refresh',
                'event' => 'click',
            ),
        );
    } else {
        $form_state['cart_is_empty'] = true;
    }

    return $checkout_form;
}

/**
 * Checkout pane ajax callback: refresh form.
 */
function cart_ajax_refresh ($form, $form_state) {

    if (empty($form_state['cart_is_empty'])) {
        // вернуть форму и виджет корзины
        $commands = array(
            ajax_command_replace('#cart', trim(commerce_helper_cart_info_block())),
            ajax_command_replace('#commerce-checkout-ajax-wrapper', trim(drupal_render($form))),
        );
    } else {
        // если корзина пуста, редирект на главную
        ctools_include('ajax');
        $commands = array(ctools_ajax_command_redirect('/'));
    }

    return array(
        '#type' => 'ajax',
        '#commands' => $commands,
    );
}

/**
 * Checkout pane callback: validation callback.
 */
function cart_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order)
{
    return TRUE;
}

/**
 * Checkout pane callback: submit callback.
 */
function cart_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
}