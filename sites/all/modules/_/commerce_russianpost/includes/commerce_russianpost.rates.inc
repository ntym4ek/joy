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
function commerce_russianpost_build_rate_request($order = false)
{
  $customer_number = variable_get('commerce_russianpost_customer_number');
  $api_key = variable_get('commerce_russianpost_api_key', '');

  $rate_request = array(
      'site' => $customer_number,
      'email' => $api_key,
  );

  // если выполняется запрос доступных сервисов
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Determine the shipping profile reference field name for the order.
  $field_name = commerce_physical_order_shipping_field_name($order);
  $shipping_profile = $order_wrapper->{$field_name}->value();

  $zipcode = empty($shipping_profile->field_zipcode_calc['und'][0]['value']) ? '' : $shipping_profile->field_zipcode_calc['und'][0]['value'];

  // Calculate order total (products only) and convert it to RUB.
  $order_total = commerce_line_items_total($order_wrapper->commerce_line_items, array('product'));
  $rate_request['content-value'] = commerce_currency_convert($order_total['amount'], $order_total['currency_code'], 'RUB')/100;

  // Build rate request
  $rate_request['origin-postal-code'] = variable_get('commerce_russianpost_origin');
  $physical_weight = commerce_physical_order_weight($order, 'g');
  $rate_request['weight'] = $physical_weight['weight'];

  // Set shipping destination.
  $rate_request['country-code'] = 'RU';
  $rate_request['postal-code'] = $zipcode;

  // Calculate expected mailing date.
  $turnaround_time = variable_get('commerce_russianpost_turnaround');
  if ($turnaround_time) {
    $rate_request['expected-mailing-date'] = date('d.m.Y', strtotime('+' . $turnaround_time . ' hours'));
  }

  return $rate_request;
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