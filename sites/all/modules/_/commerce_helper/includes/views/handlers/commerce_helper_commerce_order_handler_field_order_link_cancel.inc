<?php

/**
 * Field handler to present a link to cancel an order.
 */
class commerce_helper_commerce_order_handler_field_order_link_cancel extends commerce_order_handler_field_order_link {
  function construct() {
    parent::construct();

    $this->additional_fields['uid'] = 'uid';
  }

  function render($values) {
    if ($values->commerce_order_status != 'pending') return;

      // Ensure the user has access to delete this order.
    $order = commerce_order_new();
    $order->order_id = $this->get_value($values, 'order_id');
    $order->uid = $this->get_value($values, 'uid');

    if (!commerce_order_access('delete', $order)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Cancel order');

    return l($text, 'user/' . $GLOBALS['user']->uid . '/orders/' . $order->order_id . '/cancel', ['query' => drupal_get_destination(), 'attributes' => ['class' => ['btn', 'btn-action-link', 'btn-wide']]]);
  }
}
