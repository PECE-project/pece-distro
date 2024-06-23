<?php

use DrupalCodeBuilder\Task\Generate;
// Use DCB's copy of D8's NestedArray.
use DrupalCodeBuilder\Utility\NestedArray;

/**
 * Backport of ComponentFormBase from 8.x-3.x version.
 */
class ModuleBuilderComponentFormBase {

  /**
   * The complete data property info array for modules.
   *
   * This needs to be static because as an object property, it breaks the
   * serialization of the form object due to the closures it contains.
   *
   * @var array
   */
  static protected $componentDataInfo = [];

  /**
   * The DCB Generate Task handler.
   */
  protected $codeBuilderTaskHandlerGenerate;

  /**
   * Construct a new form object
   *
   * @param \DrupalCodeBuilder\Task\Generate $generate_task
   *   The Drupal Code Builder generate Task object.
   *   This needs to be injected so that submissions after an AJAX operation
   *   work (plus it's good for testing too).
   */
  function __construct(Generate $generate_task) {
    $this->codeBuilderTaskHandlerGenerate = $generate_task;
  }

  /**
   * {@inheritdoc}
   */
  // Not used for D7.
  public static function create(ContainerInterface $container) {
    // Get the component data info.
    try {
      $generate_task = $container->get('module_builder.drupal_code_builder')->getTask('Generate', 'module');
    }
    catch (SanityException $e) {
      // Switch the form class so we don't try to build the form without DCB
      // in working order. The ComponentBrokenForm form class handles the
      // exception to show a message to the user.
      return new ComponentBrokenForm($e);
    }

    return new static($generate_task);
  }

  /**
   * Gets the names of properties this form should show.
   *
   * @return string[]
   *   An array of property names.
   */
  // Not used for D7.
  protected function getFormComponentProperties() {
    // Get the list of component properties this section form uses from the
    // handler, which gets them from the entity type annotation.
    $component_entity_type_id = $this->entity->getEntityTypeId();
    $component_sections_handler = $this->entityTypeManager->getHandler($component_entity_type_id, 'component_sections');

    $operation = $this->getOperation();
    $component_properties_to_use = $component_sections_handler->getSectionFormComponentProperties($operation);
    return $component_properties_to_use;
  }

  /**
   * Title callback.
   *
   * @see \Drupal\module_builder\Routing\ComponentRouteProvider
   */
  // Not used for D7.
  public function title($module_builder_module, $op, $title) {
    return $this->t($title, [
      '%label' => $module_builder_module->label(),
    ]);
  }

  /**
   * {@inheritdoc}
   */
  // Not used for D7.
  public function form(array $form, FormStateInterface $form_state) {
    $form = parent::form($form, $form_state);

    $component_properties_to_use = $this->getFormComponentProperties();

    // Get the module entity's current values, to populate form defaults.
    $module = $this->entity;
    $this->moduleEntityData = $module->get('data') ?? [];
    // dsm($this->moduleEntityData);

    $form = $this->componentPropertiesForm($form, $form_state, $component_properties_to_use);

    // \Kint::$maxLevels = 0;
    // dsm($form);

    return $form;
  }

  /**
   * Add form elements for the specified component properties.
   *
   * @param $form
   *  The form array.
   * @param FormStateInterface $form_state
   *  The form state object.
   * @param $component_properties_to_use
   *  An array of property names from the component.
   *
   * @return
   *  The form array.
   */
  public function componentPropertiesForm($form, FormStateInterface $form_state) {
    $component_data_info = $this->codeBuilderTaskHandlerGenerate->getRootComponentDataInfo();
    // dsm($component_data_info);

    $component_data = [];

    /*
    // Sanity check the list of properties to use.
    // Iterate over our list of properties, so we are in control of the order.
    foreach ($component_properties_to_use as $property_name) {
      if (!isset($component_data_info[$property_name])) {
        drupal_set_message(t("The property '@name' is not defined in Drupal Code Builder. You should ensure you are using an up-to-date version.", [
          '@name' => $property_name,
        ]), 'error');

        continue;
      }
    }

    $component_properties_to_use = array_fill_keys($component_properties_to_use, TRUE);

    // Mark the ones we don't show here. We keep the full data array as the
    // prepare property step should always work on the full list of properties.
    // Note that this means our setting is NOT in control of the order!!!
    foreach ($component_data_info as $property_name => &$property_info) {
      if (!isset($component_properties_to_use[$property_name])) {
        $property_info['hidden'] = TRUE;
      }
    }
    */

    static::$componentDataInfo = $component_data_info;

    $form['data'] = $this->getCompomentElement($form_state, [], [], ['data']);

    // Set #tree on the data element.
    $form['data']['#tree'] = TRUE;

    return $form;
  }

