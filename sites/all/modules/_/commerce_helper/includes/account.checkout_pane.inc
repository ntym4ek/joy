<?php


/**
 * Checkout pane callback: returns the contents View for inclusion in the checkout form.
 */
function account_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
    global $user;

    $user_wrapper = entity_metadata_wrapper('user', $user);

    $pane_form = array(
        '#title' => 'Доставка и оплата',
        '#title_display' => 'none',
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('checkout-account')),
        '#prefix' => '<div id="checkout-account-ajax-wrapper">',
        '#suffix' => '</div>',
    );

    // Note we're not using theme_username() to avoid linking out of checkout.
    $pane_form['title'] = array(
        '#markup' => '<div class="form-item form-item-label"><h5>Получатель</h5></div>',
    );
    $pane_form['access'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('field-access-group')),
    );
    $pane_form['access']['mail'] = array(
        '#type' => 'textfield',
        '#default_value' => empty($form_state['values']['mail']) ? check_plain($order->mail) : $form_state['values']['mail'],
        '#disabled' => $user->uid > 0 ? true : false,
        '#attributes' => array('placeholder' => array('E-mail адрес')),
    );
    if (!empty($form_state['pass_demand'])) {
        $pane_form['access']['pass'] = array(
            '#type' => 'password',
            '#default_value' => '',
            '#attributes' => array('placeholder' => array('Пароль')),
        );
    }

    $phone = $user_wrapper->field_phone->value();
    $phone = empty($order->field_phone) ? $phone : $order->field_phone['und'][0]['value'];
    $pane_form['phone'] = array(
        '#type' => 'textfield',
        '#default_value' => empty($form_state['values']['phone']) ? check_plain($phone) : $form_state['values']['phone'],
        '#attributes' => array('placeholder' => array('Телефон')),
    );

    // имя пользователя
    $surname        = empty($user_wrapper->field_surname) ? '' : $user_wrapper->field_surname->value();
    $name_first     = empty($user_wrapper->field_name_first) ? '' : $user_wrapper->field_name_first->value();
    $name_second    = empty($user_wrapper->field_name_second) ? '' : $user_wrapper->field_name_second->value();

    $pane_form['surname'] = array(
        '#type' => 'textfield',
        '#default_value' => empty($form_state['values']['surname']) ? check_plain($surname) : $form_state['values']['surname'],
        '#attributes' => array('placeholder' => array('Фамилия')),
    );
    $pane_form['name_first'] = array(
        '#type' => 'textfield',
        '#default_value' => empty($form_state['values']['name_first']) ? check_plain($name_first) : $form_state['values']['name_first'],
        '#attributes' => array('placeholder' => array('Имя')),
    );
    $pane_form['name_second'] = array(
        '#type' => 'textfield',
        '#default_value' => empty($form_state['values']['name_second']) ? check_plain($name_second) : $form_state['values']['name_second'],
        '#attributes' => array('placeholder' => array('Отчество')),
    );

    return $pane_form;
}


/**
 * Account pane: validation callback.
 */
