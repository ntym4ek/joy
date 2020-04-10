<?php

/**
 * --------------------------- Ручная рассылка SMS -----------------------------------------
 * form callback
 */
function mailer_sms_form($form, $form_state)
{
  $form['filter'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Фильтр получателей',
    '#options' => [
      1 => 'Без заказов',
      2 => 'С заказами',
      3 => 'Только зарегистрированным за последние 2 недели',
    ],
  );

  $form['message'] = array(
    '#type' => 'textfield',
    '#title' => 'Сообщение',
    '#description' => 'Текст SMS. Без "JOY-MAGAZIN" в начале.',
    '#required' => TRUE,
  );

  $form['actions'] = [
    '#type' => 'container'
  ];
  $form['actions']['test'] = [
    '#type' => 'submit',
    '#value' => 'Сначала тест на ' . variable_get('manager_phone', null),
    '#validate' => ['mailer_sms_form_validate'],
    '#submit' => ['mailer_sms_form_test_submit'],
  ];
  $form['actions']['spam'] = [
    '#type' => 'submit',
    '#value' => 'Начать спамить',
    '#validate' => ['mailer_sms_form_filter_validate', 'mailer_sms_form_validate'],
    '#submit' => ['mailer_sms_form_spam_submit'],
  ];

  return $form;
}

/**
 * form validate callback.
 */
function mailer_sms_form_filter_validate($form, $form_state)
{
  if (!$form_state['input']['filter'][1] && !$form_state['input']['filter'][2]) {
    form_set_error('filter', 'Выберите одну или несколько категорий получателей.' );
  }
}

/**
 * form validate callback.
 */
function mailer_sms_form_validate($form, $form_state)
{
  if (empty($form_state['input']['message'])) {
    form_set_error('message', 'Текст сообщения обязателен к заполнению.' );
  }
}

/**
 * form submit callback.
 */
function mailer_sms_form_test_submit($form, &$form_state)
{
  $params['message'] = $form_state['values']['message'];
  $params['phone'] = variable_get('manager_phone', null);

  // SMS менеджеру
  $result = mailer_send_sms('manual', $params);
  if ($result['success']) {
    drupal_set_message('SMS отправлено');
  } elseif (!empty($result["error"]["descr"])) {
    drupal_set_message($result["error"]["descr"], 'error');
  } else {
    drupal_set_message('Неизвестная ошибка при отправке', 'error');
  }

  // оставляем значения в форме
  $form_state['rebuild'] = true;
}

/**
 * form submit callback.
 */
function mailer_sms_form_spam_submit($form, &$form_state)
{
  // определение пула получателей
  $query = db_select('field_data_field_phone', 'fp');
  $query->condition('fp.entity_type', 'commerce_order');
  $query->innerJoin('commerce_order', 'co', 'co.order_id = fp.entity_id');
  $query->condition('co.status', ['paid', 'pending','processing', 'completed'], 'IN');
  $query->fields('fp', ['field_phone_value']);
  $query->distinct();
  $phones_w_orders = $query->execute()->fetchCol();

  $phones_users = [];
  if ($form_state['values']['filter'][1]) {
    $query = db_select('field_data_field_phone', 'fp');
    $query->condition('fp.entity_type', 'user');
    $query->innerJoin('users', 'u', 'u.uid = fp.entity_id');
    $query->condition('u.status', 1);
    if ($form_state['values']['filter'][3]) {
      // только для пользователей не старше 2 недель
      $query->condition('u.created', REQUEST_TIME - 1210100, '>');
    }
    $query->fields('fp', ['field_phone_value']);
    $phones_users = $query->execute()->fetchCol();

    // todo проверка подписки
//    $flag = flag_get_flag('phone_marketing');
//    if ($flag && !$flag->is_flagged($params['account']->uid, $params['account']->uid)) {
//      $will_get_email = false;
//    }
  }

  $phones_raw = $phones_w_orders;
  if ($form_state['values']['filter'][1] && $form_state['values']['filter'][2]) $phones_raw = array_unique(array_merge($phones_users, $phones_w_orders));
  if ($form_state['values']['filter'][1] && !$form_state['values']['filter'][2]) $phones_raw = array_diff($phones_users, $phones_w_orders);

  // убрать дубликаты из-за разного написания в ЛК и заказе
  $phones = [];
  foreach ($phones_raw as $item) {
    $item = ext_user_normalize_phone($item);
    $phones[$item] = $item;
  }

  // добавление в очередь отправки
  if ($phones) {
    $params['message'] = $form_state['values']['message'];

    $queue = DrupalQueue::get('manual_sms_queue');
    foreach($phones as $phone) {
      if (ext_user_valid_phone($phone)) {
        $params['phone'] = $phone;
        $queue->createItem($params);
      }
    }
  }

  drupal_set_message('SMS поставлены в очередь и будут отправлены в ближайшее время');
}




