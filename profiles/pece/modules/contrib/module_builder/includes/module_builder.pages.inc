<?php

/**
 * @file
 * Menu callback for main module builder page.
 *
 */

// Use DCB's copy of D8's NestedArray.
use DrupalCodeBuilder\Utility\NestedArray;

/**
 * @defgroup module_builder_callback Functions which are the menu callbacks for this module
 */

/**
 * Displays module builder interface via a multi-step form.
 * The steps are:
 *
 * - input    => shows a form where the user can enter module options.
 * - module   => shows the generated module and info files.
 * - download => pushes a file for download.
 * - write    => writes files.
 *
 * @ingroup module_builder_callback
 * @param $form_values will be NULL when the page is first displayed,
 *   when the form is submitted, this will be an array of the submitted
 *   values.
 * @return
 *   One of three results depending on the state of this multi-step form.
 *   Form for entering module options
 *   Form showing built module and info file
 *   Nothing, but file is pushed to client for download
 */
function module_builder_page($form, &$form_state) {
  module_builder_init_library();

  // Set up multistep form.
  // Once again, thank you examples.module!
  if (empty($form_state['step'])) {
    $form_state['step'] = 'input';

    // This array contains the function to be called at each step to get the
    // relevant form elements. It will also store state information for each
    // step.
    $form_state['step_information'] = array(
      'input'     => 'module_builder_page_input',
      'generate'  => 'module_builder_page_generate',
      // back: same as input
      // download: 'module_builder_page_download'
    );
  }

  // Call the function named in $form_state['step_information'] to get the
  // form elements to display for this step.
  $form_builder_function = $form_state['step_information'][$form_state['step']];
  $form = $form_builder_function($form, $form_state);

  return $form;
}

/**
 * Form builder for page 1.
 */
