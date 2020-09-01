<?php
?>

<div class="product teaser <?php print $classes; ?> clearfix ajax-host"
      data-price="<? print $commerce_price[0]["amount"]/100; ?>"
      data-title="<? print $title; ?>"
      data-variant="<? print $volume; ?>">
    <div class="product-card">
        <div class="p-image-block">
            <a href="<?php print $node_url; ?>" class="p-image">
                <img src="<?php print $image_url; ?>" class="img-responsive" loading="lazy" alt="<?php print $title; ?>">
            </a>

            <?php $index = 1; ?>
            <?php if (!empty($is_novelty)): ?>
                <img src="/sites/all/themes/joy/images/icons/novelty.png" class="novelty pos-<?php print $index++; ?>" >
            <? endif; ?>
            <?php if (!empty($has_discount)): ?>
                <img src="/sites/all/themes/joy/images/icons/discount20.png" class="discount pos-<?php print $index++; ?>" >
            <? endif; ?>
            <?php if (!empty($is_popular)): ?>
                <img src="/sites/all/themes/joy/images/icons/popular.png" class="popular pos-<?php print $index++; ?>" >
            <? endif; ?>
            <?php if (!empty($number_discount)): ?>
                <div class="p-discount"><?php print $number_discount; ?>%</div>
            <? endif; ?>

        </div>

        <div class="p-info-block">
            <div class="p-price-block">
                <div class="p-price">
                    <?php print render($content['commerce_price']); ?>
                </div>
                <div class="p-volume">
                    <?php print $volume; ?>
                </div>
            </div>

            <?php if (!empty($fivestar)): ?>
            <div class="p-rating">
                <a href="<?php print $node_url; ?>#scroll-to-block-views-product-reviews-block"><?php print render($fivestar); ?></a>
            </div>
            <? endif; ?>

            <div class="p-title-block">
                <div class="p-title">
                    <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
                </div>
                <div class="p-summary">
                    <?php print $summary; ?>
                </div>
            </div>

            <?php if (!empty($lets_buy)): ?>
            <div class="p-buy-block">
                <?php print $cart_link; ?>
            </div>
            <? endif; ?>

        </div>
    </div>
</div>
