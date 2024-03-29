<?php

/**
 * @file
 * pece_artifacts.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function pece_artifacts_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_settings|node|pece_artifact|form';
  $field_group->group_name = 'group_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pece_artifact';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'og_group_ref',
    ),
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => 'group-settings field-group-fieldset',
        'description' => '',
        'required_fields' => 1,
      ),
    ),
    'format_type' => 'fieldset',
    'label' => 'Group settings',
    'weight' => '3',
  );
  $field_groups['group_settings|node|pece_artifact|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Group settings');

  return $field_groups;
}
