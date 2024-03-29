<?php

/**
 * @file
 * pece_groups.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function pece_groups_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_fields|node|pece_group|form';
  $field_group->group_name = 'group_fields';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'pece_group';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'children' => array(
      0 => 'field_pece_media_image',
      1 => 'group_access',
      2 => 'group_group',
      3 => 'og_group_ref',
      4 => 'og_roles_permissions',
      5 => 'group_content_access',
    ),
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'classes' => 'group-fields field-group-fieldset',
        'description' => 'Be aware that this content might become <strong>Visible</strong> to anyone if you assign it to an <strong>Open Group</strong>.
',
        'id' => '',
        'required_fields' => 1,
      ),
      'label' => 'Group settings',
    ),
    'format_type' => 'fieldset',
    'label' => 'Group settings',
    'weight' => '3',
  );
  $field_groups['group_fields|node|pece_group|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Group settings');

  return $field_groups;
}