function account_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order)
{
    global $user;

    $fields = array();

    if (!empty($form_state['values'][$checkout_pane['pane_id']])) {
        $pane_values = $form_state['values'][$checkout_pane['pane_id']];

        // если введен email
        if ($user->uid == 0) {
            if (!empty($pane_values['access']['mail'])) {
                // проверить валидность email
                if ($error = user_validate_mail($pane_values['access']['mail'])) {
                    form_set_error($checkout_pane['pane_id'] . '][access][mail', $error);
                    $fields[] = 'mail';
                }

                // проверить, существует ли такой пользователь в бд
                $account = db_select('users', 'u')
                    ->fields('u', array('uid', 'name'))
                    ->condition('mail', $pane_values['access']['mail'])
                    ->range(0, 1)
                    ->execute()
                    ->fetchObject();

                // если существует
                if ($account && $account->uid) {
                    // если пароль ещё не затребован - вывести сообщение и поле для пароля
                    if (empty($form_state['pass_demand'])) {
    //                    $link = l('Войдите с паролем', '/ajax_register/login/nojs', array(
    //                        'query' => array(
    //                            'email=' . $pane_values['access']['mail'],
    //                            'redirect=/checkout',
    //                        ),
    //                        'attributes' => array('class' => array('ctools-use-modal', 'ctools-modal-ctools-ajax-register-style', 'default-throbber')),
    //                    ));
    //                    form_set_error($checkout_pane['pane_id'] . '][access][mail', $link . ' или укажите другой e-mail, чтобы оформить заказ');
                        form_set_error($checkout_pane['pane_id'] . '][access][pass', 'Введите пароль или укажите другой e-mail, чтобы оформить заказ');
                        $fields[] = 'mail';

                        // при следующей проверке требовать пароль или проверять, введен ли новый email
                        $form_state['mail_demand'] = $pane_values['access']['mail'];
                        $form_state['pass_demand'] = true;
                    }
                    // если пароль уже задан, попытаться авторизовать
                    else {
                        if ($uid = user_authenticate($account->name, $pane_values['access']['pass'])) {
                            $user = user_load($uid);
                            user_login_finalize();
                        } else {
                            form_set_error($checkout_pane['pane_id'] . '][access][pass', 'Неверный пароль для указанного e-mail.');
                            $fields[] = 'pass';
                        }
                    }
                }
            } else {
                $fields[] = 'access][mail';
            }
        }

        if (empty($pane_values['phone']))           { $fields[] = 'phone'; }
        if (empty($pane_values['surname']))         { $fields[] = 'surname'; }
        if (empty($pane_values['name_first']))      { $fields[] = 'name_first'; }
        if (empty($pane_values['name_second']))     { $fields[] = 'name_second'; }
    }

    if ($fields) {
        $message = 'Заполните поля информации о пользователе, выделенные красным.';
        foreach($fields as $field) {
            if ($field != 'mail' && $field != 'pass') {
                form_set_error($checkout_pane['pane_id'] . '][' . $field, $message);
                $message = '';
            }
        }
        return false;
    } else {
        return true;
    }
}

/**
 * Checkout pane callback: submit callback.
 */
function account_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
    global $user;

    if (!empty($form_state['values'][$checkout_pane['pane_id']])) {
        $pane_values = $form_state['values'][$checkout_pane['pane_id']];

        // для анонима создать новый аккаунт
        // источник http://sanchiz.net/blog/register-and-user-authentication
        if ($user->uid == 0) {

            // массив данных для создания пользователя
            $arguments = array(
                'name' => $pane_values['access']['mail'],
                'pass' => user_password(), // генерация пароля
                'mail' => $pane_values['access']['mail'],
                'init' => $pane_values['access']['mail'],
                'status' => 1,
                'roles' => array(DRUPAL_AUTHENTICATED_RID => TRUE),
            );

            // сохранить ФИО
            $arguments['field_surname'][LANGUAGE_NONE][0]['value']      = trim($pane_values['surname']);
            $arguments['field_name_first'][LANGUAGE_NONE][0]['value']   = trim($pane_values['name_first']);
            $arguments['field_name_second'][LANGUAGE_NONE][0]['value']  = trim($pane_values['name_second']);

            // телефон
            if (!empty($pane_values['phone'])) $arguments['field_phone'][LANGUAGE_NONE][0]['value'] = ext_user_normalize_phone($pane_values['phone']);

            // создание пользователя
            user_save(NULL, $arguments);
            // аутентификация пользователя
            $uid = ext_user_user_authenticate($arguments['mail'], $arguments['pass']);
            $user = user_load($uid);
            user_login_finalize();
            // нужно для rpt - Registration Password Token, в противном случае токен будет пустой
            $user->password = $arguments['pass'];
            // генерация события для отправки email
            _user_mail_notify('register_no_approval_required', $user);
//            if ($user->uid) {
//                drupal_set_message('Успешная регистрация. Пароль выслан вам на Email.');
//            }
        } else {
            // проверить наличие данных в ЛК и внести недостаающие
            $user_wrapper = entity_metadata_wrapper('user', $user);
            if (empty($user_wrapper->field_phone->value()))     $user_wrapper->field_phone = trim($pane_values['phone']);
            if (empty($user_wrapper->field_surname->value()))   $user_wrapper->field_surname = trim($pane_values['surname']);
            if (empty($user_wrapper->field_name_first->value())) $user_wrapper->field_name_first = trim($pane_values['name_first']);
            if (empty($user_wrapper->field_name_second->value())) $user_wrapper->field_name_second = trim($pane_values['name_second']);
            $user_wrapper->save();
        }


        $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
        $order_wrapper->mail = $pane_values['access']['mail'];
        $order_wrapper->field_phone = $pane_values['phone'];
        $order_wrapper->field_username = $pane_values['surname'] . ' ' . $pane_values['name_first'] . ' ' . $pane_values['name_second'];
        $order_wrapper->save();
    }
}
