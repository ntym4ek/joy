<?php

/**
 * @file
 * Admin UI.
 */

/**
 * Settings.
 */
function helper_settings($form, &$form_state)
{
    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => 'Общие настройки',
    );

    $form['main']['support_phone'] = array(
        '#type' => 'textfield',
        '#title' => 'Телефон',
        '#default_value' => variable_get('support_phone', '8-800-550-28-85'),
        '#required' => TRUE,
    );
    $form['main']['support_email'] = array(
        '#type' => 'textfield',
        '#title' => 'Электропочта',
        '#default_value' => variable_get('support_email', 'help@joy-magazin.ru'),
        '#required' => TRUE,
    );

    $form['front'] = array(
        '#type' => 'fieldset',
        '#title' => 'Главная страница',
    );

    $terms = taxonomy_get_tree(taxonomy_vocabulary_machine_name_load('products_categories')->vid);
    $options = array();
    foreach($terms as $term){
        $options[$term->tid] = str_repeat("-", $term->depth) . ' ' . $term->name;
    }

    $form['front']['carousel_categories'] = array(
        '#type' => 'select',
        '#title' => 'Список категорий в карусели',
        '#options' => $options,
        '#default_value' => variable_get('carousel_categories', ''),
        '#multiple' => true,
        '#chosen' => true,
    );

    $form['front']['carousel_products_1_title'] = array(
        '#type' => 'textfield',
        '#title' => 'Заголовок первой карусели товаров',
        '#default_value' => variable_get('carousel_products_1_title', ''),
    );
    $form['front']['carousel_products_2_title'] = array(
        '#type' => 'textfield',
        '#title' => 'Заголовок второй карусели товаров',
        '#default_value' => variable_get('carousel_products_2_title', ''),
    );

    $form['front_center_banner'] = array(
        '#type' => 'fieldset',
        '#title' => 'Центральный баннер на главной',
    );
    $form['front_center_banner']['front_center_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('front_center_banner_status', 0),
    );
    $form['front_center_banner']['front_center_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Баннер в центре главной',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg, размер 2280 точек по ширине, высота в пределах разумного',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('front_center_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="front_center_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['front_center_banner']['front_center_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Баннер в центре главной - ссылка',
        '#description' => '',
        '#default_value' => variable_get('front_center_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="front_center_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );

    /** -------------------------------------------------------------------------------------------------------- */
    $form['side_banner'] = array(
        '#type' => 'fieldset',
        '#title' => 'Боковой баннер в каталоге',
    );
    $form['side_banner']['side_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('side_banner_status', 0),
    );
    $form['side_banner']['side_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Боковой баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg, размер 260 точек по ширине, высота в пределах разумного',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('side_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="side_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['side_banner']['side_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Боковой баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('side_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="side_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );

    /** -------------------------------------------------------------------------------------------------------- */
    $form['line2x_1_banner'] = array(
        '#type' => 'fieldset',
        '#title' => 'Первая линия баннеров',
    );
    $form['line2x_1_banner']['line2x_1_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('line2x_1_banner_status', 0),
    );
    $form['line2x_1_banner']['line2x_1_left_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Левый баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('line2x_1_left_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_1_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_1_banner']['line2x_1_left_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Левый баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('line2x_1_left_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_1_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_1_banner']['line2x_1_right_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Правый баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('line2x_1_right_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_1_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_1_banner']['line2x_1_right_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Правый баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('line2x_1_right_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_1_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );

    /** -------------------------------------------------------------------------------------------------------- */
    $form['line2x_2_banner'] = array(
        '#type' => 'fieldset',
        '#title' => 'Вторая линия баннеров',
    );
    $form['line2x_2_banner']['line2x_2_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('line2x_2_banner_status', 0),
    );
    $form['line2x_2_banner']['line2x_2_left_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Левый баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('line2x_2_left_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_2_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_2_banner']['line2x_2_left_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Левый баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('line2x_2_left_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_2_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_2_banner']['line2x_2_right_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Правый баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('line2x_2_right_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_2_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_2_banner']['line2x_2_right_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Правый баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('line2x_2_right_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_2_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );

    /** -------------------------------------------------------------------------------------------------------- */
    $form['line2x_3_banner'] = array(
        '#type' => 'fieldset',
        '#title' => 'Третья линия баннеров',
    );
    $form['line2x_3_banner']['line2x_3_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('line2x_3_banner_status', 0),
    );
    $form['line2x_3_banner']['line2x_3_left_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Левый баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('line2x_3_left_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_3_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_3_banner']['line2x_3_left_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Левый баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('line2x_3_left_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_3_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_3_banner']['line2x_3_right_banner_image'] = array(
        '#type' => 'managed_file',
        '#title' => 'Правый баннер',
        '#theme' => 'image_widget',
        '#description' => 'Выберите файл с расширением jpg',
        '#upload_location' => "public://images/banner/",
        '#upload_validators' => array(
            'file_validate_is_image' => array(),
            'file_validate_extensions' => array('jpg'),
        ),
        '#default_value' => variable_get('line2x_3_right_banner_image', 0),
        '#element_validate' => array('file_managed_file_validate'),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_3_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );
    $form['line2x_3_banner']['line2x_3_right_banner_url'] = array(
        '#type' => 'textfield',
        '#title' => 'Правый баннер - ссылка',
        '#description' => '',
        '#default_value' => variable_get('line2x_3_right_banner_url', ''),
        '#states' => array(
            'visible' => array(
                'input[name="line2x_3_banner_status"]' => array(
                    'checked' => TRUE,
                ),
            ),
        ),
    );


    $form['shop'] = array(
        '#type' => 'fieldset',
        '#title' => 'Настройки магазина',
    );

    $form['shop']['free_shipping_sum'] = array(
        '#type' => 'textfield',
        '#title' => 'Бесплатная доставка от суммы',
        '#description' => 'Минимальная сумма заказа для обнуления стоимости доставки. 0 - отключено',
        '#default_value' => variable_get('free_shipping_sum', 0),
        '#required' => TRUE,
    );
    $form['shop']['free_shipping_show_sum'] = array(
        '#type' => 'textfield',
        '#title' => 'Сообщение о бесплатной доставке',
        '#description' => 'Сумма остатка до бесплатной доставки при которой выводить сообщение. 1 - выводить при любом остатке, 0 - не выводить никогда',
        '#default_value' => variable_get('free_shipping_show_sum', 0),
        '#required' => TRUE,
    );

    $form['shop']['free_shipping_weight'] = array(
        '#type' => 'textfield',
        '#title' => 'Бесплатная доставка по весу',
        '#description' => 'Вес в КГ до которого доставка может быть бесплатной. 0 - вес не учитывается',
        '#default_value' => variable_get('free_shipping_weight', 0),
        '#required' => TRUE,
    );

    $form['shop']['courier_delivery_cost'] = array(
        '#type' => 'textfield',
        '#title' => 'Стоимость доставки курьером',
        '#description' => 'Сумма прибавляется к стоимости доставки до ПВЗ. 1 - стоимость будет браться с сервера Boxberry',
        '#default_value' => variable_get('courier_delivery_cost', 150),
        '#required' => TRUE,
    );

    // получить список регионов
    $options = [];
    if ($vid = taxonomy_vocabulary_machine_name_load('regions')->vid) {
        $tree = taxonomy_get_tree($vid);
        foreach ($tree as $term) {
            $options[$term->tid] = $term->name;
        }
    }
    $form['shop']['non_free_shipping_regions'] = array(
        '#type' => 'select',
        '#title' => 'Платная доставка',
        '#chosen' => TRUE,
        '#multiple' => TRUE,
        '#options' => $options,
        '#description' => 'Список регионов, для которых не действует бесплатная доставка',
        '#default_value' => variable_get('non_free_shipping_regions', []),
    );

    $form['#validate'][] = 'helper_settings_images_validate';
    $form['#submit'][] = 'helper_settings_images_submit';

    return system_settings_form($form);
}


/**
 * Form validate callback.
 */
function helper_settings_images_validate($form, &$form_state)
{
    if((empty($form_state['values']['line2x_1_left_banner_image']) && !empty($form_state['values']['line2x_1_right_banner_image']))
        || (!empty($form_state['values']['line2x_1_left_banner_image']) && empty($form_state['values']['line2x_1_right_banner_image']))) {
          form_set_error('line2x_1_banner', 'Должны быть заполнены оба баннера в первой линейке.');
    }
    if((empty($form_state['values']['line2x_2_left_banner_image']) && !empty($form_state['values']['line2x_2_right_banner_image']))
        || (!empty($form_state['values']['line2x_2_left_banner_image']) && empty($form_state['values']['line2x_2_right_banner_image']))) {
          form_set_error('line2x_2_banner', 'Должны быть заполнены оба баннера во второй линейке.');
    }
    if((empty($form_state['values']['line2x_3_left_banner_image']) && !empty($form_state['values']['line2x_3_right_banner_image']))
        || (!empty($form_state['values']['line2x_3_left_banner_image']) && empty($form_state['values']['line2x_3_right_banner_image']))) {
          form_set_error('line2x_3_banner', 'Должны быть заполнены оба баннера в третьей линейке.');
    }
}
/**
 * Form submit callback.
 */
function helper_settings_images_submit($form, &$form_state)
{
    /** - изображение front_center_banner --------------------------- */
    if (!empty($form_state['values']['front_center_banner_image'])) {
        $file = file_load($form_state['values']['front_center_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('front_center_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'front_center_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'front_center_banner_image', 0);
                variable_set('front_center_banner_image', $file->fid);
                $form_state['values']['front_center_banner_image'] = $file->fid;
            }
        }
    }

    /** - изображение side_banner_image --------------------------- */
    if (!empty($form_state['values']['side_banner_image'])) {
        $file = file_load($form_state['values']['side_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('side_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'side_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'side_banner_image', 0);
                variable_set('side_banner_image', $file->fid);
                $form_state['values']['side_banner_image'] = $file->fid;
            }
        }
    }

    /** - изображения первой линии баннеров --------------------------- */
    if (!empty($form_state['values']['line2x_1_left_banner_image']) && !empty($form_state['values']['line2x_1_left_banner_image'])) {
        $file = file_load($form_state['values']['line2x_1_left_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('line2x_1_left_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'line2x_1_left_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'line2x_1_left_banner_image', 0);
                variable_set('line2x_1_left_banner_image', $file->fid);
                $form_state['values']['line2x_1_left_banner_image'] = $file->fid;
            }
        }

        $file = file_load($form_state['values']['line2x_1_right_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('line2x_1_right_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'line2x_1_right_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'line2x_1_right_banner_image', 0);
                variable_set('line2x_1_right_banner_image', $file->fid);
                $form_state['values']['line2x_1_right_banner_image'] = $file->fid;
            }
        }
    }

    /** - изображения второй линии баннеров --------------------------- */
    if (!empty($form_state['values']['line2x_2_left_banner_image']) && !empty($form_state['values']['line2x_2_right_banner_image'])) {
        $file = file_load($form_state['values']['line2x_2_left_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('line2x_2_left_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'line2x_2_left_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'line2x_2_left_banner_image', 0);
                variable_set('line2x_2_left_banner_image', $file->fid);
                $form_state['values']['line2x_2_left_banner_image'] = $file->fid;
            }
        }

        $file = file_load($form_state['values']['line2x_2_right_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('line2x_2_right_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'line2x_2_right_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'line2x_2_right_banner_image', 0);
                variable_set('line2x_2_right_banner_image', $file->fid);
                $form_state['values']['line2x_2_right_banner_image'] = $file->fid;
            }
        }
    }

    /** - изображения третьей линии баннеров --------------------------- */
    if (!empty($form_state['values']['line2x_3_left_banner_image']) && !empty($form_state['values']['line2x_3_right_banner_image'])) {
        $file = file_load($form_state['values']['line2x_3_left_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('line2x_3_left_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'line2x_3_left_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'line2x_3_left_banner_image', 0);
                variable_set('line2x_3_left_banner_image', $file->fid);
                $form_state['values']['line2x_3_left_banner_image'] = $file->fid;
            }
        }

        $file = file_load($form_state['values']['line2x_3_right_banner_image']);

        // удалить старый файл
        $old_fid = variable_get('line2x_3_right_banner_image', 0);
        if ($old_fid != $file->fid) {
            if ($old_file = file_load($old_fid)) {
                file_usage_delete($old_file, 'helper');
                file_delete($old_file);
            }

            // сохранить новый
            if ($file) {
                // статус постоянного файла
                $file->status = FILE_STATUS_PERMANENT;

                // переименование файла
                $filename = 'line2x_3_right_banner.' . pathinfo($file->filename, PATHINFO_EXTENSION);
                $file->filename = $filename;
                $file = file_move($file, 'public://images/banner/' . $filename, FILE_EXISTS_REPLACE);

                file_save($file);
                file_usage_add($file, 'helper', 'line2x_3_right_banner_image', 0);
                variable_set('line2x_3_right_banner_image', $file->fid);
                $form_state['values']['line2x_3_right_banner_image'] = $file->fid;
            }
        }
    }
}