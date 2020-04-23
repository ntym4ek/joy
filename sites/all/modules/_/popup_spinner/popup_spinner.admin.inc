<?php

/**
 * @file
 * Settings.
 */

/**
 * Settings form.
 *
 * @return array
 *   Form array.
 */
function popup_spinner_settings()
{
    $form = array();
    $form['popup_spinner_enable'] = array(
        '#type' => 'radios',
        '#title' => 'Включить для незарегистрированных посетителей',
        '#default_value' => variable_get('popup_spinner_enable', 1),
        '#options' => array(
            1 => t('Enabled'),
            0 => t('Disabled'),
        ),
    );
  $form['popup_spinner_coupon_free_shipping'] = array(
    '#type' => 'textfield',
    '#title' => 'Код купона на бесплатную доставку',
    '#required' => TRUE,
    '#default_value' => variable_get('popup_spinner_coupon_free_shipping', ''),
  );
  $form['popup_spinner_coupon_10'] = array(
    '#type' => 'textfield',
    '#title' => 'Код купона на 10% скидку',
    '#required' => TRUE,
    '#default_value' => variable_get('popup_spinner_coupon_10', ''),
  );
  $form['popup_spinner_test_gift'] = array(
    '#type' => 'textfield',
    '#title' => 'Протестировать сектор',
    '#description' => 'Число от 1 до 6, где 1 - это Биогумус. На рулетке выпадет указанный приз. 0 - включить случайное распределение.',
    '#default_value' => variable_get('popup_spinner_test_gift', 0),
  );
    $form['popup_spinner_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => 'Настройки всплывающего окна',
        '#collapsed' => FALSE,
        '#collapsible' => TRUE,
    );
    $form['popup_spinner_fieldset']['popup_spinner_width'] = array(
        '#type' => 'textfield',
        '#title' => 'Ширина окна',
        '#required' => TRUE,
        '#default_value' => variable_get('popup_spinner_width', 300),
    );
    $form['popup_spinner_fieldset']['popup_spinner_height'] = array(
        '#type' => 'textfield',
        '#title' => 'Высота окна',
        '#required' => TRUE,
        '#default_value' => variable_get('popup_spinner_height', 300),
    );

    $form['popup_spinner_fieldset']['visibility']['path'] = array(
        '#type' => 'fieldset',
        '#title' => 'Страницы',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#group' => 'visibility',
        '#weight' => 0,
    );
    $options = array(
        POPUP_SPINNER_VISIBILITY_NOTLISTED => t('All pages except those listed'),
        POPUP_SPINNER_VISIBILITY_LISTED => t('Only the listed pages'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
            '%blog' => 'blog',
            '%blog-wildcard' => 'blog/*',
            '%front' => '<front>')
    );
    $access = user_access('use PHP for settings');
    $visibility = variable_get('popup_spinner_visibility', POPUP_SPINNER_VISIBILITY_NOTLISTED);
    if (isset($visibility) && $visibility == POPUP_SPINNER_VISIBILITY_PHP && !$access) {
        $form['popup_spinner_fieldset']['visibility']['path']['popup_spinner_visibility'] = array(
            '#type' => 'value',
            '#value' => POPUP_SPINNER_VISIBILITY_PHP,
        );
        $form['popup_spinner_fieldset']['visibility']['path']['popup_spinner_visibility_pages'] = array(
            '#type' => 'value',
            '#value' => variable_get('popup_spinner_visibility_pages', ''),
        );
    }
    else {
        if (module_exists('php') && $access) {
            $options += array(POPUP_SPINNER_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
            $title = t('Pages or PHP code');
            $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
        }
        else {
            $title = t('Pages');
        }

        $form['popup_spinner_fieldset']['visibility']['path']['popup_spinner_visibility'] = array(
            '#type' => 'radios',
            '#title' => t('Show block on specific pages'),
            '#options' => $options,
            '#default_value' => $visibility,
        );

        $form['popup_spinner_fieldset']['visibility']['path']['popup_spinner_visibility_pages'] = array(
            '#type' => 'textarea',
            '#default_value' => variable_get('popup_spinner_visibility_pages', ''),
            '#description' => $description,
            '#title' => '<span class="element-invisible">' . $title . '</span>',
        );
    }
    return system_settings_form($form);
}
