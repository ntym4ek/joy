<?php

/**
 * Implements hook_views_data_alter().
 */
function integration_1c_views_data_alter(&$data)
{
    $data['commerce_order']['field_line_items_xml']['field'] = array(
        'title' => t('Line Items List in XML'),
        'help' => t('Custom handler.'),
        'handler' => 'integration_1c_handler_field_line_items_xml',
    );
    $data['commerce_order']['field_payment']['field'] = array(
        'title' => t('Payment Info in XML'),
        'help' => t('Custom handler.'),
        'handler' => 'integration_1c_handler_field_payment',
    );
}