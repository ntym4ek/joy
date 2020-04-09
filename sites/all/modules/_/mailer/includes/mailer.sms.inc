<?php

/**
 * form callback: Рассылка SMS
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
    '#type' => 'text_format',
    '#title' => 'Сообщение',
    '#description' => 'Текст SMS',
    '#required' => TRUE,
  );

  $form['actions'] = [
    '#type' => 'container'
  ];
  $form['actions']['test'] = [
    '#type' => 'submit',
    '#value' => 'Сначала тест!',
    '#description' => 'Отправить SMS на номер ' . variable_get('manager_phone', null),
    '#validate' => ['mailer_sms_form_validate'],
    '#submit' => ['mailer_sms_form_test_submit'],
  ];
  $form['actions']['spam'] = [
    '#type' => 'submit',
    '#value' => 'Начать спамить',
    '#description' => 'Отправить письма заданному контингенту',
    '#validate' => ['mailer_sms_form_filter_validate', 'mailer_smsl_form_validate'],
    '#submit' => ['mailer_sms_form_spam_submit'],
  ];

  return $form;
}

/**
 * роутер
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
        $response = json_decode($result, TRUE);
        return $response['success'];
    } else return false;
}
