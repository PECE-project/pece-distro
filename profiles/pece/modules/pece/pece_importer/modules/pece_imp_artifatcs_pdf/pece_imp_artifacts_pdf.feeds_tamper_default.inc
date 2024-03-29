<?php

/**
 * @file
 * pece_imp_artifacts_pdf.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function pece_imp_artifacts_pdf_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-author-implode';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'author';
  $feeds_tamper->plugin_id = 'implode';
  $feeds_tamper->settings = array(
    'glue' => ',',
    'real_glue' => ',',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Implode';
  $export['pece_imp_artifacts_pdf-author-implode'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-author-username2uid';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'author';
  $feeds_tamper->plugin_id = 'string2id';
  $feeds_tamper->settings = array(
    'method' => 'views',
    'views' => array(
      'view' => 'user_info_by_username:default',
    ),
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert username to uid';
  $export['pece_imp_artifacts_pdf-author-username2uid'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-content_creators-explode';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'content_creators';
  $feeds_tamper->plugin_id = 'explode';
  $feeds_tamper->settings = array(
    'limit' => '',
    'real_separator' => ';',
    'separator' => ';',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Explode on semicolon';
  $export['pece_imp_artifacts_pdf-content_creators-explode'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-content_creators-string2id';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'content_creators';
  $feeds_tamper->plugin_id = 'string2id';
  $feeds_tamper->settings = array(
    'method' => 'autocomplete',
    'views' => array(
      'view' => 'user_info_by_username:default',
    ),
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Convert creators name into entity ID';
  $export['pece_imp_artifacts_pdf-content_creators-string2id'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-content_creators-trim';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'content_creators';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim - Whitespace will be removed from both sides.';
  $export['pece_imp_artifacts_pdf-content_creators-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-contributors-contributors_names2uid';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'contributors';
  $feeds_tamper->plugin_id = 'string2id';
  $feeds_tamper->settings = array(
    'method' => 'views',
    'views' => array(
      'view' => 'user_info_by_username:default',
    ),
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Convert contributors username into entity ID';
  $export['pece_imp_artifacts_pdf-contributors-contributors_names2uid'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-contributors-explode';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'contributors';
  $feeds_tamper->plugin_id = 'explode';
  $feeds_tamper->settings = array(
    'limit' => '',
    'real_separator' => ';',
    'separator' => ';',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Explode on semicolon';
  $export['pece_imp_artifacts_pdf-contributors-explode'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-contributors-trim';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'contributors';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim - Whitespace will be removed from both sides.';
  $export['pece_imp_artifacts_pdf-contributors-trim'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-tags-explode';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'tags';
  $feeds_tamper->plugin_id = 'explode';
  $feeds_tamper->settings = array(
    'limit' => '',
    'real_separator' => ';',
    'separator' => ';',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Explode on semicolon';
  $export['pece_imp_artifacts_pdf-tags-explode'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-tags-item_name2id';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'tags';
  $feeds_tamper->plugin_id = 'string2id';
  $feeds_tamper->settings = array(
    'method' => 'autocomplete',
    'views' => array(
      'view' => 'user_info_by_username:default',
    ),
  );
  $feeds_tamper->weight = 2;
  $feeds_tamper->description = 'Convert item name to id';
  $export['pece_imp_artifacts_pdf-tags-item_name2id'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'pece_imp_artifacts_pdf-tags-trim';
  $feeds_tamper->importer = 'pece_imp_artifacts_pdf';
  $feeds_tamper->source = 'tags';
  $feeds_tamper->plugin_id = 'trim';
  $feeds_tamper->settings = array(
    'mask' => '',
    'side' => 'trim',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Trim - Whitespace will be removed from both sides.';
  $export['pece_imp_artifacts_pdf-tags-trim'] = $feeds_tamper;

  return $export;
}
