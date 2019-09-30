<?php

/**
 * @file
 * Default implementation of a line item summary template.
 *
 * Available variables:
 * - $quantity_raw: The number of items in the cart.
 * - $quantity_label: The quantity appropriate label to use for the number of
 *   items in the shopping cart; "item" or "items" by default.
 * - $quantity: A single string containing the number and label.
 * - $total_raw: The raw numeric value of the total value of items in the cart.
 * - $total_label: A text label for the total value; "Total:" by default.
 * - $total: The currency formatted total value of items in the cart.
 * - $links: A rendered links array with cart and checkout links.
 *
 * Helper variables:
 * - $view: The View the line item summary is attached to.
 *
 * @see template_preprocess()
 * @see template_process()
 */
?>
<?php if ($quantity_raw): ?>
<div class="line-item-summary">
    <div class="line-item-wrapper">
        <h4>Предварительный итог</h4>
            <div class="line-item-quantity">
                <span class="label">Количество</span> <span class="raw"><?php print $quantity_raw; ?></span>
            </div>
            <div class="line-item-shipping">
                <span class="label">Доставка</span> <span class="raw">не выбрана</span>
            </div>
        <?php if ($total): ?>
            <div class="line-item-total">
                <span class="label">Итого</span> <span class="raw"><?php print $total; ?></span>
            </div>
        <?php endif; ?>
        <?php print $links; ?>
    </div>
</div>
<?php endif; ?>
