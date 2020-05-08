<?php

$order_info = ext_order_get_order_info($order_id, true);

$shipping = $order_info['shipping'];
$payment = $order_info['payment'];
$user = $order_info['user'];
$total = $order_info['total'];

$ordet_title = 'Информация о заказе';
$ordet_num_title = 'Номер заказа';
$payment_label = $payment['balance'] ? 'К оплате' : '<span class="brand-success">Оплачено</span>';
if ($order_info['status'] == 'cart' || strpos($order_info['status'], 'checkout') !== false) {
    $ordet_title = 'Информация о корзине';
    $ordet_num_title = 'Номер корзины';
    $payment_label = 'Сумма';
}


// таблица
foreach ($order_info['products']['items'] as $line_item_info) {
    $commerce_line_items[] = array(
        $line_item_info['product_info']['title'],
        drupal_strtolower(implode('; ', $line_item_info['options'])),
        array('data' => commerce_currency_format($line_item_info['product_info']['price'], 'RUB'), 'class' => 'text-right'),
        array('data' => number_format($line_item_info['qty'], 0), 'class' => 'text-center'),
        array('data' => commerce_currency_format($line_item_info['total'], 'RUB'), 'class' => 'text-right'),
    );
}


// компоненты итоговой суммы
$total_amount = 0;
$shipping_row = '';
foreach($total['components'] as $name => $component) {
    // посчитать заказ без доставки
    if ($name !== 'shipping') {
        $total_amount += $component['amount'];
        $commerce_line_items[] = [['data' => $component['title'], 'class' => 'text-right', 'colspan' => 4], ['data' => commerce_currency_format($component['amount'], 'RUB'), 'class' => 'text-right']];
    } else {
        $shipping_row = [['data' => $component['title'], 'class' => 'text-right', 'colspan' => 4], ['data' => commerce_currency_format($component['amount'], 'RUB'), 'class' => 'text-right']];
    }
}
// сумма со скидкой
if ($total_amount != $total['components']['base_price']['amount']) $commerce_line_items[] = [['data' => 'Сумма со скидкой', 'class' => 'text-right', 'colspan' => 4], ['data' => commerce_currency_format($total_amount, 'RUB'), 'class' => 'text-right']];
// стоимость доставки вывести последней
if ($shipping_row) $commerce_line_items[] = $shipping_row;


?>

<div class="order-complete">
    <div class="row">
        <div class="col-md-12">
            <div class="do-not-print"><a href="javascript:window.print()" class="btn btn-info pull-right">Распечатать</a></div>
            <h3>Заказ № <?php print $order_info['number']; ?></h3>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <h3>Список товаров</h3>
            <?php print theme('table', array('rows' => $commerce_line_items)); ?>
        </div>
    </div>
    <div class="row">
        <div class="col-md-12">
            <h3>Информация о заказе</h3>

            <?php if ($shipping['callme']) print '<h5 class="text-danger">Пользователь просит связаться с ним</h5>'; ?>

            <dl class="dl-horizontal">
                <dt>Доставка</dt>
                <dd>
                    <div><?php print empty($shipping['title']) ? '' : $shipping['title']; ?></div>
                    <div><?php print empty($shipping['address']) ? '' : $shipping['address']; ?></div>
                    <div><?php print empty($shipping['point_id']) ? '' : ('<span class="text-muted">id пункта: ' . $shipping['point_id'] . '</span>'); ?></div>
                </dd>

                <dt>Получатель</dt>
                <dd>
                    <div><?php print $user['name']; ?></div>
                    <div><?php print $user['phone']; ?></div>
                    <div><?php print $user['mail']; ?></div>
                    <div><?php print empty($shipping['passport']) ? '' : $shipping['passport']; ?></div>
                </dd>

                <dt>Состав заказа</dt>
                <dd>
                    <div><?php print $order_info['products']['qty_txt']; ?></div>
                    <div><?php if (is_array($order_info['weight'])): ?>Вес: <?php print $order_info['weight']['weight'] . ' ' . t($order_info['weight']['unit']); endif; ?><div>
                </dd>

                <dt><?php print $payment_label; ?></dt>
                <dd class="oc-total">
                    <div><?php print '<span>' . ($payment['balance'] ? $payment['balance_formatted'] : $order_info['total_formatted']) . '</span>'; ?></div>
                    <div><?php print empty($shipping['cost']) ? '' : ('С учётом ' . $shipping['cost'] . ' за доставку'); ?></div>
                    <div><?php print $payment['title']; ?></div>
                    <div><?php print $payment['addon']; ?></div>
                </dd>
            </dl>
        </div>
    </div>
</div>
