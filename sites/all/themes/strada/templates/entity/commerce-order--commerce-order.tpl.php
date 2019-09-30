<?php
// подготовка переменных для вывода
$order_info = _get_order_info($commerce_order);
$shipping = $order_info['shipping'];
$payment = $order_info['payment'];
$user = $order_info['user'];

$items_count = format_plural($order_info['products']['qty'], '1 item', '@count items', array(), array('context' => 'product count on a Commerce order'));

$ordet_title = 'Информация о заказе';
$ordet_num_title = 'Номер заказа';
$payment_label = $payment['balance'] ? 'К оплате' : '<span class="brand-success">Оплачено</span>';
if ($commerce_order->status == 'cart' || strpos($commerce_order->status, 'checkout') !== false) {
    $ordet_title = 'Информация о корзине';
    $ordet_num_title = 'Номер корзины';
    $payment_label = 'Сумма';
}

// оплата
$payment_widget = '';
if ($payment['before_shipping']) {
    if ($payment['balance']) {
        $payment_widget =   '<div class="well">' .
                                '<h4>Ожидание поступления оплаты</h4>' .
                                '<p>Если Вы уже оплатили заказ, информация об оплате поступит к нам в течение нескольких минут.<br />Обновите страницу, чтобы увидеть поступление оплаты.</p>' .
                                '<p>Если оплата была неудачной, Вы можете попробовать ещё раз.</p>' .
                                '<a href="/user/' . $user['uid'] . '/orders/' . $order_info['id'] . '/pay" class="btn btn-primary">Оплатить заказ</a>' .
                            '</div>';
    } else {
        $payment_widget =   '<div class="well">' .
                               '<h4 class="text-success">Заказ оплачен</h4>' .
                            '</div>';
    }
} else {
    $payment_widget =   '<div class="well">' .
                          '<h4 class="text-warning">Оплата при получении</h4>' .
                        '</div>';
}

?>

<div class="order-complete">
    <div class="row">
        <div class="col-xs-12">
            <h3>Список товаров</h3>
            <?php print render($content['commerce_line_items']); ?>
            <?php print render($content['commerce_order_total']); ?>
        </div>
    </div>
    <div class="row">
        <h3><?php print $ordet_title; ?></h3>

        <div class="col-md-offset-1 col-md-10">
            <?php print $payment_widget; ?>
        </div>

        <div class="col-md-offset-1 col-md-6">

            <dl class="dl-horizontal">
                <dt><?php print $ordet_num_title; ?></dt>
                <dd>№ <?php print $order_info['number']; ?><br />
                    <?php if ($commerce_order->status != 'cart'): ?>
                        <p class="text-muted">Для отслеживания в личном кабинете</p></dd>
                    <? endif; ?>

                <?php if (!empty($shipping['callme'])): ?>
                <dt>Комментарий<br />к заказу</dt>
                <dd>
                    <div><strong class="text-danger">Покупатель просит связаться с ним для уточнения деталей.</strong></div>
                </dd>
                <?php endif;?>

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
        <div class="col-md-3 hidden-xs hidden-sm">
            <img src="/sites/default/files/images/checkout/order_done.png" alt="" class="img-responsive pull-right">
        </div>
    </div>
</div>