  /**
   * Builds the form element for a component.
   *
   * This builds the root level form element, or an element for any part of
   * the property info array that is an array of properties. This is recursed
   * into by elementCompound().
   *
   * @param array $property_address
   *  The property address for the component. This is an array that gives the
   *  location of this component's properties list in the complete property info array
   *  in static::$componentDataInfo. For the root, this will be an empty array;
   *  for a child compound property this will be an address of the form
   *  parent->properties->child->properties.
   * @param array $value_address
   *  The value address for the form element to be created. This is similar to
   *  the property address, but will include items for compound property deltas.
   *  This ensures that buttons and item counts in form storage are unique for
   *  compound elements which are themselves children of multi-valued compound
   *  elements.
   * @param $form_value_address
   *  The form values address for the component. This is used to set the
   *  #parents property on the form element we create, so that the form values
   *  structure matches the original data structure. This is different again
   *  from the other two addresses, as it does not include a level for the
   *  'properties' array, but does include deltas.
   *
   * @return array
   *   The form array for the component's element.
   */
  private function getCompomentElement($form_state, $property_address, $value_address, $form_value_address) {
    $component_element = [];

    $properties = NestedArray::getValue(static::$componentDataInfo, $property_address);

    // TODO: should this be carried through? Check whether preparing a compound
    // property can set values in the array.
    $component_data = [];

    foreach ($properties as $property_name => &$property_info) {
      // Prepare the single property: get options, default value, etc.
      $this->codeBuilderTaskHandlerGenerate->prepareComponentDataProperty($property_name, $property_info, $component_data);

      // Skip the properties that we're not showing on this form section.
      if (!empty($property_info['hidden'])) {
        continue;
      }

      // Add the name of the current property to the address arrays.
      $property_component_address = $property_address;
      $property_component_address[] = $property_name;

      $property_value_address = $value_address;
      $property_value_address[] = $property_name;

      $property_form_value_address = $form_value_address;
      $property_form_value_address[] = $property_name;

      // Create a basic form element for the property.
      $property_element = [
        '#title' => $property_info['label'],
        '#required' => $property_info['required'],
        '#mb_property_address' => $property_component_address,
        '#mb_value_address' => $property_value_address,
        // Explicitly set this so we control the structure of the form
        // submission values. In particular, we don't want to have to pick data
        // out from the the structure the 'table' element would create.
        '#parents' => $property_form_value_address,
      ];

      if (isset($property_info['description'])) {
        $property_element['#description'] = $property_info['description'];
      }

      // Add description to properties that can get defaults filled in by
      // DCB in processing.
      if (!empty($property_info['process_default'])) {
        $property_element['#required'] = FALSE;
        $property_element['#description'] = (isset($property_element['#description']) ? $property_element['#description'] . ' ' : '')
          . t("Leave blank for a default value.");
      }

      // Determine the default value to present in the form element.
      // (Compound elements don't have a default value as they are just
      // containers, but we use the count of the array we get to determine how
      // many deltas to show.)
      $key_exists = NULL;
      // D7: there's no existing data, as modules are not stored.
      // $form_default_value = NestedArray::getValue($this->moduleEntityData, array_slice($property_form_value_address, 1), $key_exists);
      // If there is no value set in the module entity data, take the default
      // value that prepareComponentDataProperty() set.
      if (!$key_exists) {
        $form_default_value = $component_data[$property_name];

        if ($property_info['format'] == 'compound') {
          // Bit of a hack: for compound properties, zap the prepared default.
          // The problem is that this will cause a child element to appear in
          // the form, rather than starting with a zero delta.
          // This happens for example with the PHPUnit test component, where
          // the prepared default for the test_modules property tries to set a
          // module name derived from the test class name.
          // This will be fixed in DCB 3.3.x when we get the ability to do
          // defaults in JS.
          $form_default_value = [];
        }
      }

      // The type of the form element depends on the format of the component data
      // property.
      $format = $property_info['format'];
      $format_method = 'element' . ucfirst($format);
      if (!method_exists($this, $format_method)) {
        throw new \Exception("No method $format_method for $property_name.");
        continue;
      }

      $handling = $this->{$format_method}($property_element, $form_state, $property_info, $form_default_value);

      $property_form_value_address_key = implode(':', $property_form_value_address);
      $form_state->set(['element_handling', $property_form_value_address_key], $handling);

      $component_element[$property_name] = $property_element;
    }

    return $component_element;
  }

