<?php
/**
 * @file
 * Stub file for bootstrap_menu_tree() and suggestion(s).
 */

/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * @param array $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see template_preprocess_menu_tree()
 * @see theme_menu_tree()
 *
 * @ingroup theme_functions
 */

/**
 * Bootstrap theme wrapper function for the primary desctop menu links.
 */
function strada_menu_tree__menu_help(&$variables) {
  return '<div class="menu-help">' . $variables['tree'] . '</div>';
}

/**
 * Bootstrap theme wrapper function for the primary desctop menu links.
 */
function strada_menu_tree__menu_main(&$variables) {
  return '<ul class="nav main-menu">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the user menu links.
 */
function strada_menu_tree__user_menu(&$variables) {
  return '<ul class="nav user-menu">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the user menu links.
 */
function strada_menu_tree__menu_horizont(&$variables) {
  return '<ul class="horizont-menu dry-list">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the navigation menu links.
 */
function strada_menu_tree__navigation(&$variables) {
  return '<ul class="nav navbar-nav navigation">' . $variables['tree'] . '</ul>';
}

