<?php
// подготовка переменных для вывода
$order_info = _get_order_info($order);
$shipping = $order_info['shipping'];
$payment = $order_info['payment'];
$user = $order_info['user'];

$items_count = format_plural($order_info['products']['qty'], '@count item', '@count items', array(), array('context' => 'product count on a Commerce order'));

$ordet_title = 'Информация о заказе';
$ordet_num_title = 'Номер заказа';
$payment_label = $payment['balance'] ? 'К оплате' : '<span class="brand-success">Оплачено</span>';
if ($order_info['status'] == 'cart' || strpos($order_info['status'], 'checkout') !== false) {
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

<div class="row order-complete">
    <div class="col-sm-offset-1 col-sm-10">
        <h2>Спасибо!</h2>
        <h3>Ваш заказ оформлен.</h3>

        <?php print $payment_widget; ?>
    </div>
    <div class="col-sm-offset-1 col-sm-5">

        <dl class="dl-horizontal">
            <dt>Номер заказа</dt>
            <dd>
                <div>№ <?php print $order_info['number']; ?></div>
                <div><p class="text-muted">Для отслеживания в личном кабинете</p></div>
            </dd>

            <dt>Доставка</dt>
            <dd>
                <div><?php print $shipping['title']; ?></div>
                <div><?php print $shipping['address']; ?></div>
            </dd>

            <dt>Получатель</dt>
            <dd>
                <div><?php print $user['name']; ?></div>
                <div><?php print $user['phone']; ?></div>
                <div><?php print $user['mail']; ?></div>
                <div><?php print $shipping['passport']; ?></div>
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
    <div class="col-sm-5">
        <img src="/sites/default/files/images/checkout/order_done.png" alt="" class="img-responsive pull-right">
    </div>
</div>