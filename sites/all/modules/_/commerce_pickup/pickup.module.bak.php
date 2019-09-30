<?php

/**
 * @file
 * Defines an example shipping method for testing and development.
 */


/**
 * Implements hook_commerce_shipping_method_info().
 */
function pickup_commerce_shipping_method_info() {
    $shipping_methods = array();

    $shipping_methods['pickup_shipping_method'] = array(
        'title' => t('Pickup shipping method'),
        'description' => t('Defines shipping method by pickup.'),
    );

    return $shipping_methods;
}

/**
 * Implements hook_commerce_shipping_service_info().
 */
function pickup_commerce_shipping_service_info() {
  $shipping_services = array();

  $shipping_services['pickup_shipping_service'] = array(
    'title' => t('Pickup shipping service'),
    'display_title' => t('Pickup'),
    'shipping_method' => 'pickup_shipping_method',
    'price_component' => 'shipping',
    'weight' => 2,
    'callbacks' => array(
      'rate' => 'pickup_service_rate_order',
      'details_form' => 'pickup_service_details_form',
      'details_form_validate' => 'pickup_service_details_form_validate',
      'details_form_submit' => 'pickup_service_details_form_submit',
    ),
  );

  return $shipping_services;
}

/**
 * Shipping service callback: returns a base price array for a shipping service
 * calculated for the given order.
 */
function pickup_service_rate_order($shipping_service, $order) {
    // в $order после ajax submit старые данные, загружаем заново
    // $order = commerce_order_load($order->order_id);

    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    $error = $message = '';
    $shipping_cost = 0;

    return array(
        'amount' => $shipping_cost*100,
        'currency_code' => $order_wrapper->commerce_order_total->currency_code->value(),
        'data' => array(
            'error' => $error,
            'message' => $message,
        ),
    );
}


/**
 * Allows modules to alter the options array generated to select a shipping
 * service on the checkout form.
 */
function pickup_commerce_shipping_service_rate_options_alter(&$options, $order){
    if (isset( $order->shipping_rates['pickup_shipping_service'] )) {
        $line_item = $order->shipping_rates['pickup_shipping_service'];
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

        $amount = 0;
        $data = $line_item_wrapper->commerce_unit_price->data->value();

        $txt_price = '';
        if (empty($data['error'])) {
            $txt_price = $amount ? commerce_currency_format($amount, $line_item_wrapper->commerce_unit_price->currency_code->value()) : t('free');
            $txt_price = '<span class="amount">' . $txt_price . '</span>';
        }

        $message = $delivery_date = '';
        if (!empty($data['error']) && $data['error'] == 1) $message = 'уточните адрес';
        if (!empty($data['error']) && $data['error'] == 2) $message = 'расчёт не выполнен';
        if (!empty($data['error']) && $data['error'] == 3) $message = 'сервис недоступен';

        if ($message) {
            $option = 'Самовывоз<br><span class="text-danger">Не удалось рассчитать стоимость</span>';
        } else {
            $delivery_date = '<span class="delivery-date">Можно забрать сегодня</span>';
            $option  = 'Самовывоз: ' . $txt_price . '<br />';
            $option .= $message ? '<span class="error">' . $message .  '</span>' : $delivery_date;
        }

        $options['pickup_shipping_service'] = $option;
    }
}

/**
 * Shipping service callback: returns the shipping service details form.
 */
function pickup_service_details_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service){
    // вывести инфо о пункте самовывоза
    $pane_form = array();

    $pane_form['info'] = array(
        '#type' => 'item',
        '#markup' => '<div class="form-item form-item-markup"><p>Забрать товар можно по адресу г. Кирово-Чепецк, ул. Производственная, д.6</p></div>',
    );

    return $pane_form;
}

/**
 * Shipping service callback: validates the example shipping service details.
 */
function pickup_service_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents){

}

/**
 * Shipping service callback: increases the shipping line item's unit price if
 * express delivery was selected.
 */
function pickup_service_details_form_submit($details_form, $details_values, $line_item) {
}

/**
 * Implements hook_commerce_price_component_type_info().
 */
function pickup_commerce_price_component_type_info() {
//    return array(
//        'example_shipping_service_express' => array(
//            'title' => t('Express delivery'),
//            'weight' => 20,
//        ),
//    );
}

/**
 * функция возвращающая информацию о вызывающем модуле для модуля Address Book
 * @return array
 */
function pickup_address_book_info() {
    return array('surname', 'name', 'name2', 'email', 'phone');
}
/**
 * функция, вызываемая при ajax submit в модуле Address Book
 * вставка выбранного адреса в форму
 */
function pickup_address_book_ajax_submit($form_state) {
    return order_address_book_ajax_submit($form_state);
}