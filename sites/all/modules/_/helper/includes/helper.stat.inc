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

        $form['container'] = [
            '#type' => 'container',
            '#attributes' => ['class' => ['stat-container']]
        ];
        foreach ($points as $point) {
            if (isset($result[$point])) {
                $form['container']['up_to_' . $point] = [
                    '#markup' => '<div class="stat-box">' .
                                    '<h4>' . ($point != 99999 ? 'до ' . $point : 'от 1000')  . ' руб.</h4>' .
                                    '<div class="stat-content">' .
                                        '<div><div class="number">' . $result[$point]['qty'] . '</div><div class="title">Всего корзин</div></div>' .
                                        '<div><div class="number">' . ceil($result[$point]['total'] / $result[$point]['qty']) . '</div><div class="title">Средняя сумма, руб</div></div>' .
                                    '</div>' .
                                '</div>',
                ];
            }
        }
    }

    return $form;
}

function helper_stat_carts_form_submit($form, &$form_state)
{
    $form_state['rebuild'] = true;
}