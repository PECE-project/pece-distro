<?php

/**
 * @file
 *   Menu callbacks for admin pages.
 */

/**
 * Admin settings page.
 *
 * @ingroup module_builder_core
 */
function module_builder_admin_settings($form, &$form_state) {
  $public_dir = drupal_realpath(file_default_scheme() . '://');

  $form['module_builder_hooks_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to hook documentation directory'),
    '#description' => t('Subdirectory in the directory "%dir" where local copies of hook documentation should be stored.', array('%dir' => $public_dir)),
    '#default_value' => variable_get('module_builder_hooks_directory', 'hooks'),
  );

  /*
  // Temporarily disabled, as there's currently no functionality to write files.
  $form['module_builder_write_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to write module files'),
    '#description' => t('Subdirectory in the directory "%dir" where module files should be written.', array('%dir' => $public_dir)),
    '#default_value' => variable_get('module_builder_write_directory', 'modules'),
  );
  */

  /*
  $form['module_builder_download'] = array(
    '#type' => 'radios',
    '#title' => t('Download module file checkbox defaults to'),
    '#description' => t('When checked, this will automatically generate your module file for you and prompt your browser to download it.'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => variable_get('module_builder_download', 1),
  );
  */
  return system_settings_form($form);
}

/**
 * Admin hook update form.
 */
function module_builder_admin_update($form, &$form_state) {
  module_builder_init_library();

  // Get our task, and check hook data is ready.
  try {
    $mb_task_handler_report = \DrupalCodeBuilder\Factory::getTask('ReportHookDataFolder');
  }
  catch (\DrupalCodeBuilder\Exception\SanityException $e) {
    $failed_sanity_level = $e->getFailedSanityLevel();
    switch ($failed_sanity_level) {
      case 'hook_directory':
        drupal_set_message(t("The hook data directory could not be created or is not writable."), 'error');
        break;
      case 'hook_data':
        drupal_set_message(t("Hook data is not available. Use the 'Process data' tab to get data about hooks and plugins."), 'error');
        break;
    }
    return $form;
  }

  // The task handler returns sane values for these even if there's no hook
  // data.
  $last_update = $mb_task_handler_report->lastUpdatedDate();
  $doc_files = $mb_task_handler_report->listHookFiles();
  $directory = \DrupalCodeBuilder\Factory::getEnvironment()->getHooksDirectory();

  $form['intro'] = array(
    '#markup' => '<p>' . t("Module Builder analyses your site's code to find data about Drupal components such as hooks, plugins, tagged services, and more." . ' '
      . "This processed data is stored in your local filesystem." . ' '
      . "You should update the code analysis when updating site code, or updating Module Builder or Drupal Code Builder."
      ) . '</p>',
  );

  $form['analyse'] = [
    '#type' => 'fieldset',
    '#title' => "Perform analysis",
  ];

  $form['analyse']['last_update'] = array(
    '#markup' => '<p>' . (
      $last_update ?
        t('Your last hook documentation update was %date.', array(
          '%date' => format_date($last_update, 'large'),
        )) :
        t("The site's code has not yet been analysed.")
      ) . '</p>',
  );

  $form['analyse']['submit'] = array(
    '#type' => 'submit',
    '#value' => $last_update
      ? t('Update code analysis')
      : t('Perform code analysis'),
  );

  if ($last_update) {
    $form['results'] = [
      '#type' => 'fieldset',
      '#title' => "Analysis results",
    ];

    $form['results']['text'] = array(
      '#markup' => '<p>' . t('You have the following data saved in %dir: ', array(
        '%dir' => $directory,
      )) . '</p>',
    );

    $form['results']["files"] = array(
      '#markup' => theme('item_list', array('items' => $doc_files)),
    );
  }

  return $form;
}

/**
 * Admin hook update form submit handler.
 */
function module_builder_admin_update_submit($form, $form_state) {
  // Safe to do this without exception handling: it's already been checked in
  // the form builder.
  $mb_task_handler_collect = \DrupalCodeBuilder\Factory::getTask('Collect');
  $mb_task_handler_collect->collectComponentData();
}