function module_builder_page_input($form, &$form_state) {
  // Get our task handler, which checks hook data is ready, and our generate
  // handler, from which to get the list of properties to collect.
  try {
    $mb_task_handler_report = \DrupalCodeBuilder\Factory::getTask('ReportHookData');
    $mb_task_handler_generate = \DrupalCodeBuilder\Factory::getTask('Generate', 'module');
  }
  catch (\DrupalCodeBuilder\Exception\SanityException $e) {
    if ($e->getFailedSanityLevel() == 'component_data_processed') {
      drupal_set_message(t('No hooks were found. Please check the documentation path specified in the <a href="!settings-url">%administer >> %config >> %develop >> %modulebuilder</a> page.',
        array(
          '!settings-url' => url('admin/config/development/module_builder'),
          '%administer' => 'Administer',
          '%config' => 'Site configuration',
          '%develop' => 'Development',
          '%modulebuilder' => "Module builder",
        )
      ), 'warning');
      return $form;
    }
  }

  // Get the component data info.
  $component_data_info = $mb_task_handler_generate->getRootComponentDataInfo();

  $component_data = array();

  module_load_include('inc', 'module_builder', 'includes/module_builder.form');
  $form_helper = new ModuleBuilderComponentFormBase($mb_task_handler_generate);

  $form_state_object = new FormState($form_state);
  $form = $form_helper->componentPropertiesForm($form, $form_state_object);

  // Get the hook names for the presets for our Javascript to work with.
  $mb_task_handler_report_presets = \DrupalCodeBuilder\Factory::getTask('ReportHookPresets');
  $hook_presets = $mb_task_handler_report_presets->getHookPresets();
  foreach ($hook_presets as $hook_preset_name => $hook_preset_data) {
    // Build a reverse array whose keys are hooks and values are arrays
    // of one of more preset names.
    // This allows us further down the line to put classes on each hook
    // checkbox for the presets they belong to: this is what the JS then uses.
    foreach ($hook_preset_data['hooks'] as $hook) {
      $hook_presets_reverse[$hook][] = "preset-$hook_preset_name";
    }
  }
  //dsm($hook_presets_reverse);

  // Rework the hooks form element into fieldsets with checkboxes.
  $hook_groups = $mb_task_handler_report->listHookOptionsStructured();

  $form['data']['hooks'] = array(
    '#title' => t('Use the following specific hooks'),
    '#tree' => TRUE,
  );
  foreach ($hook_groups as $hook_group => $hooks) {
    $form['data']['hooks'][$hook_group] = array(
      '#type' => 'fieldset',
      '#title' => $hook_group . ' hooks',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      //'#theme'  => 'module_builder_hook_list',
      '#tree' => TRUE,
    );
    foreach ($hooks as $hook_name => $hook_info) {
      // Skip callbacks.
      if ($hook_info['type'] == 'callback') {
        continue;
      }

      $form['data']['hooks'][$hook_group][$hook_name] = array(
        '#type' => 'checkbox',
        '#title' => $hook_name,
        '#description' => $hook_info['description'],
        '#repopulate' => TRUE,

        // TODO: For some reason this gives me some wacky error under PHP 5:
        // Fatal error: Cannot use string offset as an array
        //'#default_value' => $edit['hooks'][$hook_group][$hook],

        // TODO: I would *like* to do something like the following, so some of the long
        // descriptions don't totally mangle the page output, but need a way to do like
        // a "onmouseover" effect to display the full thing. Note that 'title' is not
        // a valid attribute for divs. :\
        //'#description' => truncate_utf8($hook_description, 40, TRUE, TRUE),

      );
      // If this hook belongs to any presets, add the classes to it for the
      // Javascript to find.
      if (isset($hook_presets_reverse[$hook_name])) {
        $form['data']['hooks'][$hook_group][$hook_name]['#attributes'] = array(
          'class' => $hook_presets_reverse[$hook_name],
        );
      }

      // Set some default hooks
      if ($hook_name == 'hook_menu') {
        $form['data']['hooks'][$hook_group][$hook_name]['#default_value'] = 1;
      }
    }
    // Sort list alphabetically
    ksort($form['data']['hooks'][$hook_group]);
  }

  // Add our own (longer) descriptions and defaults.
  // TODO: consider moving some of these to the Module generator.
  $form['data']['root_name']['#description'] = t('This string is used to name the module files and to prefix all of your functions. This must only contain letters, numbers, and underscores, and may not start with a number.');
  $form['data']['readable_name']['#description'] = t('Name of your module as it will appear on the module admin page.');
  $form['data']['short_description']['#description'] = t('This text will appear in the module admin list at <a href="!adminmodules">%administer >> %modules</a>.',
    array(
      '!adminmodules' => url('admin/modules'),
      '%administer' => 'Administer',
      '%modules' => 'Modules',
    ));
  $form['data']['short_description']['#default_value'] = t('Does awesome things. Makes tea. Washes up. Favours of a personal nature.');
  $form['data']['module_help_text']['#description'] = t('Help text (HTML) to appear within the system help at <a href="!help">%administer >> %help >> module_name</a>',
    array(
      '!help' => url('admin/help'),
      '%administer' => 'Administer',
      '%help' => 'Help',
    ));
  $form['data']['module_dependencies']['#description'] = t('Space separated list of other modules that your module requires.');
  $form['data']['module_package']['#description'] = t('If your module comes with other modules or is meant to be used exclusively with other modules, enter the package name here. Suggested package names: Audio, Bot, CCK, Chat, E-Commerce, Event, Feed parser, Organic groups, Station, Video, Views and Voting.'
    . '<br />'
    . 'For more information on package names, see the <a href="!url-package">documentation on Drupal.org</a>.', array(
      '!url-package' => 'http://drupal.org/node/542202#package',
    ));

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'module_builder') . '/theme/module_builder.js',
  );

  // Include CSS for formatting
  drupal_add_css(drupal_get_path('module', 'module_builder') . '/theme/module_builder.css');

  // Mark form as fresh to enable JS clearing of fields with sample text.
  // TODO: multiform re-fill may work differently on D7.
  // Failing that, look into ctools?
  $form['#attributes'] = array('class' => 'fresh');

  $form['generate_module'] = array(
    '#type' => 'submit',
    '#name' => 'generate',
    '#value' => t('Generate'),
  );
  $form['#submit'] = array('module_builder_page_input_submit');

  //dsm($form);
  return $form;
}

