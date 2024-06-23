<?php

/**
 * @file
 * panopoly_search.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function panopoly_search_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'panopoly_search';
  $page->task = 'page';
  $page->admin_title = 'Search Page';
  $page->admin_description = '';
  $page->path = 'search/site/!keywords';
  $page->access = array(
    'logic' => 'and',
    'plugins' => array(
      0 => array(
        'context' => 'logged-in-user',
        'name' => 'perm',
        'not' => FALSE,
        'settings' => array(
          'perm' => 'search content',
        ),
      ),
    ),
    'settings' => NULL,
    'type' => 'none',
  );
  $page->menu = array();
  $page->arguments = array(
    'keywords' => array(
      'id' => 1,
      'identifier' => 'String or query string',
      'name' => 'string_or_query',
      'settings' => array(
        'fallback_value' => '',
        'key' => 'keys',
        'use_tail' => 0,
      ),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_panopoly_search_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'panopoly_search';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        1 => array(
          'name' => 'solr',
          'not' => FALSE,
          'settings' => NULL,
        ),
        2 => array(
          'name' => 'search_api_index',
          'not' => FALSE,
          'settings' => NULL,
        ),
      ),
    ),
    'contexts' => array(),
    'css' => '',
    'css_id' => 'solr-search-panel',
    'no_blocks' => 0,
    'panels_breadcrumbs_home' => 1,
    'panels_breadcrumbs_paths' => 'search/site',
    'panels_breadcrumbs_state' => 1,
    'panels_breadcrumbs_titles' => 'Search',
    'pipeline' => 'ipe',
    'relationships' => array(),
    'title' => 'Solr Search Panel',
  );
  $display = new panels_display();
  $display->layout = 'radix_burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'contentmain' => NULL,
      'default' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Search Results';
  $display->uuid = 'b369b7fe-466f-4e52-b315-5070800e6d63';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_panopoly_search_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-15f4a1d6-8bf1-43cd-8ef3-a8b484aabe79';
  $pane->panel = 'contentmain';
  $pane->type = 'search_current';
  $pane->subtype = 'search_current';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'form' => 'advanced',
    'override_prompt' => FALSE,
    'path' => '',
    'path_type' => 'default',
    'prompt' => '',
    'type' => 'node',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '15f4a1d6-8bf1-43cd-8ef3-a8b484aabe79';
  $display->content['new-15f4a1d6-8bf1-43cd-8ef3-a8b484aabe79'] = $pane;
  $display->panels['contentmain'][0] = 'new-15f4a1d6-8bf1-43cd-8ef3-a8b484aabe79';
  $pane = new stdClass();
  $pane->pid = 'new-9315ef28-12fb-4ed8-a491-1c628e698406';
  $pane->panel = 'contentmain';
  $pane->type = 'search_box';
  $pane->subtype = 'search_box';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(),
    'form' => 'simple',
    'override_prompt' => FALSE,
    'override_title' => 1,
    'override_title_text' => '',
    'path' => '',
    'path_type' => 'default',
    'prompt' => '',
    'type' => 'node',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '9315ef28-12fb-4ed8-a491-1c628e698406';
  $display->content['new-9315ef28-12fb-4ed8-a491-1c628e698406'] = $pane;
  $display->panels['contentmain'][1] = 'new-9315ef28-12fb-4ed8-a491-1c628e698406';
  $pane = new stdClass();
  $pane->pid = 'new-25ec26bf-8e25-4163-bf5c-406fed80259d';
  $pane->panel = 'contentmain';
  $pane->type = 'views_panes';
  $pane->subtype = 'panopoly_search-search_solr_results';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(
      0 => 'argument_string_or_query_1',
    ),
    'fields_override' => array(
      'body' => 1,
      'field_featured_image' => 1,
      'title' => 1,
    ),
    'header_type' => 'none',
    'items_per_page' => '',
    'override_title' => '',
    'override_title_heading' => 'h2',
    'override_title_text' => '',
    'view_mode' => 'teaser',
    'view_settings' => 'fields',
    'widget_title' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '25ec26bf-8e25-4163-bf5c-406fed80259d';
  $display->content['new-25ec26bf-8e25-4163-bf5c-406fed80259d'] = $pane;
  $display->panels['contentmain'][2] = 'new-25ec26bf-8e25-4163-bf5c-406fed80259d';
  $pane = new stdClass();
  $pane->pid = 'new-3d1478d7-0f89-424e-824f-82483bc89651';
  $pane->panel = 'sidebar';
  $pane->type = 'facet';
  $pane->subtype = 'facet';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(),
    'delta' => 'Shb0Q1vWgCArrHrjHszTyhl2jaEhIjzW',
    'override_title' => 1,
    'override_title_text' => 'Filter by Type',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '3d1478d7-0f89-424e-824f-82483bc89651';
  $display->content['new-3d1478d7-0f89-424e-824f-82483bc89651'] = $pane;
  $display->panels['sidebar'][0] = 'new-3d1478d7-0f89-424e-824f-82483bc89651';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-15f4a1d6-8bf1-43cd-8ef3-a8b484aabe79';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_panopoly_search_panel_context_3';
  $handler->task = 'page';
  $handler->subtask = 'panopoly_search';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'access' => array(
      'logic' => 'and',
      'plugins' => array(),
    ),
    'body_classes_to_add' => '',
    'body_classes_to_remove' => '',
    'contexts' => array(),
    'css' => '',
    'css_id' => 'database-search-panel',
    'no_blocks' => 0,
    'panels_breadcrumbs_home' => 1,
    'panels_breadcrumbs_paths' => 'search/site',
    'panels_breadcrumbs_state' => 1,
    'panels_breadcrumbs_titles' => 'Search',
    'pipeline' => 'ipe',
    'relationships' => array(),
    'title' => 'Database Search Panel',
  );
  $display = new panels_display();
  $display->layout = 'radix_burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'contentmain' => NULL,
      'default' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Search Results';
  $display->uuid = '2d27c2cb-916a-4d51-8a64-9eba5a57a81e';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_panopoly_search_panel_context_3';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-2693d0e2-3b32-4ca3-87b6-fa2ccec54399';
  $pane->panel = 'contentmain';
  $pane->type = 'search_current';
  $pane->subtype = 'search_current';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'form' => 'advanced',
    'override_prompt' => FALSE,
    'path' => '',
    'path_type' => 'default',
    'prompt' => '',
    'type' => 'node',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '2693d0e2-3b32-4ca3-87b6-fa2ccec54399';
  $display->content['new-2693d0e2-3b32-4ca3-87b6-fa2ccec54399'] = $pane;
  $display->panels['contentmain'][0] = 'new-2693d0e2-3b32-4ca3-87b6-fa2ccec54399';
  $pane = new stdClass();
  $pane->pid = 'new-dbac3f16-6f98-43f5-a2b3-bb1b0dc363d9';
  $pane->panel = 'contentmain';
  $pane->type = 'search_box';
  $pane->subtype = 'search_box';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(),
    'form' => 'simple',
    'override_prompt' => FALSE,
    'override_title' => 1,
    'override_title_text' => '',
    'path' => '',
    'path_type' => 'default',
    'prompt' => '',
    'type' => 'node',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'dbac3f16-6f98-43f5-a2b3-bb1b0dc363d9';
  $display->content['new-dbac3f16-6f98-43f5-a2b3-bb1b0dc363d9'] = $pane;
  $display->panels['contentmain'][1] = 'new-dbac3f16-6f98-43f5-a2b3-bb1b0dc363d9';
  $pane = new stdClass();
  $pane->pid = 'new-6e68b43e-ac55-4417-9705-64e3c639c681';
  $pane->panel = 'contentmain';
  $pane->type = 'views_panes';
  $pane->subtype = 'panopoly_database_search-search_database_results';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(
      0 => 'argument_string_or_query_1',
    ),
    'exposed' => array(
      'widget_title' => '',
    ),
    'fields_override' => array(
      'body' => 1,
      'field_featured_image' => 1,
      'title' => 1,
    ),
    'header_type' => 'none',
    'items_per_page' => '15',
    'override_title' => '',
    'override_title_heading' => 'h2',
    'override_title_text' => '',
    'pager_id' => '0',
    'use_pager' => 1,
    'view_mode' => 'teaser',
    'view_settings' => 'fields',
    'widget_title' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '6e68b43e-ac55-4417-9705-64e3c639c681';
  $display->content['new-6e68b43e-ac55-4417-9705-64e3c639c681'] = $pane;
  $display->panels['contentmain'][2] = 'new-6e68b43e-ac55-4417-9705-64e3c639c681';
  $pane = new stdClass();
  $pane->pid = 'new-44eb1f21-6415-4344-ae5a-45172ed2cb8c';
  $pane->panel = 'sidebar';
  $pane->type = 'facet';
  $pane->subtype = 'facet';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(),
    'delta' => '1Cpx6naJU4Y3YvKVc0vcLK7Yo0ahaN0l',
    'override_title' => 1,
    'override_title_text' => 'Filter by Type',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '44eb1f21-6415-4344-ae5a-45172ed2cb8c';
  $display->content['new-44eb1f21-6415-4344-ae5a-45172ed2cb8c'] = $pane;
  $display->panels['sidebar'][0] = 'new-44eb1f21-6415-4344-ae5a-45172ed2cb8c';
  $display->hide_title = PANELS_TITLE_PANE;
  $display->title_pane = 'new-2693d0e2-3b32-4ca3-87b6-fa2ccec54399';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['panopoly_search'] = $page;

  return $pages;

}
