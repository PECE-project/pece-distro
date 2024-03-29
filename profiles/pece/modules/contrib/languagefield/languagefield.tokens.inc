<?php

/**
 * @file
* Token module integration.
*
* Pretty much copy-pasted from addressfield.
*/

/**
 * Implements hook_token_info().
*/
function languagefield_token_info() {
  $type = array(
    'name' => t('Language field'),
    'description' => t('Tokens related to languagefields.'),
    'needs-data' => 'language-field',
    'field' => TRUE,
  );

  $info['value'] = array(
    'name' => t('Language code'),
    'description' => t('ISO 639 language code.'),
  );

  return array(
    'types' => array('language-field' => $type),
    'tokens' => array('language-field' => $info),
  );
}

/**
 * Returns a list of language fields optionally filtered by entity type.
 *
 * @param string $entity_type
 *   Optional machine-name of an entity type to filter the returned array by.
 *
 * @return array
 *   An array of language field mapping data.
 */
function languagefield_get_language_fields($entity_type = '') {
  $fields = &drupal_static(__FUNCTION__ . '_' . $entity_type);

  if (isset($fields)) {
    return $fields;
  }

  // Get mapping data for all language fields.
  $fields = array_filter(field_info_field_map(), 'languagefield_field_map_filter');

  // Filter the list of fields by entity type if specified.
  if (!empty($fields) && !empty($entity_type)) {
    foreach ($fields as $field_name => $field) {
      if (!isset($field['bundles'][$entity_type])) {
        unset($fields[$field_name]);
      }
    }
  }

  return $fields;
}

/**
 * Implements hook_tokens().
 */
function languagefield_tokens($type, $tokens, array $data = array(), array $options = array()) {
  if (isset($options['language'])) {
    $language_code = $options['language']->language;
  }
  else {
    $language_code = LANGUAGE_NONE;
  }

  $sanitize = !empty($options['sanitize']);

  $replacements = array();

  if ($type == 'language-field' && !empty($data['language-field'][$language_code]) && is_array($data['language-field'][$language_code])) {
    $language = reset($data['language-field'][$language_code]);

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'value':
          $replacements[$original] = $sanitize ? check_plain($language['value']) : $language['value'];
          break;

      }
    }
  }

  if ($type == 'entity') {
    // Loop over the language fields defined on the site.
    foreach (languagefield_get_language_fields($data['entity_type']) as $field_name => $field) {
      if ($languagefield_tokens = token_find_with_prefix($tokens, $field_name)) {
        // If the current field is on the matching entity type...
        if (!empty($field['bundles'][$data['entity_type']])) {
          // Extract the format handlers selected in a representative instance
          // settings form for use in formatting tokens.
          $instance = field_info_instance($data['entity_type'], $field_name, reset($field['bundles'][$data['entity_type']]));
        }

        $replacements += token_generate('language-field', $languagefield_tokens, array('language-field' => $data['entity']->$field_name), $options);
      }
    }
  }

  return $replacements;
}

/**
 * Returns TRUE if a field map array value represents an languagefield.
 *
 * Provided for use as a callback by array_filter().
 */
function languagefield_field_map_filter($field) {
  return !empty($field['type']) && $field['type'] == 'language_field';
}
