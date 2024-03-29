<?php

/**
 * @file
 * A specification for the custom spotlight entity that is part of Panopoly Widgets
 */

/**
 * Implements hook_field_info().
 */
function panopoly_widgets_field_info() {
  return array(
    'panopoly_spotlight' => array(
      'label' => t('Spotlight'),
      'description' => t('A wonderfully widgetsal spotlight field'),
      'default_widget' => 'panopoly_spotlight_form',
      'default_formatter' => 'panopoly_spotlight_default',
      'settings' => array(
        'uri_scheme' => variable_get('file_default_scheme', 'public'),
        'default_image' => 1,
      ),
      'instance_settings' => array(
        'file_extensions' => 'jpg jpeg gif png',
        'file_directory' => 'general',
        'max_filesize' => '',
        'alt_field' => 0,
        'title_field' => 0,
        'max_resolution' => '',
        'min_resolution' => '',
      ),
    ),
  );
}

/**
 * Implements hook_field_is_empty()
 */
function panopoly_widgets_field_is_empty($item, $field) {
  if (empty($item['title']) && empty($item['link']) && empty($item['description']) && empty($item['fid'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_widget_info().
 */
function panopoly_widgets_field_widget_info() {
  return array(
    'panopoly_spotlight_form' => array(
      'label' => t('Spotlight'),
      'field types' => array('panopoly_spotlight'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_settings_form()
 */
function panopoly_widgets_field_settings_form($field, $instance, $has_data) {
  $form = array();
  $defaults = field_info_field_settings($field['type']);
  $settings = array_merge($defaults, $field['settings']);

  $scheme_options = array();
  foreach (file_get_stream_wrappers(STREAM_WRAPPERS_WRITE_VISIBLE) as $scheme => $stream_wrapper) {
    $scheme_options[$scheme] = $stream_wrapper['name'];
  }
  $form['uri_scheme'] = array(
    '#type' => 'radios',
    '#title' => t('Upload destination'),
    '#options' => $scheme_options,
    '#default_value' => $settings['uri_scheme'],
    '#description' => t('Select where the final files should be stored. Private file storage has significantly more overhead than public files, but allows restricted access to files within this field.'),
  );

  // When the user sets the scheme on the UI, even for the first time, it's
  // updating a field because fields are created on the "Manage fields"
  // page. So image_field_update_field() can handle this change.
  $form['default_image'] = array(
    '#title' => t('Default image'),
    '#type' => 'managed_file',
    '#description' => t('If no image is uploaded, this image will be shown on display.'),
    '#default_value' => $field['settings']['default_image'],
    '#upload_location' => $settings['uri_scheme'] . '://default_images/',
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form()
 */
function panopoly_widgets_field_instance_settings_form($field, $instance) {
  $form = image_field_instance_settings_form($field, $instance);
  return $form;
}

/**
 * Implementation of hook_field_formatter_info().
 */
function panopoly_widgets_field_formatter_info() {
  return array(
    'panopoly_spotlight_default' => array(
      'label' => t('Default'),
      'field types' => array('panopoly_spotlight'),
      'settings' => array(
        'image_style' => 'panopoly_image_spotlight',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function panopoly_widgets_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'panopoly_spotlight_default') {
    $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#options' => image_style_options(),
      '#default_value' => $settings['image_style'],
      '#required' => TRUE,
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function panopoly_widgets_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = t('Image Style: @style', array('@style' => $settings['image_style']));
  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function panopoly_widgets_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  // Include the Spotlight Rotation time setting
  $duration_field = field_get_items('fieldable_panels_pane', $entity, 'field_basic_spotlight_duration');
  if ($duration_field) {
    $duration_field = reset($duration_field);
    $duration = $duration_field['value'];
  }
  if (empty($duration)) {
    $duration = variable_get('panopoly_widgets_spotlight_rotation_time', 5);
  }

  $pager_style = 'full'; // Default to the Full pager if field isn't filled out (i.e., legacy data).
  $pager_style_field = field_get_items('fieldable_panels_pane', $entity, 'field_basic_spotlight_pager');
  if ($pager_style_field) {
    $pager_style_field = reset($pager_style_field);
    $pager_style = $pager_style_field['value'];
  }

  $element = array();
  $settings = $display['settings'];
  $formatter = $display['type'];

  // Load the images and filter out slides with missing images.
  foreach ($items as $delta => $item_data) {
    if (!empty($item_data['uuid']) && module_exists('uuid')) {
      $image_entity = entity_uuid_load('file', array($item_data['uuid']));
      $image = file_load(array_pop($image_entity)->fid);
    }
    else {
      $image = (object)$item_data;
    }
    if (empty($image->uri) || !file_exists($image->uri)) {
      unset($items[$delta]);
    }
    else {
      $items[$delta]['image'] = $image;
    }
  }

  // Assemble the spotlight wrapper
  // $element[0] rather than $element because hook_field_formatter_view()
  // expects a renderable array for the $items, as an array of child elements
  // keyed by numeric indexes starting from 0.
  $element[0] = array(
    '#theme' => 'panopoly_spotlight_wrapper',
    '#items' => $items,
    '#settings' => $settings,
    '#formatter' => $formatter,
    '#duration' => $duration,
    '#pager_style' => $pager_style,
    '#entity_type' => $entity_type,
    '#entity' => $entity,
    'slides' => array(),
  );

  // Assemble the spotlight items (rendered in panopoly_spotlight_wrapper())
  foreach ($items as $delta => $item_data) {
    if (isset($item_data['field_title'])) {
      $item_data['title'] = $item_data['field_title'];
    }
    if (isset($item_data['field_alt'])) {
      $item_data['alt'] = $item_data['field_alt'];
    }
    $element[0]['slides'][] = array(
      '#theme' => 'panopoly_spotlight_view',
      '#items' => $item_data,
      '#delta' => $delta,
      '#settings' => $settings,
      '#entity_type' => $entity_type,
      '#entity' => $entity,
    );
  }

  return $element;
}

/**
 * Implements hook_field_widget_form().
 */
function panopoly_widgets_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  if ($instance['widget']['type'] == 'panopoly_spotlight_form') {

    // Make it a multipart form
    $form['#attributes']['enctype'] = 'multipart/form-data';

    // Define the element
    $element['title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['field_title']) ? $items[$delta]['field_title'] : NULL,
    );

    $element['link'] = array(
      '#title' => t('Link'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['link']) ? $items[$delta]['link'] : NULL,
      '#maxlength' => 255,
      '#linkit' => array(
        'enable' => TRUE,
        'profile' => 'content_fields',
        'button_text' => t('Search for existing content'),
      ),
    );

    $instance_settings = $instance['settings'];
    $field_settings = $field['settings'];

    $element['fid'] = array(
      '#title' => t('Image'),
      '#type' => 'media',
      '#media_options' => array(
        'global' => array(
          'types' => array(
            'image' => 'image'
           ),
          'enabledPlugins' => array(
            'media_default--media_browser_1' => 'media_default--media_browser_1',
            'upload' => 'upload',
          ),
          'schemes' => array(
            'private' => 'private',
            'public' => 'public',
          ),
          'file_directory' => isset($instance_settings['file_directory']) ? $instance_settings['file_directory'] : '',
          'file_extensions' => isset($instance_settings['file_extensions']) ? $instance_settings['file_extensions'] : 'jpg jpeg gif png',
          'max_filesize' => isset($instance_settings['max_filesize']) ? $instance_settings['max_filesize'] : 0,
          'min_resolution' => isset($instance_settings['min_resolution']) ? $instance_settings['min_resolution'] : 0,
          'max_resolution' => isset($instance_settings['max_resolution']) ? $instance_settings['max_resolution'] : 0,
          'uri_scheme' => !empty($field_settings['uri_scheme']) ? $field_settings['uri_scheme'] : file_default_scheme(),
          'multiselect' => FALSE,
        ),
      ),
      '#default_value' => isset($items[$delta]['fid']) ? $items[$delta]['fid'] : NULL,
    );

    // Allow cropping via the manualcrop module.
    if (module_exists('manualcrop') && user_access("use manualcrop")) {
      $fid = isset($form_state['values'][$field['field_name']][$langcode][$delta]['fid']) ?
        $form_state['values'][$field['field_name']][$langcode][$delta]['fid'] : $element['fid']['#default_value'];
      $file = !empty($fid) ? file_load($fid) : NULL;

      // If the remove button was clicked, then unset the file.
      $triggering_parents = !empty($form_state['triggering_element']) ? $form_state['triggering_element']['#parents'] : array();
      if (count($triggering_parents) >= 5 && $triggering_parents[4] == 'remove_button' && $triggering_parents[2] == $delta) {
        $file = NULL;
      }

      if (!empty($file)) {
        $manualcrop = array(
          'manualcrop_enable' => TRUE,
          'manualcrop_keyboard' => TRUE,
          'manualcrop_thumblist' => FALSE,
          'manualcrop_inline_crop' => FALSE,
          'manualcrop_crop_info' => TRUE,
          'manualcrop_instant_preview' => TRUE,
          'manualcrop_instant_crop' => FALSE,
          'manualcrop_default_crop_area' => TRUE,
          'manualcrop_maximize_default_crop_area' => TRUE,
          'manualcrop_styles_mode' => 'include',
          'manualcrop_styles_list' => array(
            'panopoly_image_spotlight' => 'panopoly_image_spotlight',
          ),
          'manualcrop_require_cropping' => array(),
        );
        manualcrop_croptool_process($form, $form_state, $element['fid'], $file, $manualcrop);
      }
      else {
        // Mark this element as supporting manualcrop, even if the crop tool
        // isn't currently available.
        $element['fid']['manualcrop'] = array();
      }
    }

    $element['alt'] = array(
      '#title' => t('Alt text'),
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['field_alt']) ? $items[$delta]['field_alt'] : NULL,
    );

    $element['description'] = array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#rows' => '2',
      '#resizable' => FALSE,
      '#default_value' => isset($items[$delta]['description']) ? $items[$delta]['description'] : NULL,
    );
  }

  return $element;
}

/**
 * Implements hook_field_validate().
 */
function panopoly_widgets_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!panopoly_widgets_field_is_empty($item, $field) && empty($item['fid'])) {
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'panopoly_widgets_spotlight_image_required',
        'message' => t('!name field is required.', array('!name' => t('Image'))),
      );
    }
  }
}

/**
 * Implements hook_field_presave()
 */
function panopoly_widgets_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  // We need to set presave values if fids are not set. This shouldn't effect save operations
  // form API validation will prevent this from ever happening unless previewing.
  if (!empty($items)) {
    foreach ($items as &$item) {
      if (empty($item['fid'])) {
        $fid = variable_get('panopoly_widgets_default_' . $entity->bundle . '_fid', NULL);

        if (!empty($fid)) {
          $item['fid'] = $fid;
        }
      }
      if (isset($item['field_title'])) {
        $item['title'] = $item['field_title'];
      }
      if (isset($item['field_alt'])) {
        $item['alt'] = $item['field_alt'];
      }
    }
    image_field_presave($entity_type, $entity, $field, $instance, $langcode, $items);
  }
}

/**
 * Implements hook_field_load().
 */
function panopoly_widgets_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  $entity_items_copy = $items;
  image_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
  // Both image_field_load() and file_entity_entity_load() will overwrite the
  // 'title' and 'alt' with the one from the file. Here we save the originals
  // for later.
  foreach ($entity_items_copy as $entity_id => $items_copy) {
    foreach ($items_copy as $delta => $item) {
      $items[$entity_id][$delta]['field_title'] = $item['title'];
      $items[$entity_id][$delta]['field_alt'] = $item['alt'];
    }
  }
}

/**
 * Implements hook_field_insert().
 */
function panopoly_widgets_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  image_field_insert($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 */
function panopoly_widgets_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {

  // Load the original entity, if $entity->original is not already set.
  // This is done as per http://drupal.org/node/985642 and can be removed when that patch is committed.
  if (empty($entity->original)) {
    $entity->original = entity_load_unchanged($entity_type, $field['id']);
  }

  image_field_update($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_delete().
 */
function panopoly_widgets_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  image_field_delete($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_delete_revision().
 */
function panopoly_widgets_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
  image_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_prepare_view().
 */
function panopoly_widgets_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // If there are no files specified at all, use the default.
  // TODO - make sure this works
  foreach ($entities as $id => $entity) {
    if (empty($items[$id]) && $field['settings']['default_image']) {
      if ($file = file_load($field['settings']['default_image'])) {
        $items[$id][0] = (array) $file + array(
          'is_default' => TRUE,
          'alt' => '',
          'title' => '',
        );
      }
    }
  }
}

/**
 * Theme function for the spotlight wrapper
 */
function theme_panopoly_spotlight_wrapper($variables){
  $output = '';
  $items = $variables['element']['#items'];
  $duration = $variables['element']['#duration'];
  $pager_style = $variables['element']['#pager_style'];
  $element = array();

  $output .= '<div class="panopoly-spotlight-widget-wrapper">';

  // Assemble the tabs header
  $output .= '<div class="panopoly-spotlight-buttons-wrapper" data-rotation-time="' . $duration * 1000 . '">';
  if (count($items) > 1) {
    if ($pager_style == 'full') {
      $output .= '<ul>';
    }
    else {
      // We're not displaying a full pager but we still want to put the links
      // in the DOM, because the Javascript depends on them.
      $output .= '<ul style="display: none;">';
    }
    foreach ($items as $delta => $item_data) {
      $slide_id = drupal_html_id('panopoly-spotlight-' . $delta);
      $variables['element']['slides'][$delta]['#slide_id'] = $slide_id;
      $output .= '<li>' . '<a href="#' . $slide_id . '">' . ($delta+1) . '</a></li>';
    }
    $output .= '</ul>';

    if (variable_get('panopoly_widgets_spotlight_pause_play_buttons', 1) || $pager_style == 'mini') {
      $output .= '<div class="panopoly-spotlight-function-buttons">';

      // Have to split up previous and next so that we can squeeze pause between them if needed.
      if ($pager_style == 'mini') {
        $output .= '<a href="#" class="panopoly-spotlight-previous">' . t('Prev') . '</a>';
      }
      if (variable_get('panopoly_widgets_spotlight_pause_play_buttons', 1)) {
        $output .= '<a href="#" class="panopoly-spotlight-pause-play">' . t('Pause') . '</a>';
      }
      if ($pager_style == 'mini') {
        $output .= '<a href="#" class="panopoly-spotlight-next">' . t('Next') . '</a>';
      }

      $output .= '</div>';
    }
  }

  $output .= '</div>';  // class="panopoly-spotlight-buttons-wrapper"

  $output .= render($variables['element']['slides']);

  $output .= '</div>';  // class="panopoly-spotlight-widget-wrapper"

  return $output;
}

/**
 * Theme function for spotlight items
 */
function theme_panopoly_spotlight_view($variables) {
  $output = '';

  $title = $variables['element']['#items']['title'];
  $description = $variables['element']['#items']['description'];
  $link = $variables['element']['#items']['link'];
  $alt = $variables['element']['#items']['alt'];
  $settings = $variables['element']['#settings'];
  $delta = $variables['element']['#delta'];
  $slide_id = isset($variables['element']['#slide_id']) ? $variables['element']['#slide_id'] : ('panopoly-spotlight-' . $delta);

  $image = $variables['element']['#items']['image'];
  $image_markup = theme('image_style', array('style_name' => $settings['image_style'], 'path' => $image->uri, 'alt' => $alt));

  $output = '<div id="' . $slide_id . '" class="' . 'panopoly-spotlight' . '">';

  if ($link) {
    $output .= l($image_markup, $link, array('html' => TRUE));
  }
  else {
    $output .= $image_markup;
  }

  $output .= '<div class="panopoly-spotlight-wrapper">';
  if (!empty($title)) {
    $output .= '<h3 class="panopoly-spotlight-label">' . (empty($link) ? check_plain($title) : l($title, $link)) . '</h3>';
  }
  if (!empty($description)) {
    $output .= '<div class="panopoly-spotlight-info">';
    $output .= '<p>' . $description . '</p>';
    $output .= '</div>';
  }
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}
