<?php

/**
 * @file
 * Provides Delovie Linii shipping quotes for Drupal Commerce.
 */


module_load_include('inc', 'commerce_dellin', 'includes/commerce_dellin.services');

/**
 * Implements hook_menu().
 */
function commerce_dellin_menu() {
  $items = array();

  $items['admin/commerce/config/shipping/methods/dellin/edit'] = array(
    'title' => 'Edit',
    'description' => 'Configure the Delovie Linii shipping method.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('commerce_dellin_settings_form'),
    'access arguments' => array('administer shipping'),
    'type' => MENU_LOCAL_TASK,
    'context' => MENU_CONTEXT_INLINE,
    'weight' => 0,
    'file' => 'includes/commerce_dellin.admin.inc',
  );

  return $items;
}

/**
 * Implements hook_commerce_shipping_method_info().
 */
function commerce_dellin_commerce_shipping_method_info() {
  $shipping_methods = array();

  $shipping_methods['dellin'] = array(
    'title' => t('Delovie Linii'),
    'description' => t('Provides Delovie Linii shipping quotes.'),
  );

  return $shipping_methods;
}

/**
 * Implements hook_commerce_shipping_service_info().
 */
function commerce_dellin_commerce_shipping_service_info() {
  $shipping_services = array();

  // Get list of all available shipping services.
  $shipping_services['dellin_shipping_service'] = array(
      'title' => t('Delovie Linii Shipping Service'),
      'display_title' => t('Delovie Linii'),
      'shipping_method' => 'dellin',
      'price_component' => 'shipping',
      'weight' => 10,
      'callbacks' => array(
        'rate' => 'commerce_dellin_service_rate_order',
        'details_form' => 'commerce_dellin_details_form',
        'details_form_validate' => 'commerce_dellin_details_form_validate',
        'details_form_submit' => 'commerce_dellin_details_form_submit',
      ),
    );

  return $shipping_services;
}

/**
 * Implements hook_commerce_price_component_type_info().
 */
function commerce_dellin_commerce_price_component_type_info() {
  return array(
      'dellin_shipping_service_to_door' => array(
        'title' => t('Delivery to the door'),
        'weight' => 25,
      ),
      'dellin_shipping_service_insurance' => array(
        'title' => t('Delivery insurance'),
        'weight' => 30,
      ),
  );
}

/**
 * Implements hook_commerce_shipping_service_rate_options_alter().
 */
function commerce_dellin_commerce_shipping_service_rate_options_alter(&$options, $order) {
  $date_format = variable_get('commerce_dellin_delivery_date_format', '');

  foreach ($order->shipping_rates as $name => $line_item) {

    if (substr($name, 0, 6) == 'dellin') {
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $data = $line_item_wrapper->commerce_unit_price->data->value();
      $amount = $line_item_wrapper->commerce_unit_price->amount->value();

      if (empty($data['errors']) && isset($data['delivery_date'])) {
        // Estimated delivery date string.
        $delivery_date = '';
        if (in_array($date_format, array_keys(system_get_date_types())) && strtotime($data['delivery_date'])) {
          $delivery_date = '<span class="delivery-date">Доставка к: ' . format_date(strtotime($data['delivery_date']), $date_format) . '</span>';
        }

        $options[$name] = t('Транспортная компания: !price<br />!delivery_date', array(
            '!price' => '<span class="amount">' . ($amount ? 'от ' . commerce_currency_format($line_item_wrapper->commerce_unit_price->amount->value(), $line_item_wrapper->commerce_unit_price->currency_code->value()) : t('free')) . '</span>',
            '!delivery_date' => $delivery_date,
        ));
      } else {
          $options[$name] = 'Транспортная компания<br><span class="text-danger">Не удалось рассчитать стоимость</span>';
      }
    }
  }
}

/**
 * Shipping service callback: returns shipping rates for a given order.
 */