  /**
   * Set form element properties specific to array component properties.
   *
   * @param &$element
   *  The form element for the component property.
   * @param FormStateInterface $form_state
   *  The form state.
   * @param $property_info
   *  The info array for the component property.
   * @param $form_default_value
   *  The default value for the form element.
   *
   * @return string
   *  The handling type to be applied to this element's value on submit.
   */
  protected function elementArray(&$element, FormStateInterface $form_state, $property_info, $form_default_value) {
    if (isset($property_info['options'])) {
      if (isset($property_info['options_extra'])) {
        // Show an autocomplete textfield.
        // TODO: use Select or Other module for this when it has a stable
        // release.
        $element['#type'] = 'textfield';

        $element['#description'] = (isset($sub_element['#description']) ? $sub_element['#description'] . ' ' : '')
          . t("Enter multiple values separated with a comma.");

        $element['#autocomplete_route_name'] = 'module_builder.autocomplete';
        $element['#autocomplete_route_parameters'] = [
          'property_address' => implode(':', $element['#mb_property_address']),
        ];

        if ($form_default_value) {
          $form_default_value = implode(', ', $form_default_value);
        }

        $handling = 'autocomplete';
      }
      else {
        $element['#type'] = 'checkboxes';
        $element['#options'] = $property_info['options'];

        if (is_null($form_default_value)) {
          $form_default_value = [];
        }
        else {
          $form_default_value = array_combine($form_default_value, $form_default_value);
        }

        $handling = 'checkboxes';
      }
    }
    else {
      $element['#type'] = 'textarea';
      $element['#description'] = (string) $element['#description'] . ' ' . t("Enter one item per line.");

      $form_default_value = implode("\n", $form_default_value);

      $handling = 'textarea';
    }

    $element['#default_value'] = $form_default_value;

    return $handling;
  }

  /**
   * Set form element properties specific to boolean component properties.
   *
   * @param &$element
   *  The form element for the component property.
   * @param FormStateInterface $form_state
   *  The form state.
   * @param $property_info
   *  The info array for the component property.
   * @param $form_default_value
   *  The default value for the form element.
   *
   * @return string
   *  The handling type to be applied to this element's value on submit.
   */
  protected function elementBoolean(&$element, FormStateInterface $form_state, $property_info, $form_default_value) {
    $element['#type'] = 'checkbox';

    $element['#default_value'] = $form_default_value;

    return 'checkbox';
  }

