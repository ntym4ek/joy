<?php

/**
 * Description of what my handler does.
 */
class integration_1c_handler_field_line_items_xml extends views_handler_field
{
    function init(&$view, &$options) {
        parent::init($view, $options);

        $this->additional_fields['order_id'] = 'order_id';
    }

    function query() {
        $this->ensure_my_table();
        $this->add_additional_fields();
    }

    function render($values) {
        $order_id = $this->get_value($values, 'order_id');

        $xml = 'Товаров не найдено.';
        if (!empty($order_id) && $order = commerce_order_load($order_id)) {
            $xml = '';
            $order_wr = entity_metadata_wrapper('commerce_order', $order);
            foreach($order_wr->commerce_line_items as $line_item_wr) {
                switch ($line_item_wr->type->value()) {
                    case 'product':
                        $xml .= '<product>';
                        $xml .=     '<price>';
                        foreach ($line_item_wr->commerce_unit_price->value()['data']['components'] as $component) {
                            $name = str_replace('|', '_', $component['name']);
                            $xml .= $component['included'] ? '<' . $name . '>' . $component['price']['amount']/100 . '</' . $name . '>' : '';
                        }
                        $xml .=     '</price>';

                        $xml .=     '<qty>' . $line_item_wr->quantity->value() . '</qty>';
                        $xml .=     '<product_id>' . $line_item_wr->commerce_product->product_id->value() . '</product_id>';
                        $xml .= '</product>';
                        break;

                    case 'shipping':
                        $xml .= '<shipping>';
                        $xml .=     '<price>';
                        foreach ($line_item_wr->commerce_unit_price->value()['data']['components'] as $component) {
                            $name = str_replace('|', '_', $component['name']);
                            $xml .= $component['included'] ? '<' . $name . '>' . $component['price']['amount']/100 . '</' . $name . '>' : '';
                        }
                        $xml .=     '</price>';

                        $xml .=     '<method>' . $line_item_wr->commerce_shipping_service->value() . '</method>';
                        $xml .= '</shipping>';

                        break;
                }
            }
        }

        return $xml;
    }
}