function commerce_dellin_service_rate_order($shipping_service, $order)
{
    $order_wr = entity_metadata_wrapper('commerce_order', $order);

    $rate = array(
        'amount' => 0,
        'currency_code' => 'RUB',
        'data' => array(),
    );

    module_load_include('inc', 'commerce_dellin', 'includes/commerce_dellin.rates');
    $rate_request = commerce_dellin_build_rate_request($order);

    if (!empty($rate_request['arrivalPoint']) && !empty($rate_request['sizedWeight'])) {
        // поднять из кэша расчёт для этого Индекса-Суммы-Веса
        $cache_key = $rate_request['arrivalPoint'] . $rate_request['statedValue'] . $rate_request['sizedWeight'];
        $cache = commerce_shipping_rates_cache_get('dellin' . $cache_key, $order, variable_get('commerce_dellin_cache_timeout', 600));

        // No rates recovered or cache has expired.
        if (!$cache) {

            $response = commerce_dellin_api_request($rate_request, t('Requesting shipping rates for package in Order @order_number', array('@order_number' => $order->order_number)));

            // If rate request was successful.
            if ($response) {
                if (empty($response['errors'])) {
                    // если задана бесплатная доставка от суммы
                    if (CHECKOUT_FREE_SHIPPING_MIN_ORDER_AMOUNT > 0 && $order_wr->commerce_order_total->amount->value() >= CHECKOUT_FREE_SHIPPING_MIN_ORDER_AMOUNT*100) {
                        $rate['amount'] = 0;
                        $rate['data'] = array(
                            'delivery_date' => date('d.m.Y', time() + $response['time']['value'] * 24 * 3600),
                            'insurance_price' => 0,
                            'to_door_price' => 0,
                            'terminal_access_price' => 0,
                            'terminals' => $response['arrival']['terminals'],
                        );
                    } else {
                        // Parse the response into the rates array.
                        $intercity_price = empty($response['intercity']['price']) ? 0 : commerce_currency_decimal_to_amount($response['intercity']['price'], 'RUB');
                        $terminal_access_price = commerce_currency_decimal_to_amount($response['arrival']['terminals'][0]['price'], 'RUB');
                        $notify_price = empty($response['notify']['price']) ? 0 : commerce_currency_decimal_to_amount($response['notify']['price'], 'RUB');
                        $insurance_price = empty($response['insurance']) ? 0 : commerce_currency_decimal_to_amount($response['insurance'], 'RUB');
                        $to_door_price = empty($response['arrival']['price']) ? 0 : commerce_currency_decimal_to_amount($response['arrival']['price'], 'RUB');

                        $rate['amount'] = $intercity_price + $notify_price + $insurance_price;
                        $rate['data'] = [
                            'delivery_date' => date('d.m.Y', time() + $response['time']['value'] * 24 * 3600),
                            'insurance_price' => $insurance_price,
                            'to_door_price' => $to_door_price,
                            'terminal_access_price' => $terminal_access_price,
                            'terminals' => $response['arrival']['terminals'],
                        ];
                    }

                    // Apply rate markup.
                    $rate_markup_amount = commerce_currency_decimal_to_amount(commerce_dellin_service_markup($rate), $rate['currency_code']);
                    $rate['amount'] += $rate_markup_amount;

                    // Cache these rates.
                    commerce_shipping_rates_cache_set('dellin' . $cache_key, $order, $rate);
                } else {
                    if (isset($response['errors']['messages'])){
                        foreach ($response['errors']['messages'] as $key => $message) {
                            $rate['data']['errors'][] = $message;
                        }
                    } elseif (isset($response['errors'])) {
                        foreach ($response['errors'] as $key => $message) {
                            switch ($key) {
                                case 'sizedvolume':
                                    $message = 'Расчёт не выполнен, так не удалось определить объём.';
                                    break;
                                case 'sizedweight':
                                    $message = 'Расчёт не выполнен, так не удалось определить вес.';
                                    break;
                                case 'arrivalpoint':
                                    $message = 'Расчёт не выполнен, так как индекс ' . $rate_request['arrivalPoint'] . ' не найден.';
                                    break;
                                default:
                                    $message = 'Сигнатура ошибки: ' . $message;
                            }
                            $rate['data']['errors'][] = $message;
                        }
                    }
                }

            } else {
                $rate['data']['errors'][] = 'Нет ответа от сервера Деловых Линий. Попробуйте оформить заказ позже.';
            }

        } else {
            $rate = $cache;
        }
    } else {
        if (empty($rate_request['arrivalPoint'])) $rate['data']['errors'][] = 'Расчёт не выполнен, так не удалось определить индекс.';
        if (empty($rate_request['sizedWeight'])) $rate['data']['errors'][] = 'Расчёт не выполнен, так не удалось определить вес.';
    }

    // Return the rate for the requested service or FALSE if not found.
    return isset($rate) ? $rate : FALSE;
}