  /**
   * Set form element properties specific to compound component properties.
   *
   * @param &$element
   *  The form element for the component property.
   * @param FormStateInterface $form_state
   *  The form state.
   * @param $property_info
   *  The info array for the component property.
   * @param $form_default_value
   *  The default value for the form element.
   *
   * @return string
   *  The handling type to be applied to this element's value on submit.
   */
  protected function elementCompound(&$element, FormStateInterface $form_state, $property_info, $form_default_value) {
    // A compound property shows a fieldset element, for which we recurse and
    // show another component.
    // D7: use fieldset instead of details element.
    $element['#type'] = 'fieldset';
    $element['#collapsed'] = FALSE;

    // Figure out how many items to show.
    // If we're reloading the form in response to the 'add more' button, then
    // form storage dictates the item count.
    // If there's nothing set in form storage yet, it's the first time we're
    // here and the number of items in the entity tells us how many items to
    // show in the form.
    // Finally, if that's empty, then show no items, just a button to add one.
    $item_count = static::getCompoundPropertyItemCount($form_state, $element['#mb_value_address']);
    if (is_null($item_count)) {
      $item_count = count($form_default_value);
      static::setCompoundPropertyItemCount($form_state, $element['#mb_value_address'], $item_count);
    }
    if (empty($item_count)) {
      $item_count = 0;
      static::setCompoundPropertyItemCount($form_state, $element['#mb_value_address'], $item_count);
    }

    // Property cardinality overrides anything else.
    if (isset($property_info['cardinality'])) {
      $item_count = min($item_count, $property_info['cardinality']);

      if ($item_count == $property_info['cardinality']) {
        // We're at the maximum item count.
        $add_more = FALSE;
      }
      else {
        // We're not yet at the cardinality: we can add more.
        $add_more = TRUE;
      }
    }
    else {
      // Unlimited cardinality: can always add more.
      $add_more = TRUE;
    }

    // Set up a wrapper for AJAX.
    $wrapper_id = Html::getUniqueId(implode('-', $element['#mb_value_address']) . '-add-more-wrapper');
    // TODO - use   '#type' => 'container',?
    $element['#prefix'] = '<div id="' . $wrapper_id . '">';
    $element['#suffix'] = '</div>';

    // Show the items in a table. This is single-column, with all child
    // properties in the one cell, but we just want the striping for visual
    // clarity.
    // D7: this won't work, but theming a table inside a form in D7 is just too
    // much faff. TODO: add borders or something?
    $element['table'] = array(
      '#type' => 'table',
    );

    // The address in the properties array to find this component's properties
    // list.
    $component_properties_address = $element['#mb_property_address'];
    $component_properties_address[] = 'properties';

    $component_value_address = $element['#mb_value_address'];
    $component_value_address[] = 'properties';

    $property_form_value_address = $element['#parents'];

    for ($delta = 0; $delta < $item_count; $delta++) {
      $row = [];

      $delta_value_address = $component_value_address;
      $delta_value_address[] = $delta;

      $delta_form_value_address = $property_form_value_address;
      $delta_form_value_address[] = $delta;

      // Put all the properties into a single cell so it's a 1-column table.
      // TODO: WTF NO STRIPING IN SEVEN THEME???
      $delta_component_element = $this->getCompomentElement($form_state, $component_properties_address, $delta_value_address, $delta_form_value_address, []);

      $row['row'] = $delta_component_element;
      $element['table'][$delta] = $row;
    }

    if ($add_more) {
      // Show a button to add items, if they can be added.
      $button_text = ($item_count == 0)
        ? t('Add a @label item', [
          '@label' => strtolower($property_info['label']),
        ])
        : t('Add another @label item', [
          '@label' => strtolower($property_info['label']),
        ]);

      $element['actions']['add'] = array(
        '#type' => 'submit',
        // This allows FormAPI to figure out which button is the triggering
        // element. The name must be unique across all buttons in the form,
        // otherwise, the first matching name will be taken by FormAPI as being
        // the button that was clicked, with unexpected results.
        // See \Drupal\Core\Form\FormBuilder::elementTriggeredScriptedSubmission().
        '#name' => implode(':', $element['#mb_value_address']) . '_add_more',
        '#value' => $button_text,
        '#limit_validation_errors' => [],
        '#submit' => ['mb_addItemSubmit'],
        '#ajax' => array(
          'callback' => 'mb_itemButtonAjax',
          'wrapper' => $wrapper_id,
          'effect' => 'fade',
        ),
      );
    }

    if ($item_count > 0) {
      $element['actions']['remove'] = [
        '#type' => 'submit',
        '#name' => implode(':', $element['#mb_value_address']) . '_remove_item',
        '#value' => t('Remove last item'),
        '#limit_validation_errors' => [],
        '#submit' => ['mb_removeItemSubmit'],
        '#ajax' => array(
          'callback' => 'mb_itemButtonAjax',
          'wrapper' => $wrapper_id,
          'effect' => 'fade',
        ),
      ];
    }

    return 'compound';
  }

