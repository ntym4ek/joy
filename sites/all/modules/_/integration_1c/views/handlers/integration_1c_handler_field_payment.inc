<?php

/**
 * Description of what my handler does.
 */
class integration_1c_handler_field_payment extends views_handler_field
{
    function init(&$view, &$options) {
        parent::init($view, $options);

        $this->additional_fields['order_id'] = 'order_id';
    }

    function query() {
        $this->ensure_my_table();
        $this->add_additional_fields();
    }

    function render($values) {
        $order_id = $this->get_value($values, 'order_id');

        $xml = '';
        if (!empty($order_id) && $order = commerce_order_load($order_id)) {
            $order_wr = entity_metadata_wrapper('commerce_order', $order);

            if (!empty($order_wr->value()->data['payment_method'])) {
                $payment_method = $order_wr->value()->data['payment_method'];
                switch ($payment_method) {
                    case 'ya_commerce_api|commerce_payment_ya_commerce_api':
                        $payment_title = 'Оплата онлайн';
                        break;

                    case 'pay_on_delivery|commerce_payment_pay_on_delivery':
                        $payment_title = 'Оплата при получении';
                        break;

                    case 'bank_transfer|commerce_payment_bank_transfer':
                        $payment_title = 'Банковский перевод';
                        break;
                }

                // проверка оплаты
                commerce_helper_ya_commerce_api_check_order_paid($order_wr->value());
                if ($balance = commerce_payment_order_balance($order_wr->value())) {
                    $balance = $balance['amount'];
                }
                $paid = $order_wr->commerce_order_total->amount->value() - $balance;
            }

            $xml .=     '<method>' . $payment_title . '</method>';
            $xml .=     '<paid>' . $paid/100 . '</paid>';
        }

        return $xml;
    }
}