/**
 * Submits an API request to the Delovie Linii XML Processor.
 *
 * @param string $target_url
 *   The url to submit the request to.
 * @param string $xml
 *   An XML string to submit.
 * @param string $message
 *   (Optional) Log message to use for logged API requests.
 * @param string $alternate_api_key
 *   (Optional) An API key (if different from the one saved in the database)
 *
 * @return SimpleXMLElement
 *   Returns a SimpleXML Element containing the response from the API server or
 *   FALSE if there was no response or an error occurred.
 */
function commerce_dellin_api_request($request = '', $message = '') {
  $api_key = variable_get('commerce_dellin_api_key', '');
  $error_messages = array();

  // Abort if API key hasn't been set.
  if (empty($api_key)) {
    return FALSE;
  }

  // Build the array of header information for the request.
  $curl = curl_init();
  curl_setopt($curl, CURLOPT_URL, "https://api.dellin.ru/v1/public/calculator.json");
  curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-Type:application/json'));
  curl_setopt($curl, CURLOPT_POST, true);
  curl_setopt($curl, CURLOPT_POSTFIELDS, json_encode($request));
  curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
  $data = curl_exec($curl);

  if (curl_error($curl)) {
    $error_messages[] = curl_error($curl);
  }
  curl_close($curl);

  $response = json_decode($data, true);

  // Validate the response object.
  if (isset($response)) {
    $response_status = commerce_dellin_validate_api_response($response);

    // Collect additional error messages.
    if ($response_status['error']) {
      $error_messages = array_merge($error_messages, $response_status['messages']);
    }
  }

  // Output any error messages to watchdog.
  if (count($error_messages)) {
    watchdog('dellin', 'API request error(s):<ul><li>!errors</li></ul>', array('!errors' => implode('</li><li>', $error_messages)), WATCHDOG_ERROR);
  }

  return $response;
}

/**
 * Checks a Delovie Linii API response for errors.
 *
 * @param SimpleXMLElement $response
 *   The response element to evaluate.
 * 
 * @return array
 *   An array containing an error status and any related messages.
 */
function commerce_dellin_validate_api_response($response) {
  $status = array(
    'error' => FALSE,
    'messages' => array(),
  );

  // Check for error messages.
  if (isset($response['errors'])) {
    $status['error'] = TRUE;

    foreach ($response['errors'] as $key => $error) {
      if ($key == 'messages') {
        foreach ($error as $e) {
            $status['messages'][] = t('From Delovie Linii API: @code (@description)', array(
                '@code' => 'none',
                '@description' => $e,
            ));
        }
      } else {
          $status['messages'][] = t('From Delovie Linii API: @code (@description)', array(
              '@code' => $key,
              '@description' => $error,
          ));
      }
    }
  }

  return $status;
}

/**
 * Calculates rate markup for a service.
 *
 * @param array $rate
 *   A keyed array containing shipping rate information for a service.
 *
 * @return double
 *   The amount to add to the base rate (in decimal format).
 */
function commerce_dellin_service_markup($rate) {
  $rate_markup = variable_get('commerce_dellin_rate_markup', array('type' => 'none', 'amount' => 0));

  switch ($rate_markup['type']) {
    // Flat amount.
    case 'flat':
      return $rate_markup['amount'];

    // Flat amount per package.
    case 'package':
      return $rate_markup['amount'] * $rate['data']['package_count'];

    // Percentage of rate.
    case 'percentage':
      return ($rate_markup['amount'] / 100) * commerce_currency_amount_to_decimal($rate['amount'], $rate['currency_code']);

    default:
      return 0;
  }
}