  /**
   * Submission handler for the "Add another item" buttons.
   */
  public static function addItemSubmit(array $form, FormStateInterface $form_state) {
    $button = $form_state->getTriggeringElement();

    // Go up in the form, to the widgets container.
    $element = NestedArray::getValue($form, array_slice($button['#array_parents'], 0, -2));

    // Increment the item count for this element.
    $property_address = $element['#mb_value_address'];
    static::incrementCompoundPropertyItemCount($form_state, $property_address);

    $form_state->setRebuild();
  }

  /**
   * Submission handler for the "Remove item" buttons.
   */
  public static function removeItemSubmit(array $form, FormStateInterface $form_state) {
    $button = $form_state->getTriggeringElement();

    // Go up in the form, to the widgets container.
    $element = NestedArray::getValue($form, array_slice($button['#array_parents'], 0, -2));

    // Increment the item count for this element.
    $property_address = $element['#mb_value_address'];
    static::decrementCompoundPropertyItemCount($form_state, $property_address);

    $form_state->setRebuild();
  }

  /**
   * Ajax callback for the item count buttons.
   *
   * This returns the new page content to replace the page content made obsolete
   * by the form submission.
   */
  public static function itemButtonAjax(array $form, FormStateInterface $form_state) {
    $button = $form_state->getTriggeringElement();

    // Go up in the form, to the widgets container.
    $button_array_parents = $button['#array_parents'];
    $widgets_container_parents = array_slice($button_array_parents, 0, -2);

    $element = NestedArray::getValue($form, $widgets_container_parents);

    return $element;
  }

  /**
   * Set form element properties specific to array component properties.
   *
   * @param &$element
   *  The form element for the component property.
   * @param FormStateInterface $form_state
   *  The form state.
   * @param $property_info
   *  The info array for the component property.
   * @param $form_default_value
   *  The default value for the form element.
   *
   * @return string
   *  The handling type to be applied to this element's value on submit.
   */
  protected function elementString(&$element, FormStateInterface $form_state, $property_info, $form_default_value) {
    if (isset($property_info['options'])) {
      $element['#type'] = 'select';

      $options = [];

      $element['#options'] = $property_info['options'];
      $element['#empty_value'] = '';

      if (empty($form_default_value)) {
        $form_default_value = '';
      }

      $handling = 'select';
    }
    else {
      $element['#type'] = 'textfield';

      $handling = 'textfield';
    }

    $element['#default_value'] = $form_default_value;

    return $handling;
  }

  /**
   * Returns an array of supported actions for the current entity form.
   */
  protected function actions(array $form, FormStateInterface $form_state) {
    // TODO: remove #mb_action, use #name instead.
    $actions['submit'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Save'),
      '#dropbutton' => 'mb',
      // Still no way to get a button's name, apparently?
      '#mb_action' => 'submit',
      '#submit' => array('::submitForm', '::save'),
    );
    if ($this->getNextLink() != 'generate-form') {
      $actions['submit_next'] = array(
        '#type' => 'submit',
        '#value' => $this->t('Save and go to next page'),
        '#dropbutton' => 'mb',
        '#mb_action' => 'submit_next',
        '#submit' => array('::submitForm', '::save'),
      );
    }
    $actions['submit_generate'] = array(
      '#type' => 'submit',
      '#value' => $this->t('Save and generate code'),
      '#dropbutton' => 'mb',
      '#mb_action' => 'submit_generate',
      '#submit' => array('::submitForm', '::save'),
    );

    return $actions;
  }

