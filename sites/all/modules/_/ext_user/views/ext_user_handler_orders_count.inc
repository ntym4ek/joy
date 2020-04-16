<?php

/**
 * @file
 * Definition of mymodule_handler_handlername.
 */

/**
 * Description of what my handler does.
 */
class ext_user_handler_orders_count extends views_handler_field
{
  function query() {
    $this->ensure_my_table();
  }

  /**
   * Renders the field handler.
   */
  function render($values) {
    $uid = $values->uid;

    $cart_items_count = 0;
    if ($cart = commerce_cart_order_load($uid)) {
      $order_wr = entity_metadata_wrapper('commerce_order', $cart);
      $cart_items_count = commerce_line_items_quantity($order_wr->commerce_line_items, commerce_product_line_item_types());
    }

    $orders_count = db_select('commerce_order', 'co')
      ->condition('co.uid', $uid)
      ->condition('co.status', ['pending', 'completed', 'processing'], 'IN')
      ->countQuery()
      ->execute()
      ->fetchField();

    return 'Товаров в корзине: ' . $cart_items_count . '<br />Заказов: ' . $orders_count;
  }
}
