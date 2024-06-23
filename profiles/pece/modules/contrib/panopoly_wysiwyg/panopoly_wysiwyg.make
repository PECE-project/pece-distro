; Panopoly WYSIWYG Makefile

api = 2
core = 7.x

; The WYSIWYG Module Family

projects[wysiwyg][subdir] = contrib
projects[wysiwyg][version] = 2.10
projects[wysiwyg][patch][1489096] = https://www.drupal.org/files/issues/2019-11-16/wysiwyg-table-format-1489096-10.patch
projects[wysiwyg][patch][1786732] = https://www.drupal.org/files/issues/2019-11-16/wysiwyg-arbitrary_image_paths_markitup-1786732-6.patch
projects[wysiwyg][patch][3261512] = https://www.drupal.org/files/issues/2022-02-01/wysiwyg-php7-compatibility-3261512_1.patch

projects[wysiwyg_filter][version] = 1.6-rc9
projects[wysiwyg_filter][subdir] = contrib

; The WYSIWYG Helpers

projects[linkit][version] = 3.7
projects[linkit][subdir] = contrib
projects[linkit][patch][3401666] = https://www.drupal.org/files/issues/2023-11-14/linkit-dynamic-properties-php82-3401666-2.patch

projects[image_resize_filter][version] = 1.16
projects[image_resize_filter][subdir] = contrib

projects[caption_filter][version] = 1.3
projects[caption_filter][subdir] = contrib

; Include our Editors

libraries[tinymce][type] = library
libraries[tinymce][download][type] = get
libraries[tinymce][download][url] = https://download.tiny.cloud/tinymce/community/tinymce_3.5.12.zip
libraries[tinymce][patch][1561882] = http://drupal.org/files/1561882-cirkuit-theme-tinymce-3.5.8.patch
libraries[tinymce][patch][2876031] = https://www.drupal.org/files/issues/2022-12-01/tinymce-chrome58-fix-2876031-13.patch

libraries[markitup][type] = library
libraries[markitup][download][type] = git
libraries[markitup][download][url] = https://github.com/markitup/1.x.git
libraries[markitup][download][revision] = 2c88c42
libraries[markitup][download][branch] = master
libraries[markitup][patch][1715642] = http://drupal.org/files/1715642-adding-html-set-markitup-editor.patch