/**
 * --------------------------- Подготовка и отправка SMS -----------------------------------------
 */
function mailer_send_sms($event, $params)
{
    $result = false;

    // не отправлять при запуске на dev сервере
    if (strpos($_SERVER['HTTP_HOST'], '.local') !== false) return false;

    if (!variable_get('phone_registration', false)) return false;

    // определить телефон для отправки смс
    $phone = '';
    if (!empty($params['phone'])) {
        $phone = $params['phone'];
    } elseif (!empty($params['account'])) {
        $field_phone = field_get_items('user', $params['account'], 'field_phone');
        if (!empty($field_phone[0]['value'] && ext_user_valid_phone($field_phone[0]['value']))) {
            $phone = $field_phone[0]['value'];
        }
    }

    $phone = ext_user_normalize_phone($phone);
    if (empty($phone)) return false;

    switch ($event) {
        case 'manual':
          $text = 'JOY-MAGAZIN. Код подтверждения: ' . $params['message'];
          $result = mailer_process_send_sms($phone, $text, $event);
          break;

        case 'points_add':
            if (!empty($params['userpoints'])) {
                $pts = format_plural($params['userpoints']['points'],'@count point', '@count points');
                if ($params['userpoints']['points'] > 0) {
                  $text = 'JOY-MAGAZIN. Вам начислено '  . $pts . '. Оплачивайте до ' . variable_get('commerce_userpoints_discount_custom_order_total_cover_max_percent', '100') . '% стоимости.';
                }
                if ($params['userpoints']['points'] < 0) $text = 'JOY-MAGAZIN. Со счёта списано '  . $pts . '. Подробнее ' . variable_get('manager_phone', '');
                $result = mailer_process_send_sms($phone, $text, $event);
            }
            break;

        case 'user_code':
            if (!empty($params['code'])) {
                $text = 'JOY-MAGAZIN. Код подтверждения: ' . $params['code'];
                $result = mailer_process_send_sms($phone, $text, $event);
            }
            break;

        case 'user_new':
            $text = 'JOY-MAGAZIN. Вы успешно зарегистрированы.';
            $result = mailer_process_send_sms($phone, $text, $event);
            break;

        case 'user_pass':
            $text = 'JOY-MAGAZIN. Ваш пароль от кабинета: ' . $params['pass'];
            $result = mailer_process_send_sms($phone, $text, $event);
            break;

        case 'order_creation':
            if (!empty($params['order_info'])) {
                $text = 'JOY-MAGAZIN. Ваш заказ № ' . $params['order_info']['id'] . ' принят в обработку.';
                $result = mailer_process_send_sms($phone, $text, $event);
            }
            break;

        case 'order_paid':
            if (!empty($params['order_info'])) {
                $text = 'JOY-MAGAZIN. Оплата заказа №' . $params['order_info']['id'] . ' прошла успешно.';
                $result = mailer_process_send_sms($phone, $text, $event);
            }
            break;

        case 'order_sent':
            if (!empty($params['order_info'])) {
                $text = 'JOY-MAGAZIN. Заказ № ' . $params['order_info']['id'] . ' отправлен.' . ($params['order_info']['shipping']['parcel_number'] ? ' Трек-номер для отслеживания ' . $params['order_info']['shipping']['parcel_number'] : '');
                $result = mailer_process_send_sms($phone, $text, $event);
            }
            break;
    }

    return $result;
}


/**
 * отправка смс пользователю
 */
function mailer_process_send_sms($phone, $text, $tag = null)
{
    // токен из ЛК Talk-Me
    $api_token = 'ru2zkdh4xnxqto9ixfvactad59zqea9zh0w0w2dddqb4awfzfce185x28dx22pox';

    $request = [
        'messages' => [
            0 => [
                "recipient" => $phone,
                "text" => $text
            ]
        ],
    ];

    if ($tag) $request['tags'][] = $tag;

    $ch = curl_init();
    curl_setopt_array($ch, [
        CURLOPT_URL => "https://lcab.talk-me.ru/json/v1.0/sms/send/text",
        CURLOPT_POST => true,
        CURLOPT_HTTPHEADER => [
            "X-Token: " . $api_token,
            "Content-Type: application/json"
        ],
        CURLOPT_POSTFIELDS => json_encode($request),
        CURLOPT_RETURNTRANSFER => true
    ]);

    if ($result = curl_exec($ch)) {
      return json_decode($result, TRUE);
    } else return false;
}