/**
 * Shipping service callback: returns the example shipping service details form.
 */
function commerce_dellin_details_form($pane_form, $pane_values, $checkout_pane, $order, $shipping_service) {
    $pane_form = array();

    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $order->shipping_rates['dellin_shipping_service']);
    $data = $line_item_wrapper->commerce_unit_price->data->value();

    // если стоимость рассчитана - вывести опции
    if (empty($data['errors'])) {

        $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
        if (CHECKOUT_FREE_SHIPPING_MIN_ORDER_AMOUNT > 0 && $order_wrapper->commerce_order_total->amount->value() >= CHECKOUT_FREE_SHIPPING_MIN_ORDER_AMOUNT*100) {
            $pane_form['free'] = [
                '#markup' => '<h6 class="brand-success">Бесплатная доставка для заказов от ' . commerce_currency_format(CHECKOUT_FREE_SHIPPING_MIN_ORDER_AMOUNT * 100, $order_wrapper->commerce_order_total->currency_code->value()) . '</h6>'
            ];
        }

        $pane_values['service_details'] += array(
            'insurance' => '',
            'to_door' => '',
        );

        $i_price = commerce_currency_format($data['insurance_price'], $line_item_wrapper->commerce_unit_price->currency_code->value());

        $options_t = array();
        foreach($data['terminals'] as $terminal) {
            $options_t[$terminal['address']] = $terminal['address'];
        }
        $pane_form['terminal'] = array(
            '#type' => 'select',
            '#title' => t('Terminal'),
            '#options' => $options_t,
            '#default_value' => $pane_values['service_details']['terminal'],
            '#states' => array(
                'visible' => array(
                    'input[name$="[to_door]"]' => array(
                        'checked' => FALSE,
                    ),
                ),
            ),
        );

        $pane_form['insurance'] = array(
            '#type' => 'checkbox',
            '#title' => t('Insurance: +!amount', array('!amount' => $i_price)),
            '#disabled' => true,
            '#default_value' => 1,
        );

        $td_price = commerce_currency_format($data['to_door_price'], $line_item_wrapper->commerce_unit_price->currency_code->value());
        $pane_form['to_door'] = array(
            '#type' => 'checkbox',
            '#title' => t('Delivery to the door: +!amount', array('!amount' => $td_price)),
            '#default_value' => $pane_values['service_details']['to_door'],
        );

        $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
        $addr = $office = $notes = '';
        if (!empty($order->commerce_customer_shipping)) {
            $addr = $order_wrapper->commerce_customer_shipping->field_addr->value();
            $office = $order_wrapper->commerce_customer_shipping->field_office->value();
            $passport = $order_wrapper->commerce_customer_shipping->field_passport->value();
            $notes = $order_wrapper->commerce_customer_shipping->field_notes->value();
        }
        $pane_form['passport'] = array(
            '#title' => 'Паспортные данные',
            '#type' => 'textfield',
            '#default_value' => empty($pane_values['service_details']['passport']) ? $passport : $pane_values['service_details']['passport'],
        );
        $pane_form['addr'] = array(
            '#title' => 'Адрес',
            '#type' => 'textfield',
            '#default_value' => empty($pane_values['service_details']['addr']) ? $addr : $pane_values['service_details']['addr'],
            '#states' => array(
                'visible' => array(
                    'input[name$="[to_door]"]' => array(
                        'checked' => TRUE,
                    ),
                ),
            ),
        );
        $pane_form['office'] = array(
            '#title' => 'Квартира/Офис',
            '#type' => 'textfield',
            '#default_value' => empty($pane_values['service_details']['office']) ? $office : $pane_values['service_details']['office'],
            '#states' => array(
                'visible' => array(
                    'input[name$="[to_door]"]' => array(
                        'checked' => TRUE,
                    ),
                ),
            ),
        );
        // комментарий к заказу
        $pane_form['notes'] = array(
            '#title' => 'Комментарий менеджеру',
            '#type' => 'textfield',
            '#default_value' => empty($pane_values['service_details']['notes']) ? $notes : $pane_values['service_details']['notes'],
        );

        // иначе - вывести ошибки
    } else {
        $message = '<ul class="bg-danger">';
        foreach ($data['errors'] as $key => $error) {
            $message .= '<li>' . $error . '</li>';
        }
        $message .= '</ul>';

        $pane_form['message'] = array(
            '#type' => 'item',
            '#markup' => $message,
        );
    }

    return $pane_form;
}

