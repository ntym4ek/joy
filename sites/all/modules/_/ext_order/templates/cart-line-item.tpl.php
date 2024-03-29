<div class="ccf-item<?php print ($product_info['stock'] ? '' : ' out-of-stock'); ?>">
  <div class="ccf-image">
    <img src="<?php print image_style_url('thumbnail', $product_info['image_uri']); ?>" alt="<?php print $product_info['title']; ?>" />
  </div>
  <div class="ccf-info">
    <div class="ccf-product" data-id="<?php print $product_info['id']; ?>">
      <div class="ccf-title"><?php print $product_info['title']; ?></div>
      <div class="ccf-options">
        <?php foreach($options as $option) { print '<div>' . $option . '</div>'; } ?>
      </div>
      <div class="ccf-stock"><?php print ($product_info['stock'] ? '<span class="text-success">В наличии на складе</span>' : '<span class="text-warning">Нет в наличии</span>'); ?></div>
    </div>
    <div class="ccf-parameters">
      <div class="ccf-qty">
        <?php print $qty_formatted; ?>
      </div>
      <div class="ccf-total">
        <?php print $total_formatted; ?>
      </div>
    </div>
  </div>
</div>
