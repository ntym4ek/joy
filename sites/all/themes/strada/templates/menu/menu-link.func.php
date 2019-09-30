<?php
/**
 * @file
 * Stub file for bootstrap_menu_link() and suggestion(s).
 */

/**
 * Returns HTML for a menu link and submenu.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see theme_menu_link()
 *
 * @ingroup theme_functions
 */
function strada_menu_link__menu_main(array $variables)
{
    $element = $variables['element'];
    $sub_menu = '';

    $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();
    $depth = $element['#original_link']['depth'];

    // Check plain title if "html" is not set, otherwise, filter for XSS attacks.
    $mlid = $element['#original_link']['mlid'];
    $title = empty($options['html']) ? check_plain($element['#title']) : filter_xss_admin($element['#title']);

    if ($mlid == 3714) $title = '<i class="far fa-bars" aria-hidden="true"></i><span>' . $title . '</span>';
    if ($mlid == 3682) $title = '<span class="text-success">' . $title . '</span>';
    if ($mlid == 3713) $title = '<span class="text-danger">' . $title . '</span>';

    // Ensure "html" is now enabled so l() doesn't double encode. This is now
    // safe to do since both check_plain() and filter_xss_admin() encode HTML
    // entities. See: https://www.drupal.org/node/2854978
    $options['html'] = TRUE;

    $href = $element['#href'];
    $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();
        if ($element['#original_link']['module'] == 'taxonomy_menu') {
            $source_tid = str_replace('taxonomy/term/', '', $element['#original_link']['link_path']);
            if ($source_term_wr = entity_metadata_wrapper('taxonomy_term', $source_tid)) {

                $href = $source_term_wr->field_link->value();
            }
        }

    if (!empty($element['#below'])) {
        unset($element['#below']['#theme_wrappers']);
        $sub_menu = (($depth == 1) ? '<div class="dropdown-menu">' : '') .
                        '<div class="level-' . ($depth + 1) . '-wrapper">' .
                            ($depth + 1 == 3 ? '<span class="back-button"><i class="far fa-chevron-left"></i></span>' : '') .  // для меню третьего уровня добавить кнопку закрытия в моб. виде
                            '<ul class="level-' . ($depth + 1) . '">' .
                            drupal_render($element['#below']) .
                            ($depth + 1 == 3 ? '<li class="show-all"><a href="' . url($href) . '">Показать все</a></li>' : '') .
                            '</ul>' .
                        '</div>' .
                    (($depth == 1) ? '</div>' : '');

        if ($depth == 1) {
            $attributes['class'][] = 'dropdown';
            $attributes['id'] = 'dropdown';
            $options['attributes']['class'][] = 'dropdown-toggle btn btn-brand';
            $options['attributes']['data-toggle'] = 'dropdown';
        }
    }
    $attributes['class'][] = 'level-' . $depth . '-item';
//    if ($mlid == 3683) $attributes['class'][] = 'visible';

    return '<li' . drupal_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>";
}

function strada_menu_link__user_menu(array $variables)
{
  $element = $variables['element'];
  $mlid = $element['#original_link']['mlid'];

  // не выводить кнопку входа или личного кабинета в зависимости от наличия авторизации
  if ($GLOBALS['user']->uid) {
    if ($mlid == 3661)   return '';
  } else {
    if ($mlid == 2) return '';
  }

  // Check plain title if "html" is not set, otherwise, filter for XSS attacks.
  $title = empty($options['html']) ? check_plain($element['#title']) : filter_xss_admin($element['#title']);

  $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();
  // Ensure "html" is now enabled so l() doesn't double encode. This is now
  // safe to do since both check_plain() and filter_xss_admin() encode HTML
  // entities. See: https://www.drupal.org/node/2854978
  $options['html'] = TRUE;

  // вставить Иконки
  if (in_array($mlid, [2, 3661])) {
    $options['attributes']['class'][] = 'btn btn-link';
    $title =  '<i class="far fa-user"></i><span>' . $title . '</span>';
  }

  $href = $element['#href'];
  $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();

  $sub_menu = '';
  $depth = $element['#original_link']['depth'];
  if (!empty($element['#below'])) {
    unset($element['#below']['#theme_wrappers']);
    $sub_menu .= '<div class="dropdown-menu level-' . ($depth + 1) . '-wrapper">';
    if ($GLOBALS['user']->uid) {

      $sub_menu .= '<div class="user-info">' .
                        (module_exists('realname') ? realname_load($GLOBALS['user']) : $GLOBALS['user']->name) .
                        '<span>' . $GLOBALS['user']->mail . '</span>' .
                    '</div>';
    }
    $sub_menu .=    '<ul class="level-' . ($depth + 1) . '">' .
                        drupal_render($element['#below']) .
                    '</ul>' .
        '</div>';

    $attributes['class'][] = 'dropdown';
    $attributes['id'] = 'dropdown';

    $options['attributes']['class'][] = 'dropdown-toggle';
    $options['attributes']['data-toggle'] = 'dropdown';
  }
  $attributes['class'][] = 'level-' . $depth . '-item';

  return '<li' . drupal_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>";
}

