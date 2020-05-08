<?php
// подготовка переменных для вывода
$order_info = ext_order_get_order_info($order);
$shipping = $order_info['shipping'];
$coupons = $order_info['coupons'];
$payment = $order_info['payment'];
$user = $order_info['user'];

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
        if ($payment['is_online']) {
            $payment_widget =   '<div class="well">' .
                                    '<h4>Ожидание поступления оплаты</h4>' .
                                    '<p>Если Вы уже оплатили заказ онлайн, информация об оплате поступит к нам в течение нескольких минут.<br />Обновите страницу, чтобы увидеть поступление оплаты.</p>' .
                                    '<p>Если оплата была неудачной, Вы можете попробовать ещё раз.</p>' .
                                    '<a href="/user/' . $user['uid'] . '/orders/' . $order_info['id'] . '/pay" class="btn btn-primary">Оплатить онлайн</a>' .
                                '</div>';
        } else {
            $payment_widget =   '<div class="well">' .
                                    '<h4 class="text-warning">Ожидает оплаты</h4>' .
                                '</div>';
        }
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
                <p>№ <?php print $order_info['number']; ?></p>
                <p class="text-muted">Для отслеживания в личном кабинете</p>
            </dd>

            <dt>Доставка</dt>
            <dd>
                <p><?php print $shipping['title']; ?></p>
                <p><?php print $shipping['address']; ?></p>
            </dd>

            <dt>Получатель</dt>
            <dd>
                <p><?php print $user['name']; ?></p>
                <p><?php print $user['phone']; ?></p>
                <p><?php print $user['mail']; ?></p>
                <p><?php print $shipping['passport']; ?></p>
            </dd>

            <dt>Состав заказа</dt>
            <dd>
                <p><?php print $order_info['products']['qty_txt']; ?></p>
                <p><?php if (is_array($order_info['weight'])): ?>Вес: <?php print $order_info['weight']['weight'] . ' ' . t($order_info['weight']['unit']); endif; ?></p>
            </dd>

          <? if ($coupons): ?>
            <dt>Купоны</dt>
            <dd>
              <? foreach($coupons as $coupon):  ?>
                <p><?php print $coupon['code']; ?></p>
              <? endforeach;  ?>
            </dd>
          <? endif; ?>

            <dt><?php print $payment_label; ?></dt>
            <dd class="oc-total">
                <p><?php print '<span>' . ($payment['balance'] ? $payment['balance_formatted'] : $order_info['total_formatted']) . '</span>'; ?></p>
                <p><?php print empty($shipping['cost']) ? '' : ('С учётом ' . $shipping['cost'] . ' за доставку'); ?></p>
                <p><?php print $payment['title']; ?></p>
                <p><?php print $payment['addon']; ?></p>
            </dd>
        </dl>
    </div>
    <div class="col-sm-5">
        <img src="/sites/default/files/images/checkout/order_done.png" alt="" class="img-responsive pull-right">
    </div>
</div>
