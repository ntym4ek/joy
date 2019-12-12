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
function popup_on_leave_settings()
{
    $form = array();
    $form['popup_on_leave_enable'] = array(
        '#type' => 'radios',
        '#title' => t('Enable Popup'),
        '#default_value' => variable_get('popup_on_leave_enable', 1),
        '#options' => array(
            1 => t('Enabled'),
            0 => t('Disabled'),
        ),
    );
    $form['popup_on_leave_fieldset'] = array(
        '#type' => 'fieldset',
        '#title' => t('Popup message settings'),
        '#collapsed' => FALSE,
        '#collapsible' => TRUE,
    );
    $form['popup_on_leave_fieldset']['popup_on_leave_width'] = array(
        '#type' => 'textfield',
        '#title' => t('Window width'),
        '#required' => TRUE,
        '#default_value' => variable_get('popup_on_leave_width', 300),
    );
    $form['popup_on_leave_fieldset']['popup_on_leave_height'] = array(
        '#type' => 'textfield',
        '#title' => t('Window height'),
        '#required' => TRUE,
        '#default_value' => variable_get('popup_on_leave_height', 300),
    );
    $form['popup_on_leave_fieldset']['popup_on_leave_delay'] = array(
        '#type' => 'textfield',
        '#title' => 'Перерыв',
        '#description' => 'Период между показами сообщения',
        '#default_value' => variable_get('popup_on_leave_delay', 0),
    );

    $form['popup_on_leave_fieldset']['visibility']['path'] = array(
        '#type' => 'fieldset',
        '#title' => t('Pages'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#group' => 'visibility',
        '#weight' => 0,
    );
    $options = array(
        POPUP_ON_LEAVE_VISIBILITY_NOTLISTED => t('All pages except those listed'),
        POPUP_ON_LEAVE_VISIBILITY_LISTED => t('Only the listed pages'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
            '%blog' => 'blog',
            '%blog-wildcard' => 'blog/*',
            '%front' => '<front>')
    );
    $access = user_access('use PHP for settings');
    $visibility = variable_get('popup_on_leave_visibility', POPUP_ON_LEAVE_VISIBILITY_NOTLISTED);
    if (isset($visibility) && $visibility == POPUP_ON_LEAVE_VISIBILITY_PHP && !$access) {
        $form['popup_on_leave_fieldset']['visibility']['path']['popup_on_leave_visibility'] = array(
            '#type' => 'value',
            '#value' => POPUP_ON_LEAVE_VISIBILITY_PHP,
        );
        $form['popup_on_leave_fieldset']['visibility']['path']['popup_on_leave_visibility_pages'] = array(
            '#type' => 'value',
            '#value' => variable_get('popup_on_leave_visibility_pages', ''),
        );
    }
    else {
        if (module_exists('php') && $access) {
            $options += array(POPUP_ON_LEAVE_VISIBILITY_PHP => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
            $title = t('Pages or PHP code');
            $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
        }
        else {
            $title = t('Pages');
        }

        $form['popup_on_leave_fieldset']['visibility']['path']['popup_on_leave_visibility'] = array(
            '#type' => 'radios',
            '#title' => t('Show block on specific pages'),
            '#options' => $options,
            '#default_value' => $visibility,
        );

        $form['popup_on_leave_fieldset']['visibility']['path']['popup_on_leave_visibility_pages'] = array(
            '#type' => 'textarea',
            '#default_value' => variable_get('popup_on_leave_visibility_pages', ''),
            '#description' => $description,
            '#title' => '<span class="element-invisible">' . $title . '</span>',
        );
    }
    return system_settings_form($form);
}
