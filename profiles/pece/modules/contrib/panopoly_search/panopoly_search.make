; Panopoly Search Makefile

api = 2
core = 7.x

; Search API and Facet API Modules

projects[facetapi][version] = 1.9
projects[facetapi][subdir] = contrib
projects[facetapi][patch][3324815] = https://www.drupal.org/files/issues/2022-12-01/facetapi_deprecated_functions-3324815-2.patch
projects[facetapi][patch][3401917] = https://www.drupal.org/files/issues/2023-11-15/facetapi-dynamic-properties-php82-3401917-2.patch

projects[search_api][version] = 1.29
projects[search_api][subdir] = contrib
projects[search_api][patch][3402005] = https://www.drupal.org/files/issues/2023-11-15/search_api-dynamic-properties-php82-3402005-2.patch

projects[search_api_solr][version] = 1.16
projects[search_api_solr][subdir] = contrib

projects[search_api_db][version] = 1.9
projects[search_api_db][subdir] = contrib

; Solr PHP Client Library

libraries[SolrPhpClient][type] = library
libraries[SolrPhpClient][download][type] = get
libraries[SolrPhpClient][download][url] = https://github.com/PTCInc/solr-php-client/archive/master.zip
