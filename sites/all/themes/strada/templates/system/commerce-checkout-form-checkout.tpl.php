<div class="ch-form-container">
    <?php print render($form['account']); ?>
    <?php print render($form['customer_profile_shipping']); ?>
    <?php print render($form['commerce_shipping']); ?>
    <?php print render($form['commerce_payment']); ?>
</div>
<div class="ch-info-container">
    <?php print render($form['cart_contents']); ?>
    <?php print render($form['commerce_coupon']); ?>
    <?php print render($form['summary']); ?>
    <?php print render($form['buttons']); ?>
    <?php print render($form['fz152_agreement']); ?>
</div>
<?php echo drupal_render_children($form); ?>