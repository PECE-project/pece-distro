<?php

/**
 * @file
 * pece_artifacts.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function pece_artifacts_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'node';
  $panelizer->panelizer_key = 'pece_artifact';
  $panelizer->access = array();
  $panelizer->view_mode = 'page_manager';
  $panelizer->name = 'node:pece_artifact:default';
  $panelizer->css_id = '';
  $panelizer->css_class = '';
  $panelizer->css = '';
  $panelizer->no_blocks = FALSE;
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = array();
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $display = new panels_display();
  $display->layout = 'radix_phelan';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'column1' => NULL,
      'column2' => NULL,
      'default' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '003e28f4-414b-425f-a7f0-5445462c9337';
  $display->storage_type = 'panelizer_default';
  $display->storage_id = 'node:pece_artifact:default';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-680119d6-2670-4662-b2eb-2eb0e5442325';
  $pane->panel = 'column1';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:body';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'panelizer',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter' => 'text_default',
    'formatter_settings' => array(),
    'label' => 'hidden',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '680119d6-2670-4662-b2eb-2eb0e5442325';
  $display->content['new-680119d6-2670-4662-b2eb-2eb0e5442325'] = $pane;
  $display->panels['column1'][0] = 'new-680119d6-2670-4662-b2eb-2eb0e5442325';
  $pane = new stdClass();
  $pane->pid = 'new-0a41b978-ae6e-4b07-b4d7-8107254206f5';
  $pane->panel = 'column1';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_pece_annotations';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'panelizer',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter' => 'entityreference_label',
    'formatter_settings' => array(
      'link' => FALSE,
    ),
    'label' => 'above',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '0a41b978-ae6e-4b07-b4d7-8107254206f5';
  $display->content['new-0a41b978-ae6e-4b07-b4d7-8107254206f5'] = $pane;
  $display->panels['column1'][1] = 'new-0a41b978-ae6e-4b07-b4d7-8107254206f5';
  $pane = new stdClass();
  $pane->pid = 'new-1acee996-8b71-485a-915f-483aaf8f1105';
  $pane->panel = 'column1';
  $pane->type = 'node_links';
  $pane->subtype = 'node_links';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'build_mode' => 'page_manager',
    'context' => 'panelizer',
    'identifier' => '',
    'link' => TRUE,
    'override_title' => FALSE,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_class' => 'link-wrapper',
  );
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '1acee996-8b71-485a-915f-483aaf8f1105';
  $display->content['new-1acee996-8b71-485a-915f-483aaf8f1105'] = $pane;
  $display->panels['column1'][2] = 'new-1acee996-8b71-485a-915f-483aaf8f1105';
  $pane = new stdClass();
  $pane->pid = 'new-cb6c67f2-6b90-4b41-885e-cc85bd3cb5b7';
  $pane->panel = 'column2';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_pece_contributors';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'panelizer',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter' => 'entityreference_label',
    'formatter_settings' => array(
      'link' => FALSE,
    ),
    'label' => 'above',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'cb6c67f2-6b90-4b41-885e-cc85bd3cb5b7';
  $display->content['new-cb6c67f2-6b90-4b41-885e-cc85bd3cb5b7'] = $pane;
  $display->panels['column2'][0] = 'new-cb6c67f2-6b90-4b41-885e-cc85bd3cb5b7';
  $pane = new stdClass();
  $pane->pid = 'new-506a3d5c-6240-4c1f-99a2-05550b3e4317';
  $pane->panel = 'column2';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_pece_fieldsite';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'panelizer',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter' => 'entityreference_label',
    'formatter_settings' => array(
      'link' => FALSE,
    ),
    'label' => 'above',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '506a3d5c-6240-4c1f-99a2-05550b3e4317';
  $display->content['new-506a3d5c-6240-4c1f-99a2-05550b3e4317'] = $pane;
  $display->panels['column2'][1] = 'new-506a3d5c-6240-4c1f-99a2-05550b3e4317';
  $pane = new stdClass();
  $pane->pid = 'new-f98d75b6-3599-439b-92eb-125f0f133d0f';
  $pane->panel = 'column2';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_pece_lisence';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'panelizer',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter' => 'entityreference_label',
    'formatter_settings' => array(
      'link' => FALSE,
    ),
    'label' => 'above',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'f98d75b6-3599-439b-92eb-125f0f133d0f';
  $display->content['new-f98d75b6-3599-439b-92eb-125f0f133d0f'] = $pane;
  $display->panels['column2'][2] = 'new-f98d75b6-3599-439b-92eb-125f0f133d0f';
  $pane = new stdClass();
  $pane->pid = 'new-eb3e0897-1d9f-453e-ad8c-389796bd5be1';
  $pane->panel = 'column2';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:og_group_ref';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'panelizer',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter' => 'og_list_default',
    'formatter_settings' => array(),
    'label' => 'above',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = 'eb3e0897-1d9f-453e-ad8c-389796bd5be1';
  $display->content['new-eb3e0897-1d9f-453e-ad8c-389796bd5be1'] = $pane;
  $display->panels['column2'][3] = 'new-eb3e0897-1d9f-453e-ad8c-389796bd5be1';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-1acee996-8b71-485a-915f-483aaf8f1105';
  $panelizer->display = $display;
  $export['node:pece_artifact:default'] = $panelizer;

  return $export;
}
