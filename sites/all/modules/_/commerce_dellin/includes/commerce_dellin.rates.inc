<?php

/**
 * @file
 * Rating functions for the Commerce Delovie Linii module.
 */


/**
 * Returns a SimpleXML Element containing the rate request for an order.
 *
 * @param object $order
 *   The order to generate a rate request for.
 *
 * @return array
 */
function commerce_dellin_build_rate_request($order)
{
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Determine the shipping profile reference field name for the order.
  $field_name = commerce_physical_order_shipping_field_name($order);
  $shipping_profile = $order_wrapper->{$field_name}->value();

  $zipcode = empty($shipping_profile->field_zipcode_calc['und'][0]['value']) ? '' : $shipping_profile->field_zipcode_calc['und'][0]['value'];

  // Calculate order total (products only) and convert it to RUB.
  $order_total = commerce_line_items_total($order_wrapper->commerce_line_items, array('product'));

  // Package parameters.
  $weight = commerce_physical_order_weight($order, 'kg');
  $volume = commerce_physical_order_volume($order, 'm');

  // Postal codes
  $origin_postal_code = variable_get('commerce_dellin_origin');
  $destination_postal_code = $zipcode;

  $rate_request = array(
      'appKey'        => '3405D910-082A-11E5-AD49-00505683A6D3',// ключ приложения
//      'derivalPoint'  => $origin_postal_code,       // почтовый индекс пункта отправки  (обязательное поле)
//      'arrivalPoint'  => $destination_postal_code,  // почтовый индекс пункта прибытия (обязательный параметр)
      'derivalPoint'  => "7800000000000000000000000",
      'arrivalPoint'  => "5200000100000000000000000",
      'sizedVolume'   => $volume['volume'],         // общий объём груза в кубических метрах (обязательный параметр)
      'sizedWeight'   => $weight['weight'],         // общий вес груза в килограммах (обязательный параметр)
      'arrivalDoor'   => true,                      // необходима доставка груза до адреса    (необязательный параметр), true/false
      'statedValue'   => $order_total['amount']/100,// заявленная стоимость груза в рублях. необходимо      передать этот параметр, если требуется страхование груза с заявленной стоимостью         (необязательный параметр)
  );

  // Allow modules an opportunity to alter the request before it is sent.
  drupal_alter('commerce_dellin_build_rate_request', $rate_request, $order);

  return $rate_request;
}


