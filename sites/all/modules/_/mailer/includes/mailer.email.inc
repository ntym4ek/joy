<?php


/**
 * Implements hook_mail
 */
function mailer_mail($key, &$message, $params)
{
  $order_info = empty($params['order_info']) ? [] : $params['order_info'];

  switch($key) {
    case 'points_add':
      $message['subject'] = 'Вам начислены баллы';

      $message['body'][] = '' .
        '<h2>Здравствуйте!</h2>' .
        '<p>На сайте joy-magazin.ru Вам начислено ' . format_plural($params['userpoints']['points'],'@count point', '@count points') . '.' .
        (empty($params['userpoints']['description']) ? '' : '<br />Комментарий: ' . $params['userpoints']['description']) .
        '</p>' .
        '<p>Оплачивайте баллами до 30% стоимости товаров. </p>';

      break;

    case 'order_creation':
      if (empty($params['mail_to_manager'])) {
        $message['subject'] = 'Подтверждение заказа';

        // при предоплате
        if ($order_info['payment']['before_shipping']) {
          $message['body'][] = '' .
            '<h2>Здравствуйте, ' . $order_info['user']['name'] . '!</h2>' .
            '<p>Спасибо за заказ в нашем магазине JOY.</p>' .
            '<p>Номер Вашего заказа ' . $order_info['id'] . '.<br />' .
            'Заказ будет обработан и выслан после поступления средств на счет.</p>' .
            // для Зарегистрированного вставить ссылку на заказ, для Анонима - на оплату
            ($order_info['user']['uid']
              ? '<p>Просмотреть статус заказа, если Вы уже произвели оплату или оплатить заказ вы можете перейдя по <a href="' . $order_info['url'] . '">ссылке</a>.</p>'
              : '<p>Если Вы уже оплатили заказ, ожидайте уведомления о смене статуса заказа. Если нет, то оплатить можно перейдя по <a href="/checkout/' . $order_info['id'] . '/pay">ссылке</a></p>')  .
            '';
        }
        // при постоплате
        else {
          // для самовывоза текст другой

          $message['body'][] = '' .
            '<h2>Здравствуйте, ' . $order_info['user']['name'] . '!</h2>' .
            '<p>Спасибо за заказ в нашем магазине JOY.</p>' .
            '<p>Номер Вашего заказа ' . $order_info['id'] . '.<br />' .
            ($order_info['shipping']['method'] == 'pickup_shipping_service'
              ? 'После проверки менеджером заказ будет упакован и готов к выдаче в пункте самовывоза по адресу: Кирово-Чепецк, ул. Производственная, д.6.'
              : 'После проверки менеджером заказ будет упакован и отправлен службой "' . $order_info['shipping']['title'] . '".') . '</p>' .
            '<p>Вы выбрали оплату при получении заказа.<br />' .
            'Сумма к оплате ' . $order_info['total_formatted'] . '.</p>' .
            '';
        }
      } else {
        $order_url_manager = 'https://joy-magazin.ru/admin/commerce/orders/' . $order_info['id'];

        $message['subject'] = 'Новый заказ №' . $order_info['id'];
        $message['body'][] = '' .
          '<h2>И снова здравствуйте!</h2>' .
          '<p>В нашем магазине JOY новый <a href="' . $order_url_manager . '">заказ</a> с номером ' . $order_info['id'] . '.</p>';
      }

      break;

    case 'order_paid':
      if (empty($params['mail_to_manager'])) {
        $message['subject'] = 'Заказ оплачен';

        $message['body'][] = '' .
          '<h2>Здравствуйте, ' . $order_info['user']['name'] . '!</h2>' .
          '<p>По Вашему заказу ' . $order_info['id'] . ' поступила оплата на сумму ' . $order_info['payment']['paid_formatted'] . '.</p>' .
          '<p>В ближайшее время ваш заказ будет упакован и отправлен службой "' . $order_info['shipping']['title'] . '".<br />' .
          'Ожидайте уведомления с трек номером для отслеживания.</p>' .
          '<p>Подробную информацию Вы можете посмотреть на странице заказа, перейдя по <a href="' . $order_info['url'] . '">ссылке</a>.</p>' .
          '';
      } else {
        $order_url_manager = 'https://joy-magazin.ru/admin/commerce/orders/' . $order_info['id'];

        $message['subject'] = 'Заказ №' . $order_info['id'] . ' оплачен';
        $message['body'][] = '' .
          '<h2>И снова здравствуйте!</h2>' .
          '<p>В нашем магазине JOY оплатили <a href="' . $order_url_manager . '">заказ</a> с номером ' . $order_info['id'] . '.</p>';
      }

      break;

    case 'order_sent':
      $message['subject'] = 'Заказ отправлен';
      $message['body'][] = '' .
        '<h2>Здравствуйте, ' . $order_info['user']['name'] . '!</h2>' .
        '<p>Еще раз спасибо за покупку в нашем интернет-магазине JOY!</p>' .
        '<p>Рады сообщить, что Ваш заказ ' . $order_info['id'] . ' от ' . date('d.m.Y', $order_info['placed']) . ' был отправлен.</p>' .
        ($order_info['shipping']['parcel_number'] ? '<p>Трек-номер для отслеживания ' . $order_info['shipping']['parcel_number'] . '.</p>' : '').
        '<p>Содержимое отправления:</p>' .
        (empty($params['table']) ? 'Товары не найдены'
          :  theme('table', array(
            'header' => $params['table']['header'],
            'rows' => $params['table']['rows'],
            'attributes' => array('cellspacing' => 0, 'style' => 'font-size: .9em; border-top: 1px solid #ccc; border-left: 1px solid #ccc; margin: 0 auto;'),
          ))) .
        '<h3 style="color: red;">Обратите внимание!</h3>' .
        '<p>К данному сообщению приложена фотография полностью упакованной посылки.</p>' .
        '<p>По получению посылки, пожалуйста, сравните ее с фотографией. При обнаружении повреждения упаковки, вскройте упаковку и  проверьте сохранность товаров. Если товар поврежден,  не принимайте посылку и не расписывайтесь в ее получении.<br />' .
        'Подайте заявление в адрес руководства подразделения службы доставки. Сделайте фотографии посылки, так чтобы было видно повреждение, и перешлите их нам. Мы, со своей стороны, подадим заявление о нарушении сохранности упаковки посылки.</p>' .
        '<div style="width: 100%; text-align: center;"><img width="400" style="display: inline-block;" src="' . $order_info['shipping']['parcel_photo_url'] . '"/></div>' .
        '<h3 style="color: red;">Важно!</h3>' .
        '<p>Регулярно отслеживайте движение посылки и получите её сразу по прибытию в Ваш населённый пункт. Не ждите уведомления от служб доставки.<br />' .
        'Если посылка не будет получена в течение двух недель, то она будет отправлена обратно. Избегайте лишних хлопот и расходов как с Вашей, так и с нашей стороны.</p>' .
        '<br />';
      break;

    case 'order_changed':
      $message['subject'] = 'Статус заказа изменился';
      $message['body'][] = '' .
        '<h2>Здравствуйте, ' . $order_info['user']['name'] . '!</h2>' .
        '<p>Ваш заказ №' . $order_info['id'] . ' принят в обработку.</p>' .
        '';

      break;

    case 'cart_orphan':
      $message['subject'] = 'Вы что-то забыли в корзине!';
      $message['body'][] = '' .
        '<h2>Здравствуйте!</h2>' .
        '<p>Совсем недавно в магазине Joy Вы положили товары в корзину, но так и не оформили заказ.<br />' .
        'Мы сохранили их для Вас.</p>' .
        '<p>В любое время Вы сможете продолжить покупки или завершить оформление заказа.<br />' .
        //                (variable_get('free_shipping_sum', 0) ? 'А при покупке от ' . variable_get('free_shipping_sum') . ' рублей мы доставим ваш заказ бесплатно.' : '') . '</p>' .
        '<p>Если у Вас возникли какие-то вопросы при оформлении заказа или нужна консультация по товару, позвоните по бесплатному номеру 8-800-550-28-85 и мы обязательно поможем.</p>' .
        '<br />' .
        '<div style="text-align: center;"><a href="' . $GLOBALS['base_root'] . '" style="display: inline-block; background: #0a6526; border-radius: 4px; color: #fff; padding: 8px 15px; text-decoration: none; ">Продолжить покупки</a></div>';

      break;

    case 'leave_review':
      $message['subject'] = 'Поделитесь Вашими впечатлениями о товаре!';
      $message['body'][] = '' .
        '<h2>Здравствуйте!</h2>' .
        '<p>Благодарим Вас за покупку в магазине Joy!</p>' .
        '<p>Пожалуйста, оцените приобретенные товары или оставьте свой отзыв на нашем сайте.</p>' .
        '<p>Ваш отзыв поможет нашим покупателям в дальнейшем сделать правильный выбор.</p>' .
        (empty($params['table']) ? 'Товары не найдены'
          : theme('table', array(
            'header' => $params['table']['header'],
            'rows' => $params['table']['rows'],
            'attributes' => array('cellspacing' => 0, 'style' => 'font-size: .9em; border-top: 1px solid #ccc; border-left: 1px solid #ccc; margin: 0 auto;'),
          ))) .
        '<br />';


      break;

    case 'leave_shop_review':
      $message['subject'] = 'Всё ли Вам понравилось?';
      $message['body'][] = '' .
        '<h2>Здравствуйте!</h2>' .
        '<p>Благодарим Вас за покупку в магазине Joy!</p>' .
        '<p>Мы постоянно работаем над улучшением сервиса и Вы можете помочь нам стать лучше!</p>' .
        '<p>Ответьте на несколько вопросов о работе магазина.</p>' .
        '<div style="text-align: center;">' .
        '<a href="https://yandex.ru/poll/enter/CSHFzwCNd6Tze1TBUbX9sz" style="display: inline-block; background: #0a6526; border-radius: 4px; color: #fff; padding: 8px 15px; text-decoration: none; ">Пройти опрос</a>' .
        '</div>';


      break;

    case 'manual':
      $message['subject'] = $params['subject'];

      $body = $params['body'];

      // заменить инфо о картинке на <img src>
      $match = [];
      if (preg_match('!(.*)\[\[(.+)\]\](.*)!s', $params['body'], $match) && !empty($match[2])) {
        $image_url = '';
        if (preg_match('!\{"fid":"(\d+)"!', $match[2], $match2)) {
          if ($file = file_load($match2[1])) {
            $image_url = file_create_url($file->uri);
          }
        }
        if ($image_url) $body = $match[1] . '<img src="' . $image_url . '" style="max-width: 100%;">' . $match[3];
        else $body = $match[1] . 'Ошибка при загрузке изображения' . $match[3];
      }

      $message['body'][] = '' .
        '<h2>Здравствуйте!</h2>' . $body;
      break;
  }
}

