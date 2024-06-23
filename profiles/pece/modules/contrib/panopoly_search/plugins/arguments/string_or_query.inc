<?php

/**
 * @file
 * Plugin to provide an argument handler for a string or query string.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("String or query string"),
  // Keyword to use for %substitution.
  'keyword' => 'string',
  'description' => t('A string or query string if it is not set.'),
  'settings form' => 'panopoly_search_string_or_query_settings_form',
  'context' => 'panopoly_search_string_or_query_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter a value for this argument'),
  ),
  // This is in pagemanager.
  'path placeholder' => 'panopoly_search_string_or_query_path_placeholder',
);

/**
 * Discover if this argument gives us the term we crave.
 */
function panopoly_search_string_or_query_context($data = NULL, $conf = NULL, $empty = FALSE) {
  $context = ctools_context_create('string', $data);
  $context->original_argument = $data;

  if (empty($data) && $conf) {
    if (!empty($_GET[$conf['key']])) {
      $context->data = $_GET[$conf['key']];
    }
    else {
      $context->data = $conf['fallback_value'];
    }
  }

  return $context;
}

/**
 * Settings form for the argument.
 */
function panopoly_search_string_or_query_settings_form(&$form, &$form_state, $conf) {
  $form['settings']['use_tail'] = array(
    '#title' => t('Get all arguments after this one'),
    '#type' => 'checkbox',
    '#default_value' => !empty($conf['use_tail']),
    '#description' => t('If checked, this string will include all arguments. For example, if the path is "path/%" and the user visits "path/foo/bar", if this is not checked the string will be "foo". If it is checked the string will be "foo/bar".'),
  );

  $form['settings']['key'] = array(
    '#title' => t('Query string key'),
    '#description' => t('Enter the key of the value that must be returned from the query string.'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  if (isset($conf['key'])) {
    $form['settings']['key']['#default_value'] = $conf['key'];
  }
  $form['settings']['fallback_value'] = array(
    '#title' => t('Fallback value'),
    '#description' => t('Enter a value that must be returned if the above specified key does not exist in the query string.'),
    '#type' => 'textfield',
  );
  if (!empty($conf['fallback_value'])) {
    $form['settings']['fallback_value']['#default_value'] = $conf['fallback_value'];
  }
}

/**
 * Switch the placeholder based upon user settings.
 */
function panopoly_search_string_or_query_path_placeholder($argument) {
  if (empty($argument['settings']['use_tail'])) {
    return '%pm_arg';
  }
  else {
    return '%pm_arg_tail';
  }
}
