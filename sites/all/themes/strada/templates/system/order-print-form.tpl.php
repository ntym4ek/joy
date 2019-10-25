<?php

$order_info = _get_order_info($order_id, true);

$shipping = $order_info['shipping'];
$payment = $order_info['payment'];
$user = $order_info['user'];
$total = $order_info['total'];

$items_count = format_plural($order_info['products']['qty'], '1 item', '@count items', array(), array('context' => 'product count on a Commerce order'));

$ordet_title = 'Информация о заказе';
$ordet_num_title = 'Номер заказа';
$payment_label = $payment['balance'] ? 'К оплате' : '<span class="brand-success">Оплачено</span>';
if ($order_info['status'] == 'cart' || strpos($order_info['status'], 'checkout') !== false) {
    $ordet_title = 'Информация о корзине';
    $ordet_num_title = 'Номер корзины';
    $payment_label = 'Сумма';
}


// таблица
foreach ($order_info['products']['items'] as $line_item) {
    $commerce_line_items[] = array(
        $line_item['title'],
        drupal_strtolower(implode('; ', $line_item['options'])),
        array('data' => commerce_currency_format($line_item['price'], 'RUB'), 'class' => 'text-right'),
        array('data' => number_format($line_item['qty'], 0), 'class' => 'text-center'),
        array('data' => commerce_currency_format($line_item['total'], 'RUB'), 'class' => 'text-right'),
    );
}


// компоненты итоговой суммы
$total_amount = 0;
foreach($total['components'] as $name => $component) {
    // посчитать заказ без доставки
    if ($name == 'base_price' || strpos($name, 'discount') !== false) {
        $total_amount += $component['amount']; // скидка идёт с минусом и сложение по модулю даёт полную стоимость товара
    }

    $commerce_line_items[] = [['data' => $component['title'], 'class' => 'text-right', 'colspan' => 4], ['data' => commerce_currency_format($component['amount'], 'RUB'), 'class' => 'text-right']];
}
if ($total_amount != $total['components']['base_price']['amount']) $commerce_line_items[] = [['data' => 'Сумма без доставки', 'class' => 'text-right', 'colspan' => 4], ['data' => commerce_currency_format($total_amount, 'RUB'), 'class' => 'text-right']];


?>

<div class="order-complete" style="background: #fff; padding: 0 20px; min-width: 800px; margin: auto; max-width: 800px; ">
    <div class="do-not-print" style="padding-top: 50px;"></div>
    <div class="row">
        <div class="col-md-offset-1 col-md-10">
            <div class="do-not-print"><a href="javascript:window.print()" class="btn btn-info pull-right">Распечатать</a></div>
            <h3>Заказ № <?php print $order_info['number']; ?></h3>
        </div>
    </div>
    <div class="row">
        <div class="col-md-offset-1 col-md-10">
            <h3>Список товаров</h3>
            <?php print theme('table', array('rows' => $commerce_line_items)); ?>
        </div>
    </div>
    <div class="row">
        <div class="col-md-offset-1 col-md-10">
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
                    <div><?php print $items_count; ?></div>
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
    <div class="do-not-print" style="padding-bottom: 100px;"></div>
</div>