  /**
   * Copies top-level form values to entity properties
   *
   * This should not change existing entity properties that are not being edited
   * by this form.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   The entity the current form should operate upon.
   * @param array $form
   *   A nested array of form elements comprising the form.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   The current state of the form.
   */
  public function copyFormValuesToEntity(/*EntityInterface $entity, */ array $form, FormStateInterface $form_state) {
    $values = $form_state->getValues();

      /*
    if ($this->entity instanceof EntityWithPluginCollectionInterface) {
      // Do not manually update values represented by plugin collections.
      $values = array_diff_key($values, $this->entity->getPluginCollections());
    }

    if ($this->entity->isNew()) {
      $data = [];
    }
    else {
      // Add to the existing entity data array.
      $data = $entity->get('data');
    }
    */
    $data = [];

    foreach ($values['data'] as $key => $value) {
      // TODO kill on D8 too!
        // $form_element = $form['data'][$key];

      $value_address = ['data', $key];
      $value = $this->getFormElementValue($value_address, $value, $form_state);

      if (empty($value)) {
        unset($data[$key]);
      }
      else {
        $data[$key] = $value;
      }
    }

    //$entity->set('data', $data);
    return $data;
  }

  /**
   * Get the value for a property from the form values.
   *
   * This performs various processing depending on the form element type and the
   * property format:
   *  - explode textarea values
   *  - filter checkboxes and store only the keys
   *  - recurse into compound properties
   * The form build process leaves instructions for how to handle each value in
   * the 'element_handling' form state setting, so that here we don't need to
   * repeat the logic based on property info. Furthermore, we can't put this
   * a property info array into form state storage, because it contains closures,
   * which don't survive the serialization process in the database, and so the
   * property info would need to be run through DCB's preparation process all
   * over again.
   *
   * @param array $value_address
   *  The address array of the value in the form state values array. The final
   *  element of this is name of the property and the form element.
   * @param $value
   *  The incoming form value from the form element for this property.
   * @param \Drupal\Core\Form\FormStateInterface $form_state
   *   The current state of the form.
   *
   * @return
   *  The processed value.
   */
  protected function getFormElementValue($value_address, $value, FormStateInterface $form_state) {
    // Retrieve the handling type from the form state.
    $property_form_value_address_key = implode(':', $value_address);
    $handling = $form_state->get(['element_handling', $property_form_value_address_key]);

    switch ($handling) {
      case 'textarea':
        // Array format, without options: textarea.
        // Only explode a non-empty string, as explode() will turn '' into an
        // array!
        if (!empty($value)) {
          // Need to split on any whitespace rather than "\n" because for FKW
          // reasons, linebreaks come back through POST as Windows-style "\r\n".
          $value = preg_split("@\s+@", $value);
        }
        break;

      case 'autocomplete':
        // Array format, with extra options: textfield with autocomplete.
        // Only explode a non-empty string, as explode() will turn '' into an
        // array!
        if (!empty($value)) {
          // Textfield with autocomplete.
          $value = preg_split("@,\s*@", $value);
        }
        break;

      case 'checkboxes':
        // Array format, with options: checkboxes.
        // Filter out empty values. (FormAPI *still* doesn't do this???)
        $value = array_filter($value);
        // Don't store values also in the keys, as some of these have dots in
        // them, which ConfigAPI doesn't allow.
        $value = array_keys($value);
        break;

      case 'compound':
        // Remove the item count buttons from the values.
        unset($value['actions']);
        unset($value['table']);

        foreach ($value as $delta => $item_value) {
          $delta_value_address = $value_address;
          $delta_value_address[] = $delta;

          // Recurse into the child property values.
          foreach ($item_value as $child_key => $child_value) {
            $delta_child_value_address = $delta_value_address;
            $delta_child_value_address[] = $child_key;

            $value[$delta][$child_key] = $this->getFormElementValue($delta_child_value_address, $child_value, $form_state);
          }
        }
        break;

      case 'checkbox':
      case 'select':
      case 'textfield':
        // Nothing to do in these cases: $value is fine as it is.
        break;

      default:
        throw new \Exception("Unknown handling type: {$handling}.");
    }

    return $value;
  }

