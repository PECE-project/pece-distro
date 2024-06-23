; Panopoly Core Makefile

api = 2
core = 7.x

; Panels and Chaos Tools Magic

projects[ctools][version] = 1.21
projects[ctools][subdir] = contrib
projects[ctools][patch][1910608] = https://www.drupal.org/files/issues/2023-06-22/views_content-keyword-substitution-1910608-84.patch
projects[ctools][patch][1000146] = https://www.drupal.org/files/issues/2019-01-07/views_panes-more_link_text-1000146-32.patch
projects[ctools][patch][3284043] = https://www.drupal.org/files/issues/2022-06-03/ctools-3284043-php8-compatibility.patch

projects[panels][version] = 3.12
projects[panels][subdir] = contrib

projects[panels_breadcrumbs][version] = 2.6
projects[panels_breadcrumbs][subdir] = contrib

projects[panelizer][version] = 3.4
projects[panelizer][subdir] = contrib
projects[panelizer][patch][1549608] = https://www.drupal.org/files/issues/panelizer-n1549608-26.patch
projects[panelizer][patch][2788851] = https://www.drupal.org/files/issues/panelizer-administer-panelizer-2788851-2.patch

projects[fieldable_panels_panes][version] = 1.13
projects[fieldable_panels_panes][subdir] = contrib

projects[pm_existing_pages][version] = 1.4
projects[pm_existing_pages][subdir] = contrib

projects[fape][version] = 1.2
projects[fape][subdir] = contrib

; Views Magic

projects[views][version] = 3.29
projects[views][subdir] = contrib
projects[views][patch][2284423] = https://www.drupal.org/files/issues/2019-04-29/views-same_sort_twice-2284423-3.patch
projects[views][patch][3385759] = https://www.drupal.org/files/issues/2023-09-06/views-htmlspecialchars_decode-php81-3385759-2.patch

projects[views_autocomplete_filters][version] = 1.2
projects[views_autocomplete_filters][subdir] = contrib
projects[views_autocomplete_filters][patch][2374709] = http://www.drupal.org/files/issues/views_autocomplete_filters-cache-2374709-2.patch
projects[views_autocomplete_filters][patch][2317351] = http://www.drupal.org/files/issues/views_autocomplete_filters-content-pane-2317351-4.patch
projects[views_autocomplete_filters][patch][2404893] = https://www.drupal.org/files/issues/2404893-grammar_correction-11.patch

projects[views_bulk_operations][version] = 3.7
projects[views_bulk_operations][subdir] = contrib

; The Usual Suspects

projects[pathauto][version] = 1.3
projects[pathauto][subdir] = contrib

projects[token][version] = 1.9
projects[token][subdir] = contrib

projects[entity][version] = 1.10
projects[entity][subdir] = contrib
projects[entity][patch][3328049] = https://www.drupal.org/files/issues/2023-04-30/3328049-10.patch

projects[libraries][version] = 2.5
projects[libraries][subdir] = contrib

projects[transliteration][version] = 3.2
projects[transliteration][subdir] = contrib

; Field modules

projects[date][version] = 2.14
projects[date][subdir] = contrib

projects[entityreference][version] = 1.9
projects[entityreference][subdir] = contrib

projects[field_group][version] = 1.8
projects[field_group][subdir] = contrib

projects[link][version] = 1.11
projects[link][subdir] = contrib

; Harness the Power of Features and Apps with Default Content

projects[apps][version] = 1.1
projects[apps][subdir] = contrib
projects[apps][patch][2945929] = https://www.drupal.org/files/issues/apps-php7-compat-2945929.patch

projects[features][version] = 2.15
projects[features][subdir] = contrib

projects[strongarm][version] = 2.0
projects[strongarm][subdir] = contrib

projects[defaultconfig][version] = 1.0-alpha11
projects[defaultconfig][subdir] = contrib
projects[defaultconfig][patch][1900574] = http://drupal.org/files/1900574.defaultconfig.undefinedindex_11.patch

projects[defaultcontent][version] = 1.0-alpha9
projects[defaultcontent][subdir] = contrib
projects[defaultcontent][patch][1754428] = http://drupal.org/files/1754428-allow-node-export-alter.patch
projects[defaultcontent][patch][1757782] = http://drupal.org/files/1757782-cannot-import-menu-hierarchy-8.patch
projects[defaultcontent][patch][2946138] = https://www.drupal.org/files/issues/defaultcontent-php7-compat-2946138.patch
projects[defaultcontent][patch][3172478] = https://www.drupal.org/files/issues/2020-09-22/defaultcontent-php74-implode-3172478-2.patch

projects[migrate][version] = "2.12"
projects[migrate][type] = "module"
projects[migrate][subdir] = "contrib"
projects[migrate][patch][3401693] = https://www.drupal.org/files/issues/2023-11-14/migrate-dynamic-properties-php82-3401693-3.patch

projects[migrate_extras][version] = "2.5"
projects[migrate_extras][type] = "module"
projects[migrate_extras][subdir] = "contrib"

; jQuery Update was moved to Panopoly Core, but is left in Panopoly Admin's
; .make file to retain a stable 1.x branch of Panopoly. See the following URL
; for more information: http://drupal.org/node/2492811
projects[jquery_update][version] = 2.7
projects[jquery_update][subdir] = contrib

; Recommended Modules
projects[devel][version] = 1.7
projects[devel][subdir] = contrib

projects[distro_update][version] = 1.0-beta4
projects[distro_update][subdir] = contrib

projects[features_override][version] = 2.0-rc3
projects[features_override][subdir] = contrib

projects[uuid][version] = 1.3
projects[uuid][subdir] = contrib
