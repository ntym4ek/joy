<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
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
$line_item_info = ext_order_get_line_item_info($row->_field_data['commerce_line_item_field_data_commerce_line_items_line_item_']['entity']);
?>

<div class="ccf-item">
    <div class="ccf-image">
        <?php print $fields['field_p_image']->wrapper_prefix . $fields['field_p_image']->label_html . $fields['field_p_image']->content . $fields['field_p_image']->wrapper_suffix; ?>
    </div>
    <div class="ccf-info">
        <div class="ccf-product">
            <div class="ccf-title"><?php print $line_item_info['product_info']['title']; ?></div>
            <div class="ccf-options">
                <?php foreach($line_item_info['options'] as $option) { print '<div>' . $option . '</div>'; } ?>
            </div>
        </div>
        <div class="ccf-price-qty">
            <?php print $fields['commerce_unit_price']->wrapper_prefix . $fields['commerce_unit_price']->label_html . $fields['commerce_unit_price']->content . $fields['commerce_unit_price']->wrapper_suffix; ?>
            <?php print $fields['quantity']->wrapper_prefix . $fields['quantity']->content . ' ' . t('pcs') . $fields['quantity']->wrapper_suffix; ?>
        </div>
    </div>
</div>
