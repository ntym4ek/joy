<?php

/**
 * @file
 * Stub file for "page" theme hook [pre]process functions.
 */

/**
 * Pre-processes variables for the "page" theme hook.
 *
 * See template for list of available variables.
 *
 * @param array $vars
 *   An associative array of variables, passed by reference.
 *
 * @see page.tpl.php
 *
 * @ingroup theme_preprocess
 */
function strada_preprocess_page(array &$vars)
{
    $vars['navbar_classes_array'] = array('navbar', 'container-fluid', 'navbar-strada');
//    $vars['navbar_classes_array'] = array('navbar', 'container-fluid', 'navbar-alter');

    if (drupal_is_front_page()) {
        drupal_set_title('');
        unset($vars['page']['content']['system_main']['default_message']);
    }

    /** -------------------------------------------- Меню  ---------------------------------------------------------- */
    // menu menu main
    $menu = menu_tree_all_data('main-menu');
    $main_menu = menu_tree_output($menu);
    $vars['main_menu'] = $main_menu;

    // menu menu main
    foreach($main_menu as &$menu_item) {
        if (isset($menu_item['#theme'])) {
            $menu_item['#theme'] = 'menu_link__main_menu_mobile';
        }
    }
    $main_menu['#theme_wrappers'] = ['menu_tree__main_menu_mobile'];
    $vars['main_menu_mobile'] = $main_menu;

//    // menu menu
//    $menu = menu_tree_all_data('menu-main');
//    $vars['main_menu'] = menu_tree_output($menu);

    // user menu
    $menu = menu_tree_all_data('user-menu');
    $vars['user_menu'] = menu_tree_output($menu);
    $vars['user_menu']['#theme_wrappers'] = array('menu_tree__user_menu');

    // user menu
//    $menu = menu_tree_all_data('menu-horizont');
//    $vars['horizont_menu'] = menu_tree_output($menu);

    // поиск
    $vars['search_value'] = isset($_GET['s']) ? check_plain($_GET['s']) : '';

    /** -------------------------------------------- Таксономия ----------------------------------------------------- */
    if (isset($vars['page']['content']['system_main']['term_heading']['term'])
        &&($vars['page']['content']['system_main']['term_heading']['term']['#bundle'] == 'help_categoties')) {
        $vars['content_title'] = drupal_get_title();
        drupal_set_title('Помощь');
        // если есть статьи в разделе
        if (!empty($vars['page']['content']['system_main']['nodes'])) {
            $vars['page']['content']['system_main']['term_heading']['#suffix'] .= '<div class="taxonomy-read-next">См. разделы:</div>';
        }
    }

    /** -------------------------------------------- Ноды  ---------------------------------------------------------- */
    if (isset($vars['node'])) {
        if ($vars['node']->type == 'product') {
            drupal_set_title('');
        }

        if (explode('/', url($_GET['q']))[1] == 'info') {
            $vars['content_title'] = drupal_get_title();
            drupal_set_title('Помощь');
        }
    }

    /** -------------------------------------------- Корзина  ------------------------------------------------------- */
    // убрать все раздражители со страницы
    $vars['tsss'] = false;
    if (arg(0) == 'checkout' && arg(2) != 'complete') {
        $vars['tsss'] = true;
    }

    /** -------------------------------------------- Подключение ---------------------------------------------------- */
    // библиотека кукис
    drupal_add_library('system', 'jquery.cookie');

    // библиотека DaData для определения местоположения
    drupal_add_js('https://cdn.jsdelivr.net/npm/suggestions-jquery@latest/dist/js/jquery.suggestions.min.js', array('type' => 'external'));
//    drupal_add_js('https://unpkg.com/suggestions-jquery@latest/dist/js/jquery.suggestions.js', array('type' => 'external'));
    drupal_add_css('https://cdn.jsdelivr.net/npm/suggestions-jquery@latest/dist/css/suggestions.css', array('type' => 'external'));


    $vars['content_column_class'] = ['col-sm-12'];
    if (!empty($vars['page']['sidebar_first']) && (!empty($vars['page']['sidebar_second']) || !empty($vars['action_links']))) {
        $vars['content_column_class'] = ['col-sm-6'];
    }
    elseif (!empty($vars['page']['sidebar_first']) || !empty($vars['page']['sidebar_second']) || !empty($vars['action_links'])) {
        $vars['content_column_class'] = ['col-sm-9'];
    }
    if (!empty($vars['page']['sidebar_first'])) {
        $vars['content_column_class'][] = 'with-sidebar-first';
    }
}

/**
 * Processes variables for the "page" theme hook.
 *
 * See template for list of available variables.
 *
 * @param array $variables
 *   An associative array of variables, passed by reference.
 *
 * @see page.tpl.php
 *
 * @ingroup theme_process
 */
function strada_process_page(array &$vars) {

}