/**
 * Implements hook_mail_alter
 */
function mailer_mail_alter(&$message)
{
  if (strpos($_SERVER['HTTP_HOST'], '.local') !== FALSE) {
    $message['send'] = false;
    watchdog('mailer', 'Эмуляция отправки E-Mail.<br />Почта: ' . $message['to'] . '.<br />Тема: ' . $message['subject'] . '<br />Текст: ' . var_dump($message['body']), [], WATCHDOG_DEBUG);
  }
}

/**
 * --------------------------- Ручная рассылка E-Mail ---------------------------------------------------------------------
 * form callback
 */
function mailer_email_form($form, $form_state)
{
  $form['filter'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Фильтр получателей',
    '#options' => [
      1 => 'Без заказов',
      2 => 'С заказами',
      3 => 'Только зарегистрированным за последние 2 недели',
    ],
  );

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Тема письма',
    '#required' => TRUE,
  );

  $form['body'] = array(
    '#type' => 'text_format',
    '#title' => 'Сообщение',
    '#description' => 'Письмо может содержать в тексте HTML разметку со встроенными стилями',
    '#required' => TRUE,
  );

  $form['actions'] = [
    '#type' => 'container'
  ];
  $form['actions']['test'] = [
    '#type' => 'submit',
    '#value' => 'Сначала тест!',
    '#description' => 'Отправить письмо на Электропочту магазина',
    '#validate' => ['mailer_email_form_validate'],
    '#submit' => ['mailer_email_form_test_submit'],
  ];
  $form['actions']['spam'] = [
    '#type' => 'submit',
    '#value' => 'Начать спамить',
    '#description' => 'Отправить письма заданному контингенту',
    '#validate' => ['mailer_email_form_filter_validate', 'mailer_email_form_validate'],
    '#submit' => ['mailer_email_form_spam_submit'],
  ];

  return $form;
}

