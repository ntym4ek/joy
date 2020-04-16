<?php

/**
 * @file
 * Views definitions for mymodule module.
 */

/**
 * Implements hook_views_data_alter().
 */
function ext_user_views_data_alter(&$data)
{
  $data['users']['orders_count'] = array(
    'title' => t('Orders count'),
    'help' => t('User orders count'),
    'field' => array(
      'handler' => 'ext_user_handler_orders_count',
    ),
  );
}