  // TODO: validate compound properties on submit:
  // - check for required properties
  // - filter out items which are completely empty.

  /**
   * {@inheritdoc}
   */
  public function save(array $form, FormStateInterface $form_state) {
    $is_new = $this->entity->isNew();

    $module = $this->entity;

    $status = $module->save();

    if ($status) {
      // Setting the success message.
      drupal_set_message($this->t('Saved the module: @name.', array(
        '@name' => $module->name,
      )));
    }
    else {
      drupal_set_message($this->t('The @name module was not saved.', array(
        '@name' => $module->name,
      )));
    }

    // Optionally advance to next tab or go to the generate page.
    $element = $form_state->getTriggeringElement();
    switch ($element['#mb_action']) {
      case 'submit':
        $operation = $this->getOperation();
        // For a new module, we need to redirect to its edit form, as staying
        // put would leave on the add form.
        if ($operation == 'add') {
          $operation = 'edit';
        }
        // For an existing module, we also redirect so that changing the machine
        // name of the module goes to the new URL.
        $url = $module->toUrl($operation . '-form');
        $form_state->setRedirectUrl($url);
        break;
      case 'submit_next':
        $next_link = $this->getNextLink();
        $url = $module->toUrl($next_link);
        $form_state->setRedirectUrl($url);
        break;
      case 'submit_generate':
        $url = $module->toUrl('generate-form');
        $form_state->setRedirectUrl($url);
        break;
    }
  }

  /**
   * Get the next entity link after the one for the current form.
   *
   * @return
   *  The name of an entity link.
   */
  protected function getNextLink() {
    // Probably a more elegant way of figuring out where we currently are
    // with routes maybe?
    $operation = $this->getOperation();

    // Special case for add and edit forms.
    if ($operation == 'default' || $operation == 'edit') {
      $operation = 'name';
    }

    $handler_class = $this->entityTypeManager->getHandler('module_builder_module', 'component_sections');
    $form_ops = $handler_class->getFormOperations();

    // Add in the 'name' operation, as the handler doesn't return it.
    $form_ops = array_merge(['name'], $form_ops);

    $index = array_search($operation, $form_ops);

    return $form_ops[$index + 1] . '-form';
  }

  /**
   * Gets the item count for a compound property from the form state.
   *
   * @param $form_state
   *   The form state.
   * @param array $property_address
   *   The value address.
   *
   * @return int
   *   The item count.
   */
  protected static function getCompoundPropertyItemCount($form_state, $property_address) {
    $property_address_key = implode(':', $property_address);

    $item_count = $form_state->get(['item_count', $property_address_key]);
    return $item_count;
  }

  /**
   * Sets the item count for a compound property from the form state.
   *
   * @param $form_state
   *   The form state.
   * @param array $property_address
   *   The value address.
   * @param $item_count
   *   The item count.
   */
  protected static function setCompoundPropertyItemCount($form_state, $property_address, $item_count) {
    $property_address_key = implode(':', $property_address);

    $form_state->set(['item_count', $property_address_key], $item_count);
  }

  /**
   * Increments the item count for a compound property from the form state.
   *
   * @param $form_state
   *   The form state.
   * @param array $property_address
   *   The value address.
   */
  protected static function incrementCompoundPropertyItemCount($form_state, $property_address) {
    $item_count = static::getCompoundPropertyItemCount($form_state, $property_address);
    $item_count++;

    static::setCompoundPropertyItemCount($form_state, $property_address, $item_count);
  }

  /**
   * Decrements the item count for a compound property from the form state.
   *
   * @param $form_state
   *   The form state.
   * @param array $property_address
   *   The value address.
   */
  protected static function decrementCompoundPropertyItemCount($form_state, $property_address) {
    $item_count = static::getCompoundPropertyItemCount($form_state, $property_address);
    $item_count--;

    static::setCompoundPropertyItemCount($form_state, $property_address, $item_count);
  }

}
