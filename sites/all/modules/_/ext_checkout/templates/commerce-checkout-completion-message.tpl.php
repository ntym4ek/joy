<div class="row order-complete">
    <div class="col-sm-offset-1 col-sm-10">
        <h2>Спасибо!</h2>
        <h3>Ваш заказ оформлен.</h3>

        <? print $payment_widget; ?>
    </div>
    <div class="col-sm-offset-1 col-sm-5">

        <dl class="dl-horizontal">
            <dt>Номер заказа</dt>
            <dd>
                <p>№ <? print $order_info['number']; ?></p>
                <p class="text-muted">Для отслеживания в личном кабинете</p>
            </dd>

            <dt>Доставка</dt>
            <dd>
                <p><? print $shipping['title']; ?></p>
                <p><? print $shipping['address']; ?></p>
            </dd>

            <dt>Получатель</dt>
            <dd>
                <p><? print $user['name']; ?></p>
                <p><? print $user['phone_formatted']; ?></p>
                <p><? print $user['mail']; ?></p>
                <p><? print $shipping['passport']; ?></p>
            </dd>

            <dt>Состав заказа</dt>
            <dd>
                <p><? print $order_info['products']['qty_txt']; ?></p>
                <p><? if (is_array($order_info['weight'])): ?>Вес: <? print $order_info['weight']['weight'] . ' ' . t($order_info['weight']['unit']); endif; ?></p>
            </dd>

          <? if ($coupons): ?>
            <dt>Купоны</dt>
            <dd>
              <? foreach($coupons as $coupon):  ?>
                <p><? print $coupon['code']; ?></p>
              <? endforeach;  ?>
            </dd>
          <? endif; ?>

            <dt><? print $payment_label; ?></dt>
            <dd class="oc-total" data-id="<? print $order_info['number']; ?>" data-total="<? print $order_info['total']['amount']/100; ?>" data-paid="<? print ($payment['balance'] ? 'false' : 'true'); ?>"
                data-coupon="<? print $coupon['code']; ?>" data-shipping="<? print isset($shipping['cost']) ? $shipping['cost'] : ''; ?>">
                <p><? print '<span>' . ($payment['balance'] ? $payment['balance_formatted'] : $order_info['total']['formatted']) . '</span>'; ?></p>
                <p><? print isset($shipping['cost']) ? ('С учётом ' . $shipping['cost_formatted'] . ' за доставку') : ''; ?></p>
                <p><? print $payment['title']; ?></p>
                <p><? print $payment['addon']; ?></p>
            </dd>
        </dl>
    </div>
    <div class="col-sm-5">
        <img src="/sites/default/files/images/checkout/order_done.png" alt="" class="img-responsive pull-right">
    </div>
</div>