/**
 * Repopulate form with user values.
 */
function _form_repopulate($form, $form_state) {
  #dsm($form);
  #dsm(element_children($form));
  #dsm($form_state);
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#repopulate'])) {
      #dsm('repop: ');
      #dsm($key);
      #$form[$key]['#default_value'] = 'repop!'; // this obviously works
      #$form[$key]['#default_value'] = $form_state['values'][$key]; // arg! here we only have values from page 2!
      $form[$key]['#default_value'] = $form_state['storage']['input'][$key]; // this obviously works
    }
    // recurse into children
    $form[$key] = _form_repopulate($form[$key], $form_state);
  } // each element_children
  return $form;
}

/**
 * Form submit handler for page 1
 */
function module_builder_page_input_submit($form, &$form_state) {
  // Trigger multistep.
  $form_state['rebuild'] = TRUE;
  // Set the next step.
  $form_state['step'] = 'generate';

  // stash input.... these values will follow us around everywhere like a little dog...
  $form_state['storage']['input'] = $form_state['values'];
  foreach (array('generate', 'generate_module', 'form_build_id', 'form_token', 'form_id') as $key) {
    unset($form_state['storage']['input'][$key]);
  }
}

/**
 * Form builder for page 2: generate code.
 */
function module_builder_page_generate($form, &$form_state) {
  module_load_include('inc', 'module_builder', 'includes/module_builder.form');

  $mb_task_handler_generate = \DrupalCodeBuilder\Factory::getTask('Generate', 'module');
  $form_helper = new ModuleBuilderComponentFormBase($mb_task_handler_generate);

  $form_state_object = new FormState($form_state);

  $module_data = $form_helper->copyFormValuesToEntity($form, $form_state_object);

  // Ungroup the hooks value.
  $hooks = [];
  foreach ($form_state['values']['data']['hooks'] as $group_values) {
    // Filter out empty values. (FormAPI *still* doesn't do this???)
    $group_hooks = array_filter($group_values);
    // Store as a numeric array.
    $group_hooks = array_keys($group_hooks);

    $hooks = array_merge($group_hooks, $hooks);;
  }
  $module_data['hooks'] = $hooks;

  // Presets get expanded with JS.
  unset($module_data['module_hook_presets']);

  // Get the files.
  $files = $mb_task_handler_generate->generateComponent($module_data);
  //dsm($files);

  /*
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#name' => 'back',
  );
  */

  foreach ($files as $filename => $code) {
    $form['code_instructions_' . $filename] = array(
      '#markup' => t('Please copy and paste the following text into a file called !module.', array('!module' => $filename)),
      '#prefix' => '<div class="module-message">',
      '#suffix' => '</div>',
    );
    $form['module_code_'  . $filename] = array(
      '#type' => 'textarea',
      '#title' => t($filename .' code'),
      '#rows' => count(explode("\n", $code)),
      '#default_value' => $code,
      '#prefix' => '<div class="module-code">',
      '#suffix' => '</div>',
    );
  }

  /*
  $form['code_instructions'] = array(
    '#value' => t('Please copy and paste the following text into a file called !module.', array('!module' => $form_state['values']['root_name'] .'.module')),
    '#prefix' => '<div id="module-message">',
    '#suffix' => '</div>',
  );
  $form['module_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Module code'),
    '#rows' => 20,
    '#default_value' => $code,
    '#prefix' => '<div id="module-code">',
    '#suffix' => '</div>',
  );
  */
  /*
  $form['download_module'] = array(
    '#type' => 'submit',
    '#value' => t('Download module'),
  );
  $form['write_module'] = array(
    '#type' => 'button',
    '#value' => t('Write module file'),
  );
  */

  /*
  $form['download_info'] = array(
    '#type' => 'submit',
    '#name' => 'op',
    '#value' => t('Download info file'),
  );
  $form['write_info'] = array(
    '#type' => 'button',
    '#value' => t('Write info file'),
  );
  */

  // handle the write buttons
 ## $form['#after_build'] = array('module_builder_write_buttons');

  return $form;
}

