<?php

/**
 * форма статистики по корзинам
 */
function helper_stat_carts_form($form, $form_state)
{
    $points = [100, 500, 1000, 99999];

    $form = [
        '#attributes' => ['class' => ['stat-carts-form']],
    ];

    // фильтр по датам
    $date_to = empty($form_state['input']['date_to']['date']) ? REQUEST_TIME : strtotime($form_state['input']['date_to']['date']);
    $date_from = empty($form_state['input']['date_from']['date']) ? REQUEST_TIME - 2600000 : strtotime($form_state['input']['date_from']['date']);

    $form['filter'] = [
        '#type' => 'container',
        '#attributes' => ['class' => ['stat-filter']],
    ];

    $form['filter']['date_from'] = [
        '#type' => 'date_popup',
        '#title' => 'За период',
        '#date_format' => 'd.m.Y',
        '#default_value' => date("Y-m-d", $date_from),
    ];
    $form['filter']['date_to'] = [
        '#type' => 'date_popup',
        '#title' => '&nbsp;',
        '#date_format' => 'd.m.Y',
        '#default_value' => date("Y-m-d", $date_to),
    ];

    $form['filter']['actions'] = [
        '#type' => 'container',
        '#attributes' => ['class' => ['filter-actions']],
    ];
    $form['filter']['actions']['apply'] = [
        '#type' => 'submit',
        '#value' => 'Применить',
        '#submit' => ['helper_stat_carts_form_submit'],
    ];

    // делаем запрос за период по показателю 1
    $query = db_select('commerce_order', 'co');
    $query->condition('co.created', $date_to, '<');
    $query->condition('co.created', $date_from, '>');
    $query->condition('co.status', ['cart', 'checkout_checkout'], 'IN');
    $query->fields('co', ['order_id']);
    if ($results = $query->execute()->fetchCol()) {

        $carts = commerce_order_load_multiple($results, []);
        foreach($carts as $cart){
            foreach ($points as $point) {
                if ($cart->commerce_order_total['und'][0]['amount']/100 <= $point) {
                    $result[$point]['total'] = (empty($result[$point]['total']) ? 0 : $result[$point]['total']) +  $cart->commerce_order_total['und'][0]['amount']/100;
                    $result[$point]['qty'] = (empty($result[$point]['qty']) ? 0 : $result[$point]['qty']) + 1;
                    continue 2;
                }
            }
        }

      $carts_html = '';
      foreach ($points as $point) {
        if (isset($result[$point])) {
          $carts_html .=  '<div class="stat-box">' .
                            '<div class="carts">' .
                              '<div class="number">' .  $result[$point]['qty'] . '</div>' .
                              '<div class="title">Корзин</div>' .
                            '</div>' .
                            '<div class="foot">' .
                              '<div><div class="number">' . ($point != 99999 ? '<div class="title">до</div><span>' . $point . '</span>': '<div class="title">от</div><span>1000</span>') . ' руб.</div></div>' .
                              '<div><div class="number"><div class="title">cредняя сумма</div><span>' . ceil($result[$point]['total'] / $result[$point]['qty']) . '</span> руб.</div></div>' .
                            '</div>' .
                          '</div>';
        }
      }
      $form['container'] = [
        '#type' => 'container',
      ];
      $form['container']['carts'] = [
        '#markup' => '<h3>Распределение корзин по суммам</h3><div class="stat-container">' . $carts_html . '</div>',
      ];

    }

  // отчёт Воронка продаж
  $query = db_select('commerce_order', 'co');
  $query->condition('co.created', $date_to, '<');
  $query->condition('co.created', $date_from, '>');
  $query->condition('co.status', 'cart');
  $carts_count = $query->countQuery()->execute()->fetchField();

  $query = db_select('commerce_order', 'co');
  $query->condition('co.created', $date_to, '<');
  $query->condition('co.created', $date_from, '>');
  $query->condition('co.status', 'checkout_checkout');
  $checkout_count = $query->countQuery()->execute()->fetchField();

  $query = db_select('commerce_order', 'co');
  $query->condition('co.created', $date_to, '<');
  $query->condition('co.created', $date_from, '>');
  $query->condition('co.status', ['cart', 'checkout_checkout', 'wishlist'], 'NOT IN');
  $orders_count = $query->countQuery()->execute()->fetchField();

  // вывести результаты
  $form['container2'] = [
    '#type' => 'container',
  ];
  $form['container2']['funnel'] = [
    '#markup' => '<h3>Воронка продаж</h3>' .
      '<div class="sales-container">' .
        '<div class="sales-box box-1">' .
            '<div class="number">' . ($carts_count + $checkout_count + $orders_count) . '</div>' .
            '<div class="title">Корзина &nbsp;&nbsp;100 %</div>' .
            '<div class="icon"><i class="fal fa-shopping-cart"></i></div>' .
        '</div>' .
        '<div class="sales-box box-2">' .
            '<div class="number">' . ($checkout_count + $orders_count) . '</div>' .
            '<div class="title">Оформление &nbsp;&nbsp;' . number_format(($checkout_count + $orders_count)*100/($carts_count + $checkout_count + $orders_count), 0) . ' %</div>' .
            '<div class="icon"><i class="fal fa-credit-card-blank"></i></div>' .
        '</div>' .
        '<div class="sales-box box-3">' .
            '<div class="number">' . $orders_count . '</div>' .
            '<div class="title">Заказ &nbsp;&nbsp;' . number_format($orders_count*100/($carts_count + $checkout_count + $orders_count), 1) . ' %</div>' .
            '<div class="icon"><i class="fal fa-check"></i></div>' .
        '</div>' .
      '</div>'
  ];


  // отчёт По регионам
  // todo подготовить
  $regions = [];
  // список оформленных заказов
  $query = db_select('commerce_order', 'co');
  $query->condition('co.created', $date_to, '<');
  $query->condition('co.created', $date_from, '>');
  $query->condition('co.status', ['cart', 'checkout_checkout', 'wishlist'], 'NOT IN');
  $query->fields('co', ['order_id']);
  if ($results = $query->execute()->fetchCol()) {

    // список регионов
    $vid = taxonomy_vocabulary_machine_name_load('regions')->vid;
    foreach(taxonomy_get_tree($vid, 0, 1, true) as $term) {
      $regions[$term->field_kladr['und'][0]['value']] = [
        'name' => $term->name,
      ];
    }

    // считаем количество каждого региона в заказах
    foreach ($results as $order_id) {
      $order_wr = entity_metadata_wrapper('commerce_order', $order_id);
      if (!empty($order_wr->value()->commerce_customer_shipping)) {
        $field_data = json_decode($order_wr->commerce_customer_shipping->field_data->value(), TRUE);
        if (!empty($field_data["region"]['data']["region_kladr_id"])) {
          $kladr = $field_data["region"]['data']["region_kladr_id"];
          $regions[$kladr]['counter'] = empty($regions[$kladr]['counter']) ? 1 : ++$regions[$kladr]['counter'];
        }
      }
    }
  }

  // фильтр пустых и сортировка по количеству
  foreach ($regions as $key => $region) {
    if (empty($region['counter'])) {
      unset($regions[$key]);
    }
  }

  // Начиная с PHP 5.5.0 вы можете использовать array_column() вместо вышеуказанного кода
  $counter  = array_column($regions, 'counter');

  // Сортируем данные по volume по убыванию и по edition по возрастанию
  // Добавляем $data в качестве последнего параметра, для сортировки по общему ключу
  array_multisort($counter, SORT_DESC, $regions);

  $rows = [];
  foreach($regions as $region) {
    $rows[] = ['data' => $region];
  }


  // вывести
  $form['container3'] = [
    '#type' => 'container',
  ];
  $form['container3']['regions'] = [
    '#markup' => '<h3>Продажи по регионам</h3>' .
                  theme('table', ['header' => ['Регион', 'Заказов'], 'rows' => $rows, 'attributes' => []])
  ];


  return $form;
}

function helper_stat_carts_form_submit($form, &$form_state)
{
    $form_state['rebuild'] = true;
}
