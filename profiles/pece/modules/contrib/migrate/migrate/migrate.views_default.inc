<?php

/**
 * @file migrate.views_default.inc
 * Contains default views.
 */

/**
 * Implements hook_views_default_views().
 */
function migrate_views_default_views() {
  $migrations = migrate_migrations();
  $views = array();
  foreach ($migrations as $migration) {
    // We can only do things with migrations that use the SQLMap.
    $map = $migration->getMap();
    $migration_name = $migration->getMachineName();
    if (!is_a($map, 'MigrateSQLMap')) {
      continue;
    }

    // Skip non-entities.
    $destination = $migration->getDestination();
    if (!is_a($destination, 'MigrateDestinationEntity')) {
      continue;
    }

    $view = migrate_views_default_views_get_view($migration);

    // Add view to list of views to provide.
    $views[$view->name] = $view;

  }
  return $views;
}

/**
 * Helper for migrate_views_default_views() to get a single view.
 *
 * @param $migration
 *  A migration object.
 *
 * @return
 *  A view object.
 */
function migrate_views_default_views_get_view($migration) {
  $migration_name = $migration->getMachineName();
  $destination = $migration->getDestination();
  $entity_type = $destination->getEntityType();
  $entity_info = entity_get_info($entity_type);
  $entity_base_table = $entity_info['base table'];
  $map_table = $migration->getMap()->getMapTable();

  // The ID for the relationship from the entity base table.
  $map_table_relationship_id = 'migrate_map_' . $map_table;

  // Use the same logic as the Views wizard to figure out a field to add to the
  // view.
  $data = views_fetch_data($entity_base_table);
  if (isset($data['table']['base']['defaults']['field'])) {
    $field = $data['table']['base']['defaults']['field'];
  }
  else {
    foreach ($data as $field => $field_data) {
      if (isset($field_data['field']['handler'])) {
        break;
      }
    }
  }

  $view = new view;
  $view->name = 'migrate_' . $migration_name;
  $view->description = 'Audit view for the migration ' . $migration_name;
  $view->tag = 'migrate';
  $view->base_table = $entity_base_table;
  $view->human_name = $migration_name;
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Audit for migration ' . $migration_name;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'migration information';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: Migration map */
  $handler->display->display_options['relationships'][$map_table_relationship_id]['id'] = $map_table_relationship_id;
  $handler->display->display_options['relationships'][$map_table_relationship_id]['table'] = $entity_base_table;
  $handler->display->display_options['relationships'][$map_table_relationship_id]['field'] = $map_table_relationship_id;
  $handler->display->display_options['relationships'][$map_table_relationship_id]['required'] = TRUE;
  /* Field: Entity ID for the entity type */
  if (isset($entity_info['entity keys']['id'])) {
    $entity_id_key = $entity_info['entity keys']['id'];
    if (isset($data[$entity_id_key]['field']['handler'])) {
      $handler->display->display_options['fields'][$entity_id_key]['id'] = $entity_id_key;
      $handler->display->display_options['fields'][$entity_id_key]['table'] = $entity_base_table;
      $handler->display->display_options['fields'][$entity_id_key]['field'] = $entity_id_key;
    }
  }
  /* Field: Default field for the entity type */
  $handler->display->display_options['fields'][$field]['id'] = $field;
  $handler->display->display_options['fields'][$field]['table'] = $entity_base_table;
  $handler->display->display_options['fields'][$field]['field'] = $field;
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/migrate/audit/' . $migration_name;

  return $view;
}
