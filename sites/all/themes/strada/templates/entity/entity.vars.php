<?php
/**
 * @file
 * Stub file for "page" theme hook [pre]process functions.
 */

/**
 * Pre-processes variables for the "entity" theme hook.
 *
 * See template for list of available variables.
 *
 * @see node.tpl.php
 *
 * @ingroup theme_preprocess
 */
function strada_preprocess_entity(&$vars)
{
    // - готовим переменные для вывода Product ---------------------------------
  // todo перевести на использование ext_product_get_info()
  if ($vars['entity_type'] == 'commerce_product') {
        $product_id = $vars['elements']['#entity']->product_id;
        $host_node_id = ext_product_get_product_display_by_product_id($product_id);

        // сформировать ссылку на описание
            // параметр id для отображения соответствующего варианта товара в карточке (модуль Commerce Product URLs)
        $url_query['id'] = $product_id;
            // включить доп параметр с категорией, откуда заходим в описание
        $tid = str_replace('taxonomy/term/', '', $_GET['q']);
        if (is_numeric($tid)) {
            $url_query['cat'] = $tid;
        }
        $vars['node_url'] = url('node/'. $host_node_id, ['query' => $url_query]);

        // получить описание
        $pd_summary = db_select('field_data_body', 'b')
            ->fields('b', array('body_summary'))
            ->condition('b.entity_type', 'node')
            ->condition('b.entity_id', $host_node_id)
            ->execute()->fetchField();
        $vars['summary'] = strip_tags($pd_summary);


        // баннеры на карточках
//        $vars['has_discount'] = false;
//        foreach($vars['elements']['#entity']->commerce_price['und'][0]['data']['components'] as $item) {
//            if (strpos($item['name'], 'discount') === 0) $vars['has_discount'] = true;
//        }
//        $vars['is_novelty'] = !empty($vars['elements']['#entity']->field_p_novelty['und'][0]['value']);
//        $vars['is_popular'] = !empty($vars['elements']['#entity']->field_p_popular['und'][0]['value']);

        // разрешение на покупку
        $lets_buy = true;
//        if (drupal_is_front_page()) $lets_buy = false;

        // проверить наличие товара в корзине для вывода значка заполненной корзины
        if ($lets_buy) {
            $vars['cart_link'] = '';
            $product = $vars['elements']['#entity'];
            $in_stock = empty($product->field_p_out_of_stock['und'][0]['value']);
            if ($in_stock) {
                // если товар имеет опции, текст Выбрать размер
                $has_options = false;
                foreach((array)$product as $field_name => $field) {
                    if (strpos($field_name, '_option_') && $field) {
                        $has_options = true;
                    }
                }
                if ($has_options) {
                    $vars['cart_link'] = '<div id="product-' . $product_id . '-button"><a href="' . $vars['node_url'] . '" class="btn btn-brand btn-sm btn-empty">Выбрать размер</a></div>';
                }
                // если уже добавлен "В корзину | +1"
                elseif (in_array($product_id, _get_order_product_ids($GLOBALS['user']->uid))) {
                    $vars['cart_link'] =
                        '<div id="product-' . $product_id . '-button" class="btn-group" role="group" >' .
                            '<a href="/cart"  class="btn btn-brand btn-sm btn-empty" title="Перейти в корзину" rel="nofollow">В корзину</a>' .
                            '<a href="/cart/add-product/' . $product_id . '/nojs" id="product-' . $product_id . '" class="btn btn-brand btn-sm btn-narrow btn-add-to-cart use-ajax" rel="nofollow">+1</a>' .
                        '</div>';
                }
                else {
                    $vars['cart_link'] = '<div id="product-' . $product_id . '-button"><a href="/cart/add-product/' . $product_id . '/nojs" id="product-' . $product_id . '" class="btn btn-brand btn-sm btn-add-to-cart use-ajax" title="Добавить в корзину" rel="nofollow">Купить</a></div>';
                }
            } else {
                $vars['cart_link'] = '<div id="product-' . $product_id . '-button"><a href="' . $vars['node_url'] . '" class="btn btn-brand btn-sm btn-empty">Товар ожидается</a></div>';
            }
        }
        $vars['lets_buy'] = $lets_buy;

        $vars['image_url'] = image_style_url('medium', $vars['content']['field_p_image'][0]['#item']['uri']);
        $arr = explode('|', $vars['title']);
        $title = empty($arr[0]) ? $vars['title'] : trim($arr[0]);
        $volume = empty($arr[1]) ? '' : trim($arr[1]);
        $vars['title'] = trim($title);
        $vars['volume'] = $volume;

        // оценка
        $votes = fivestar_get_votes('node', $host_node_id);
        $vars['fivestar'] = !empty($votes['count']['value']) ? field_view_field('node', node_load($host_node_id), 'field_pr_mark','teaser') : '';

        // числовая скидка
        $discount_amount = 0;
        $base_amount = commerce_price_component_total($vars['commerce_price'][0], 'base_price')['amount'];
        foreach ($vars['commerce_price'][0]['data']['components'] as $key => $component) {
            if (strpos($component['name'], 'discount') !== false) {
                $discount_amount += $component['price']['amount'];
            }
        }
        $vars['number_discount'] = $base_amount ? floor($discount_amount/$base_amount*100) : 0;
    }
}

