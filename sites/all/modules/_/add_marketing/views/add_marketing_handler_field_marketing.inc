<?php

/**
 * Description of what my handler does.
 */
class add_marketing_handler_field_marketing extends views_handler_field
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
    $output_arr = [];
    $data = unserialize($values->{$this->aliases["data"]});
    $marketing = empty($data['marketing']) ? [] : $data['marketing'];

    if (!empty($marketing['utm'])) {
      foreach($marketing["utm"] as $utm) {
        $line = drupal_strtoupper($utm['utm_source']);
        switch(strtolower($utm['utm_source'])) {
          case 'facebook':
            $line = '<i class="fab fa-facebook-square"></i>';
            break;
          case 'instagram':
            $line = '<i class="fab fa-instagram-square"></i>';
            break;
          case 'yandex':
            $line = '<i class="fab fa-yandex"></i>';
            break;
        }
        $line .= !empty($utm['utm_medium']) ? ' ' . $utm['utm_medium'] : '';
        $line .= !empty($utm['utm_campaign']) ? ' ' . $utm['utm_campaign'] : '';
        $line .= !empty($utm['utm_content']) ? ' ' . $utm['utm_content'] : '';
        $line .= !empty($utm['utm_term']) ? ' ' . $utm['utm_term'] : '';
        $line .= !empty($utm['timestamp']) ? ' ' . format_date((int)($utm['timestamp']/1000), 'custom', 'd.m.Y') : '';
        $output_arr [] = $line;
      }
    }
    if ($output_arr) {
      $output = implode('<br />', $output_arr);
    }
    return $output;
  }
}
