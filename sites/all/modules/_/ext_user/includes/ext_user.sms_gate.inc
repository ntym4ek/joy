<?php


/**
 * роутер
 */
function ext_user_sms_gate_sms($event, $params)
{
    $result = false;

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

    if (empty($phone)) return false;

    switch ($event) {
        case 'user_code':
            if (!empty($params['code'])) {
                $text = 'JOY-MAGAZIN. Код подтверждения: ' . $params['code'];
                $result = ext_user_sms_gate_process_send_sms($phone, $text, $event);
            }
            break;

        case 'user_new':
            $text = 'JOY-MAGAZIN. Вы успешно зарегистрированы.';
            $result = ext_user_sms_gate_process_send_sms($phone, $text, $event);
            break;

        case 'order_creation':
            if (!empty($params['order_info'])) {
                $text = 'JOY-MAGAZIN. Ваш заказ № ' . $params['order_info']['id'] . ' принят в обработку.';
                $result = ext_user_sms_gate_process_send_sms($phone, $text, $event);
            }
            break;

        case 'order_paid':
            if (!empty($params['order_info'])) {
                $text = 'JOY-MAGAZIN. Оплата заказа №' . $params['order_info']['id'] . ' прошла успешно.';
                $result = ext_user_sms_gate_process_send_sms($phone, $text, $event);
            }
            break;

        case 'order_sent':
            if (!empty($params['order_info'])) {
                $text = 'JOY-MAGAZIN. Заказ № ' . $params['order_info']['id'] . ' отправлен.' . ($params['order_info']['shipping']['parcel_number'] ? ' Трек-номер для отслеживания ' . $params['order_info']['shipping']['parcel_number'] : '');
                $result = ext_user_sms_gate_process_send_sms($phone, $text, $event);
            }
            break;
    }

    return $result;
}


/**
 * отправка смс пользователю
 */
function ext_user_sms_gate_process_send_sms($phone, $text, $tag = null)
{
    return true;
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