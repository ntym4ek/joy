<?php

/**
 * Description of what my handler does.
 */
class add_marketing_handler_marketing extends views_handler_field
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
    $output = '';
    $data = unserialize($values->users_data);
    $marketing = empty($data['marketing']) ? [] : $data['marketing'];

    if (!empty($marketing['utm'])) {
      foreach($marketing["utm"] as $utm) {
        if ($utm['utm_source'] == 'facebook') {
          $output .= '<i class="fab fa-facebook-square"></i> ' . $utm['utm_medium'] . ' ' . $utm['utm_campaign'] . ' ' . format_date((int)($utm['timestamp']/1000), 'custom', 'd.m.Y');
        }
      }
    }
    return $output;
  }
}