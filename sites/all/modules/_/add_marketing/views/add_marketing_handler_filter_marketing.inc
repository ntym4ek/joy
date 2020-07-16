<?php

/**
 * Description of what my handler does.
 */
class add_marketing_handler_filter_marketing extends views_handler_filter_string
{
  /**
   * Add this filter to the query.
   *
   * Due to the nature of FAPI, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator and
   * $this->value respectively.
   */
  public function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.data";

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }
}