/**
 * Helper function: creates an array of all the data needed to build the module
 * from form values, suitable for passing to Generate::generateComponent().
 */
function module_data_from_form($form, $form_values, $component_data_info) {
  // Most things come in as we want them from the form.
  $module_data = $form_values;

  // Hooks need flattening.
  $module_data['hooks'] = array();
  foreach ($form_values['hooks'] as $hook_group) {
    $module_data['hooks'] += array_keys(array_filter($hook_group));
  }

  // Build list.
  // The UI always gets the full code.
  $module_data['requested_build'] = array('code' => TRUE, 'info' => TRUE);

  // Generic processing.
  foreach ($component_data_info as $property_name => $property_info) {
    // Skip hooks, they got handled specially.
    if ($property_name == 'hooks') {
      continue;
    }

    if ($property_info['format'] == 'array') {
      if (isset($property_info['options'])) {
        // Anything with options needs to have the crud filtered out.
        $module_data[$property_name] = array_filter($module_data[$property_name]);
      }
      else {
        // Textareas need splitting on newlines into arrays.
        $module_data[$property_name] = preg_split("@\s*\n\s*@", $module_data[$property_name], -1, PREG_SPLIT_NO_EMPTY);
      }
    }
  }

  return $module_data;
}

/**
 * Various wrappers and workarounds so ModuleBuilderComponentFormBase works
 * with a minimum of changes from the D8 version.
 */

// Wrappers for ModuleBuilderComponentFormBase AJAX callbacks.
function mb_addItemSubmit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  module_builder_init_library();
  module_load_include('inc', 'module_builder', 'includes/module_builder.form');

  $form_state_object = new FormState($form_state);

  ModuleBuilderComponentFormBase::addItemSubmit($form, $form_state_object);
}

function mb_removeItemSubmit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;

  module_builder_init_library();
  module_load_include('inc', 'module_builder', 'includes/module_builder.form');

  $form_state_object = new FormState($form_state);

  ModuleBuilderComponentFormBase::removeItemSubmit($form, $form_state_object);
}

function mb_itemButtonAjax($form, &$form_state) {
  return $form['data']['permissions'];
  module_builder_init_library();
  module_load_include('inc', 'module_builder', 'includes/module_builder.form');

  $form_state_object = new FormState($form_state);

  return ModuleBuilderComponentFormBase::itemButtonAjax($form, $form_state_object);
}

/**
 * Wrapper around D7's form state array, so D8 methods work.
 */
class FormState implements FormStateInterface {

  function __construct(&$form_state_array) {
    $this->form_state = &$form_state_array;

    // Initialize storage as an empty array so NestedArray works.
    if (!isset($this->form_state['storage'])) {
      $this->form_state['storage'] = [];
    }
  }

  public function set($address, $value) {
    NestedArray::setValue($this->form_state['storage'], $address, $value);
    // dsm($this->form_state['storage']);
  }

  public function get($address) {
    return NestedArray::getValue($this->form_state['storage'], $address);

  }

  public function getValues() {
    return $this->form_state['values'];
  }

  public function setRebuild() {
    $this->form_state['rebuild'] = TRUE;
  }

  public function getTriggeringElement() {
    return $this->form_state['triggering_element'];
  }

}

// Dummy interfaces so typehints work.
interface FormStateInterface {}

// Wrapper for drupal_html_id().
class HTML {

  static public function getUniqueId($id) {
    return drupal_html_id($id);
  }

}
