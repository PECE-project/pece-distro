<?php

/**
 * @file
 * Support for paths in core Drupal objects
 */

class MigratePathEntityHandler extends MigrateDestinationHandler {

  public function __construct() {
    $this->registerTypes(array('entity'));
  }

  /**
   * Implementation of MigrateDestinationHandler::fields().
   */
  public function fields($entity_type, $bundle, $migration = NULL) {
    if (module_exists('path')) {
      return array('path' => t('Path alias'));
    }
    return array();
  }

  public function prepare($entity, stdClass $row) {
    if (module_exists('path') && isset($entity->path)) {
      // Make sure the alias doesn't already exist
      $query = db_select('url_alias')
        ->condition('alias', $entity->path)
        ->condition('language', $entity->language);
      $query->addExpression('1');
      $query->range(0, 1);
      if (!$query->execute()->fetchField()) {
        $path = $entity->path;
        $entity->path = array();
        $entity->path['alias'] = $path;
      }
      else {
        unset($entity->path);
      }
    }
  }

  public function complete($entity, stdClass $row) {
    // Check if this is a forum taxonomy term.
    if (module_exists('forum')) {
      if (isset($entity->vocabulary_machine_name) && $entity->vocabulary_machine_name == 'forums') {
        // Check if a path ID exists for this term.
        $term_pid = db_select('url_alias', 'url_alias')
          ->fields('url_alias', array('pid'))
          ->condition('source', 'taxonomy/term/' . $entity->tid)
          ->condition('language', $entity->language)
          ->execute()
          ->fetchField();

        // Check if there is also a path ID for this term referencing forums.
        $forum_term_pid = db_select('url_alias', 'url_alias')
          ->fields('url_alias', array('pid'))
          ->condition('source', 'forum/' . $entity->tid)
          ->condition('language', $entity->language)
          ->execute()
          ->fetchField();

        // If both term and forum term path IDs exist, delete the term's path.
        if ($term_pid && $forum_term_pid) {
          db_delete('url_alias')
            ->condition('pid', $term_pid)
            ->execute();
        }
        // If a term path ID exists but forum term path ID does not,
        // update term path to match what the forum module expects.
        elseif ($term_pid && empty($forum_term_pid)) {
          db_update('url_alias')
            ->fields(array('source' => 'forum/' . $entity->tid))
            ->condition('pid', $term_pid)
            ->execute();
        }
      }
    }
  }
}
