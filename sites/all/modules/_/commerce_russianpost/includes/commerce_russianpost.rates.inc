<?php

/**
 * @file
 * Rating functions for the Commerce Canada Post module.
 */


/**
 * Returns a SimpleXML Element containing the rate request for an order.
 *
 * @param object $order
 *   The order to generate a rate request for.
 *
 * @return array
 *   A SimpleXML Element containing the rate request or FALSE if a rate request
 *   could not be generated.
 */
function commerce_russianpost_build_rate_request($order)
{
  // проверка веса и размеров
  $check = ext_order_check_product_parameters($order);
  if ($check['weight']['weight'] > 20) {
    $request['errors']['weight'] = 'Вес превышает 20 кг';
  }
  elseif (!empty($check['errors']['weight'])) {
    $request['errors'] = $check['errors'];
  }

  if (empty($request['errors'])) {
    $api_key = variable_get('commerce_russianpost_api_key', '');
    $request = [
      'key' => $api_key,
    ];

    // если выполняется запрос доступных сервисов
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

    // Determine the shipping profile reference field name for the order.
    $field_name = commerce_physical_order_shipping_field_name($order);
    $shipping_profile = $order_wrapper->{$field_name}->value();

    $zipcode = '';
    if (isset($shipping_profile->field_data['und'][0]['value'])) {
      $data = json_decode($shipping_profile->field_data['und'][0]['value'], TRUE);
      $zipcode = empty($data['region']['data']['postal_code']) ? '' : $data['region']['data']['postal_code'];
    }
    if (empty($zipcode)) {
      $request['errors']['zipcode'] = 'выберите населенный пункт из списка';
    }

    // Calculate order total (products only) and convert it to RUB.
    $order_total = commerce_line_items_total($order_wrapper->commerce_line_items, ['product']);
    $request['content-value'] = commerce_currency_convert($order_total['amount'], $order_total['currency_code'], 'RUB') / 100;

    // Build rate request
    $request['origin-postal-code'] = variable_get('commerce_russianpost_origin');
    $physical_weight = commerce_physical_order_weight($order, 'g');
    if (empty($physical_weight['weight'])) {
      $request['errors']['weight'] = 'Не задан вес товара';
    }
    else {
      $request['weight'] = $physical_weight['weight'];
    }

    // Set shipping destination.
    $request['country-code'] = 'RU';
    $request['postal-code'] = $zipcode;

    // Calculate expected mailing date.
    $turnaround_time = variable_get('commerce_russianpost_turnaround');
    if ($turnaround_time) {
      $request['expected-mailing-date'] = date('d.m.Y', strtotime('+' . $turnaround_time . ' hours'));
    }
  }

  return $request;
}

/**
 * Parse a valid XML response into a commerce_shipping rates array.
 *
 * @param SimpleXMLElement $response
 *   A SimpleXML Element containing the response from the API server.
 *
 * @return array
 *   A commerce_shipping rates array indexed by shipping service or an empty
 *   array if there was a problem parsing the data.
 */
function commerce_russianpost_parse_rate_response($response) {
  $rates = array();

  // Build rates array.
  foreach ($response as $quote) {
    $service_code = $quote['key'];
    $amount = (float) $quote['cost'];
    // добавить стоимость коробки
    $amount += (int)variable_get('box_cost', 0);
    $currency_code = 'RUB';

    // Add an item to the rates array for the current service.
    $rates[commerce_russianpost_generate_service_machine_name($service_code)] = array(
        'amount' => commerce_currency_decimal_to_amount($amount, $currency_code),
        'currency_code' => $currency_code,
        'data' => array(
            'delivery_time' => (string) $quote['delivery_time'],
            'package_count' => (string) $quote['package_count'],
        ),
    );
  }

  return $rates;
}
