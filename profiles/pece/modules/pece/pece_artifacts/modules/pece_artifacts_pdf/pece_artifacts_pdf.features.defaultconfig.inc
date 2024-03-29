<?php

/**
 * @file
 * pece_artifacts_pdf.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function pece_artifacts_pdf_defaultconfig_features() {
  return array(
    'pece_artifacts_pdf' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function pece_artifacts_pdf_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create pece_artifact_pdf content'.
  $permissions['create pece_artifact_pdf content'] = array(
    'name' => 'create pece_artifact_pdf content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'platform-director' => 'platform-director',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any pece_artifact_pdf content'.
  $permissions['delete any pece_artifact_pdf content'] = array(
    'name' => 'delete any pece_artifact_pdf content',
    'roles' => array(
      'administrator' => 'administrator',
      'platform-director' => 'platform-director',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own pece_artifact_pdf content'.
  $permissions['delete own pece_artifact_pdf content'] = array(
    'name' => 'delete own pece_artifact_pdf content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'platform-director' => 'platform-director',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any pece_artifact_pdf content'.
  $permissions['edit any pece_artifact_pdf content'] = array(
    'name' => 'edit any pece_artifact_pdf content',
    'roles' => array(
      'administrator' => 'administrator',
      'platform-director' => 'platform-director',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own pece_artifact_pdf content'.
  $permissions['edit own pece_artifact_pdf content'] = array(
    'name' => 'edit own pece_artifact_pdf content',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
      'platform-director' => 'platform-director',
    ),
    'module' => 'node',
  );

  return $permissions;
}