/**
 * form validate callback.
 */
function mailer_email_form_filter_validate($form, $form_state)
{
  if (!$form_state['input']['filter'][1] && !$form_state['input']['filter'][2]) {
    form_set_error('filter', 'Выберите одну или несколько категорий получателей.' );
  }
}

/**
 * form validate callback.
 */
function mailer_email_form_validate($form, $form_state)
{
  if (empty($form_state['input']['subject'])) {
    form_set_error('subject', 'Тема письма обязательна к заполнению.' );
  }
  if (empty($form_state['input']['body'])) {
    form_set_error('body', 'Текст письма обязателен к заполнению.' );
  }
}

/**
 * form submit callback.
 */
function mailer_email_form_test_submit($form, &$form_state)
{
  $params['subject'] = $form_state['values']['subject'];
  $params['body'] = $form_state['values']['body']['value'];

  // письмо Пользователю
  drupal_mail('mailer', 'manual', variable_get('support_email', 'help@joy-magazin.ru'), 'RU', $params);
  //    drupal_mail('mailer', 'manual', 'mega_comp@mail.ru', 'RU', $params);

  // оставляем значения в форме
  $form_state['rebuild'] = true;
}

/**
 * form submit callback.
 */
function mailer_email_form_spam_submit($form, &$form_state)
{
  // определение пула получателей
  $query = db_select('commerce_order', 'co');
  $query->condition('co.mail', '', '<>');
  $query->condition('co.status', ['paid', 'pending','processing', 'completed'], 'IN');
  $query->fields('co', ['mail']);
  $query->distinct();
  $mails_orders = $query->execute()->fetchCol();

  $mails_users = [];
  if ($form_state['values']['filter'][1]) {
    $query = db_select('users', 'u');
    $query->condition('u.status', 1);
    if ($form_state['values']['filter'][3]) {
      // только для пользователей не старше 2 недель
      $query->condition('u.created', REQUEST_TIME - 1210100, '>');
    }
    $query->fields('u', ['mail']);
    $mails_users = $query->execute()->fetchCol();
  }

  $mails = $mails_orders;
  if ($form_state['values']['filter'][1] && $form_state['values']['filter'][2]) $mails = array_unique(array_merge($mails_users, $mails_orders));
  if ($form_state['values']['filter'][1] && !$form_state['values']['filter'][2]) $mails = array_diff($mails_users, $mails_orders);

  // добавление в очередь отправки
  if ($mails) {
    $params['subject'] = $form_state['values']['subject'];
    $params['body'] = $form_state['values']['body']['value'];

    $queue = DrupalQueue::get('manual_mails_queue');
    foreach($mails as $mail) {
      $params['mail'] = $mail;

      if (in_array($mail, $mails_users)) {
        $params['account'] = user_load_by_mail($mail);
        // всем незарегистрированным или если есть подписка
        if (!$params['account']->uid || mailer_check_subscription(SUBSCRIPTION_PROMO_EMAIL, $params['account']->uid)) {
          $queue->createItem($params);
        }
      }
    }
  }

  drupal_set_message('Письма поставлены в очередь и будут отправлены в ближайшее время');
}