/**
 * Shipping service callback: validates the shipping service details.
 */
function commerce_dellin_details_form_validate($details_form, $details_values, $shipping_service, $order, $form_parents)
{
    $fields = array();
    if (strlen($details_values['passport']) < 10) { $fields[] = 'passport'; }
    if ($details_values['to_door'] && drupal_strlen($details_values['addr']) < 4) { $fields[] = 'addr'; }

    if ($fields) {
        $message = 'Заполните поля, выделенные красным.';
        foreach($fields as $field) {
            form_set_error(implode('][', array_merge($form_parents, array($field))), $message);
            $message = '';
        }
        return false;
    } else {
        return true;
    }
}

/**
 * Shipping service callback.
 *
 * Increases the shipping line item's unit price if
 * express delivery was selected.
 */
function commerce_dellin_details_form_submit($details_form, $details_values, $line_item)
{
    if ($details_values['insurance']) {
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        $data = $line_item_wrapper->commerce_unit_price->data->value();

        // Build a price array for the express delivery fee.
        $i_amount = $data['insurance_price'];
        $insurance_price = array(
            'amount' => $i_amount,
            'currency_code' => $line_item_wrapper->commerce_unit_price->currency_code->value(),
            'data' => array(),
        );

        // Add the express upcharge to the line item unit price.
        $line_item_wrapper->commerce_unit_price->amount = $line_item_wrapper->commerce_unit_price->amount->value() + $i_amount;

        // Add the express delivery fee component to the unit price.
        $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
            $line_item_wrapper->commerce_unit_price->value(),
            'dellin_shipping_service_insurance',
            $insurance_price,
            TRUE,
            FALSE
        );
    }

    if ($details_values['to_door']) {
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        $data = $line_item_wrapper->commerce_unit_price->data->value();

        // стоимость доставки до двери
        $to_door_amount = $data['to_door_price'];
        // стоимость вьезда на терминал
        $terminal_access_amount = $data['terminal_access_price'];

        $to_door_summary = $to_door_amount - $terminal_access_amount;

        $to_door_price = array(
            'amount' => $to_door_summary,
            'currency_code' => $line_item_wrapper->commerce_unit_price->currency_code->value(),
            'data' => array(),
        );

        // Add the express upcharge to the line item unit price.
        $line_item_wrapper->commerce_unit_price->amount = $line_item_wrapper->commerce_unit_price->amount->value() + $to_door_summary;

        // Add the express delivery fee component to the unit price.
        $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
            $line_item_wrapper->commerce_unit_price->value(),
            'dellin_shipping_service_to_door',
            $to_door_price,
            TRUE,
            FALSE
        );
    }

    // сохраняем введенные пользователем данные
    $order_wrapper = entity_metadata_wrapper('commerce_order', $line_item->order_id);
    if ($details_values['to_door']) {
        $order_wrapper->commerce_customer_shipping->field_addr = $details_values['addr'];
    } else {
        // внести в поле адреса расположение терминала
        $order_wrapper->commerce_customer_shipping->field_addr = $data['terminals'][$details_values['terminal']]['address'];
    }
    $order_wrapper->commerce_customer_shipping->field_office = $details_values['office'];
    $order_wrapper->commerce_customer_shipping->field_passport = $details_values['passport'];
    $order_wrapper->commerce_customer_shipping->field_notes = $details_values['notes'];
    $order_wrapper->commerce_customer_shipping->save();
}