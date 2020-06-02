<?php
/**
 * @file
 * Stub file for "page" theme hook [pre]process functions.
 */

/**
 * Pre-processes variables for the "node" theme hook.
 *
 * See template for list of available variables.
 *
 * @see node.tpl.php
 *
 * @ingroup theme_preprocess
 */
function strada_preprocess_node(&$vars)
{
    // - возможность создания отдельных шаблонов для разных view mode
    // - http://xandeadx.ru/blog/drupal/576 ---------
    $node_type_suggestion_key = array_search('node__' . $vars['type'], $vars['theme_hook_suggestions']);
    if ($node_type_suggestion_key !== FALSE) {
        $node_view_mode_suggestion = 'node__' . $vars['type'] . '__' . $vars['view_mode'];
        array_splice($vars['theme_hook_suggestions'], $node_type_suggestion_key + 1, 0, array($node_view_mode_suggestion));
    }

    // - готовим переменные для вывода Товара ---------
    if ($vars['type'] == 'product') {

        // количество покупок
        $pids = [];
        $vars['bought'] = 0;
        foreach($vars['node']->field_product as $product) {
            $pids[] = $product[0]['product_id'];
        }
        if ($pids) {
            $query = db_select('commerce_line_item', 'cli');
            $query->innerJoin('commerce_order', 'co', 'co.order_id = cli.order_id');
            $query->condition('co.status', 'completed');
            $query->innerJoin('commerce_product', 'cp', 'cp.sku = cli.line_item_label');
            $query->condition('cp.product_id', $pids, 'IN');
            $query->addExpression('COUNT(*)');
            $bought = $query->execute()->fetchField();

            if ($bought > 5) {
                $vars['bought'] = format_plural($bought, '@count customer', '@count customers');
            }
        }
    }

    // - готовим переменные для вывода Отзыва ---------
    if ($vars['type'] == 'review') {
        $vars['user_name'] = 'Аноним';
        $vars['user_sign'] = 'Пользователь не представился';

        if ($vars['node']->uid) {
          $user_wr = entity_metadata_wrapper('user', $vars['node']->uid);
          $username = $user_wr->field_name_first->value() . ' ' . $user_wr->field_surname->value();
          $vars['user_name'] = $username ? $username : $vars['user']->name;
          $vars['user_sign'] = 'Покупатель';

          // проверка на бейдж Реальный покупатель
          if ($node_view_mode_suggestion == 'node__review__teaser' && isset($vars["field_pr_product"]["und"][0]["target_id"])) {
            $product_node_wr = entity_metadata_wrapper('node', $vars["field_pr_product"]["und"][0]["target_id"]);
            $pids = [];
            foreach ($product_node_wr->field_product->raw() as $product) {
              $pids[] = $product;
            }

            $query = db_select('commerce_line_item', 'cli');
            $query->innerJoin('commerce_order', 'co', 'co.order_id = cli.order_id');
            $query->condition('co.status', 'completed');
            $query->condition('co.uid', $user_wr->getIdentifier());
            $query->innerJoin('commerce_product', 'cp', 'cp.sku = cli.line_item_label');
            $query->condition('cp.product_id', $pids, 'IN');
            $query->addExpression('COUNT(*)');
            $is_real = (bool)$query->execute()->fetchField();
            if ($is_real) {
              $vars['user_sign'] = '<span>Реальный покупатель</span>';
            }
          }
        }

        // фото
        $vars['user_image'] = '/sites/all/themes/strada/images/default_user_image.png';
        $user_image_url = '';
        if ($user_image_url) {
            $vars['user_image'] = image_style_url('thumb', $user_image_url);
        }

        // дата
        $vars['date'] = date('d.m.Y', $vars['node']->created);
    }

}

/**
 * Processes variables for the "node" theme hook.
 *
 * See template for list of available variables.
 *
 * @see node.tpl.php
 *
 * @ingroup theme_process
 */
function strada_process_node(&$vars) {

}