function strada_menu_link__menu_help(array $variables)
{
//    $element = $variables['element'];
//    $sub_menu = '';
//
//    if ($element['#below']) {
//        $sub_menu = drupal_render($element['#below']);
//    }
//    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
//    return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>";


    // оформлять меню лесенкой
    $element = $variables['element'];
    $sub_menu = '';

    $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();
    $depth = $element['#original_link']['depth'];

    // Check plain title if "html" is not set, otherwise, filter for XSS attacks.
    $mlid = $element['#original_link']['mlid'];
    $title = empty($options['html']) ? check_plain($element['#title']) : filter_xss_admin($element['#title']);

    // Ensure "html" is now enabled so l() doesn't double encode. This is now
    // safe to do since both check_plain() and filter_xss_admin() encode HTML
    // entities. See: https://www.drupal.org/node/2854978
    $options['html'] = TRUE;

    $href = $element['#href'];
    $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();
    $tid = null;
    $menu = $leaves = '';
    if ($element['#original_link']['module'] == 'taxonomy_menu') {
        $tid = str_replace('taxonomy/term/', '', $element['#original_link']['link_path']);
        $entities = _get_help_menu_entities($tid);
        $leaves = _pack_entities_to_list($entities);
    }

    if (!empty($element['#below'])) {
        unset($element['#below']['#theme_wrappers']);
        $menu = drupal_render($element['#below']);
    }

    $caret = '';
    if ($menu || $leaves) {
        $sub_menu = '<div class="level-' . ($depth + 1) . '-wrapper sub-menu">' .
                        '<ul class="level-' . ($depth + 1) . ' dry-list">' .
                            $menu .
                            $leaves .
                        '</ul>' .
                    '</div>';
        $caret = '<i class="fas fa-caret-right"></i>';
    }
    if ($sub_menu) $attributes['class'][] = 'expanded';

    $attributes['class'][] = 'level-' . $depth . '-item';

    if ($depth == 1) return $sub_menu;
    else return $sub_menu ? ('<li' . drupal_attributes($attributes) . '>' . ($caret ? '<div class="sub-menu-title">' . $caret : '') . l($title, $href, $options) . ($caret ? '</div>' : '') . $sub_menu . "</li>") : '';
}

function _get_help_menu_entities($tid = null)
{
    // выбрать продукты у чьих нод категория совпадает
    $query = db_select('node', 'n')->distinct();
    $query->condition('n.type', 'info');
    $query->fields('n', array('nid', 'title'));
    $query->innerJoin('field_data_field_i_category', 'fc', 'fc.entity_id = n.nid');
    $query->condition('fc.field_i_category_tid', $tid);

    return $query->execute()->fetchAll();
}

// завернуть вписок в ul
function _pack_entities_to_list($entities = null)
{
    $list = '';
    if ($entities) {
        foreach ($entities as $entity) {
            $title = $entity->title;
            $class = [];
            if ($_GET['q'] == 'node/' . $entity->nid) $class[] = 'active';
            $list .= '<li class="leaf"><a href="' . url('node/' . $entity->nid) . '" class="' . implode(' ', $class) . '">' . $title . '</a></li>';
        }
    }
    return $list;
}