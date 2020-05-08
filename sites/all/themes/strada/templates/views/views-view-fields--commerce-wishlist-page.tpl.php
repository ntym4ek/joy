<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT
 *   output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field.
 *   Do not use var_export to dump this object, as it can't handle the
 *   recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to
 *   use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */

// дополнительная информация о line_item
$line_item_info = ext_order_get_line_item_info($row->_field_data['line_item_id']['entity']);
$product_info = $line_item_info['product_info'];

?>
<div class="product teaser">
  <div class="product-card">

    <div class="p-image-block">
      <?php print $fields['field_p_image']->wrapper_prefix . $fields['field_p_image']->label_html . $fields['field_p_image']->content . $fields['field_p_image']->wrapper_suffix; ?>
    </div>

    <div class="p-info-block">
      <div class="p-price-block">
        <div class="p-price">
          <?php print $product_info['price']; ?>
        </div>
        <div class="p-volume">
          <?php print $product_info['tare']; ?>
        </div>
      </div>

      <?php if (!empty($product_info['fivestar_html'])): ?>
        <div class="p-rating">
          <a href="<?php print $product_info['node_url']; ?>#scroll-to-block-views-product-reviews-block"><?php print $product_info['fivestar_html']; ?></a>
        </div>
      <? endif; ?>

      <div class="p-title-block">
        <div class="p-title">
          <a href="<?php print $product_info['node_url']; ?>"><?php print $product_info['title']; ?></a>
        </div>
        <div class="p-summary">
          <?php print $product_info['summary']; ?>
        </div>
      </div>

      <?php if (!empty($product_info['lets_buy'])): ?>
        <div class="p-buy-block">
          <?php print $product_info['add_to_cart_link']; ?>
          <?php print $fields['wishlist_remove']->wrapper_prefix . $fields['wishlist_remove']->content . $fields['wishlist_remove']->wrapper_suffix; ?>
        </div>
      <? endif; ?>

    </div>
  </div>
</div>
