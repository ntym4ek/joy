<?php

/**
 * Description of what my handler does.
 */
class add_marketing_handler_field_order_marketing extends views_handler_field
{
  function init(&$view, &$options) {
    parent::init($view, $options);

    $this->additional_fields['data'] = 'data';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Renders the field handler.
   */
  function render($values) {
    $data = unserialize($values->{$this->aliases["data"]});
    $marketing = empty($data['marketing']) ? [] : $data['marketing'];

    return add_marketing_get_utm_markup($marketing);
  }
}
