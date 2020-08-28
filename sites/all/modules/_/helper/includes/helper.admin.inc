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
  $form['vertical_tabs'] = array('#type' => 'vertical_tabs');

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => 'Общие настройки',
      '#group' => 'vertical_tabs',
    );

    $form['main']['support_phone'] = array(
        '#type' => 'textfield',
        '#title' => 'Телефон',
        '#default_value' => variable_get('support_phone', '8-800-550-28-85'),
        '#required' => TRUE,
    );
    $form['main']['manager_phone'] = array(
        '#type' => 'textfield',
        '#title' => 'Телефон',
        '#default_value' => variable_get('manager_phone', '8-912-701-70-74'),
        '#required' => TRUE,
    );
    $form['main']['manager_whatsapp'] = array(
        '#type' => 'textfield',
        '#title' => 'WhatsApp',
        '#default_value' => variable_get('manager_whatsapp', '8-922-910-99-40'),
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
        '#group' => 'vertical_tabs',
    );

    $terms = taxonomy_get_tree(taxonomy_vocabulary_machine_name_load('menu')->vid);
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
      '#group' => 'vertical_tabs',
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
    $form['line2x_1_banner'] = array(
        '#type' => 'fieldset',
        '#title' => 'Первая линия баннеров',
      '#group' => 'vertical_tabs',
    );
    $form['line2x_1_banner']['line2x_1_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('line2x_1_banner_status', 0),
    );
  $form['line2x_1_banner']['line2x_1_left_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('line2x_1_left_banner_title', ''),
    '#required' => true,
    '#states' => array(
      'visible' => array(
        'input[name="line2x_1_banner_status"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
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
  $form['line2x_1_banner']['line2x_1_right_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('line2x_1_right_banner_title', ''),
    '#required' => true,
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
      '#group' => 'vertical_tabs',
    );
    $form['line2x_2_banner']['line2x_2_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('line2x_2_banner_status', 0),
    );
  $form['line2x_2_banner']['line2x_2_left_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('line2x_2_left_banner_title', ''),
    '#required' => true,
    '#states' => array(
      'visible' => array(
        'input[name="line2x_2_banner_status"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
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
  $form['line2x_2_banner']['line2x_2_right_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('line2x_2_right_banner_title', ''),
    '#required' => true,
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
      '#group' => 'vertical_tabs',
    );
    $form['line2x_3_banner']['line2x_3_banner_status'] = array(
        '#type' => 'checkbox',
        '#title' => 'Включить',
        '#default_value' => variable_get('line2x_3_banner_status', 0),
    );
  $form['line2x_3_banner']['line2x_3_left_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('line2x_3_left_banner_title', ''),
    '#required' => true,
    '#states' => array(
      'visible' => array(
        'input[name="line2x_3_banner_status"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
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
  $form['line2x_3_banner']['line2x_3_right_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('line2x_3_right_banner_title', ''),
    '#required' => true,
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

  /** -------------------------------------------------------------------------------------------------------- */
  $form['side_banner'] = array(
    '#type' => 'fieldset',
    '#title' => 'Боковой баннер в каталоге',
    '#group' => 'vertical_tabs',
  );
  $form['side_banner']['side_banner_status'] = array(
    '#type' => 'checkbox',
    '#title' => 'Включить',
    '#default_value' => variable_get('side_banner_status', 0),
  );
  $form['side_banner']['side_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('side_banner_title', ''),
    '#required' => true
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

  $form['related'] = array(
    '#type' => 'fieldset',
    '#title' => 'Сопутствующие товары',
    '#group' => 'vertical_tabs',
  );

  // Get the count of products of the type
  $products = db_query("SELECT product_id, title FROM {commerce_product}")->fetchAllKeyed();
  $form['related']['related_items'] = array(
    '#type' => 'select',
    '#title' => 'Список',
    '#options' => $products,
    '#default_value' => variable_get('related_items', []),
    '#description' => 'Товары для вывода в блоках сопутствующих товаров на страницах описания товара и в корзине',
    '#multiple' => true,
    '#chosen' => true,
  );

  /** -------------------------------------------------------------------------------------------------------- */
  $form['side_banner'] = array(
    '#type' => 'fieldset',
    '#title' => 'Боковой баннер в каталоге',
    '#group' => 'vertical_tabs',
  );
  $form['side_banner']['side_banner_status'] = array(
    '#type' => 'checkbox',
    '#title' => 'Включить',
    '#default_value' => variable_get('side_banner_status', 0),
  );
  $form['side_banner']['side_banner_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Заголовок баннера',
    '#default_value' => variable_get('side_banner_title', ''),
    '#required' => true
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

  $form['related'] = array(
    '#type' => 'fieldset',
    '#title' => 'Сопутствующие товары',
    '#group' => 'vertical_tabs',
  );

  // Get the count of products of the type
  $products = db_query("SELECT product_id, title FROM {commerce_product}")->fetchAllKeyed();
  $form['related']['related_items'] = array(
    '#type' => 'select',
    '#title' => 'Список',
    '#options' => $products,
    '#default_value' => variable_get('related_items', []),
    '#description' => 'Товары для вывода в блоках сопутствующих товаров на страницах описания товара и в корзине',
    '#multiple' => true,
    '#chosen' => true,
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
