/**
 * @file
 * Scripting for administrative interfaces of Charts module.
 */
(function ($) {

Drupal.behaviors.chartsAdmin = {};
Drupal.behaviors.chartsAdmin.attach = function(context, settings) {
  // Change options based on the chart type selected.
  $(context).find('.form-radios.chart-type-radios').once('charts-axis-inverted', function() {

    // Manually attach collapsible fieldsets first.
    if (Drupal.behaviors.collapse) {
      Drupal.behaviors.collapse.attach(context, settings);
    }

    var xAxisLabel = $('fieldset.chart-xaxis .fieldset-title').html();
    var yAxisLabel = $('fieldset.chart-yaxis .fieldset-title').html();

    $(this).find('input:radio').change(function() {
      if ($(this).is(':checked')) {
        var groupingField = $(this).closest('form').find('.charts-grouping-field').val();

        // Flip X/Y axis fieldset labels for inverted chart types.
        if ($(this).attr('data-axis-inverted')) {
          $('fieldset.chart-xaxis .fieldset-title').html(yAxisLabel);
          $('fieldset.chart-xaxis .axis-inverted-show').closest('.form-item').show();
          $('fieldset.chart-xaxis .axis-inverted-hide').closest('.form-item').hide();
          $('fieldset.chart-yaxis .fieldset-title').html(xAxisLabel);
          $('fieldset.chart-yaxis .axis-inverted-show').closest('.form-item').show();
          $('fieldset.chart-yaxis .axis-inverted-hide').closest('.form-item').hide();
        }
        else {
          $('fieldset.chart-xaxis .fieldset-title').html(xAxisLabel);
          $('fieldset.chart-xaxis .axis-inverted-show').closest('.form-item').hide();
          $('fieldset.chart-xaxis .axis-inverted-hide').closest('.form-item').show();
          $('fieldset.chart-yaxis .fieldset-title').html(yAxisLabel);
          $('fieldset.chart-yaxis .axis-inverted-show').closest('.form-item').hide();
          $('fieldset.chart-yaxis .axis-inverted-hide').closest('.form-item').show();
        }

        // Show color options for single axis settings.
        if ($(this).attr('data-axis-single')) {
          $('fieldset.chart-xaxis').hide();
          $('fieldset.chart-yaxis').hide();
          $('th.chart-field-color, td.chart-field-color').hide();
          $('div.chart-colors').show();
        }
        else {
          $('fieldset.chart-xaxis').show();
          $('fieldset.chart-yaxis').show();
          if (groupingField) {
            $('th.chart-field-color, td.chart-field-color').hide();
            $('div.chart-colors').show();
          }
          else {
            $('th.chart-field-color, td.chart-field-color').show();
            $('div.chart-colors').hide();
          }
        }
      }
    });

    // Set the initial values.
    $(this).find('input:radio:checked').triggerHandler('change');
  });

  // React to the setting of a group field.
  $(context).find('.charts-grouping-field').once('charts-grouping', function() {
    $(this).change(function() {
      $form = $(this).closest('form');

      // Hide the entire grouping field row, since no settings are applicable.
      var value = $(this).val();
      $form.find('#chart-fields tr').show();
      if (value) {
        var $labelField = $form.find('.chart-label-field input[value="' + value + '"]');
        $labelField.closest('tr').hide();
        if ($labelField.is(':checked')) {
          $form.find('input[name="style_options[label_field]"][value=""]').attr('checked', 'checked').triggerHandler('change');
        }
      }
      // Restripe the table after hiding/showing rows.
      $form.find('#chart-fields tr:visible')
        .removeClass('odd even')
        .filter(':even').addClass('odd').end()
        .filter(':odd').addClass('even');

      // Recalculate shown color fields by triggering the chart type change.
      $form.find('.form-radios.chart-type-radios input:radio:checked').triggerHandler('change');
    }).triggerHandler('change');
  });

  // Disable the data checkbox when a field is set as a label.
  $(context).find('td.chart-label-field input').once('charts-axis-inverted', function() {
    var $radio = $(this);
    $radio.change(function() {
      if ($radio.is(':checked')) {
        $('.chart-data-field input').show();
        $('.chart-field-color input').show();
        $('input.chart-field-disabled').remove();
        $radio.closest('tr').find('.chart-data-field input').hide().after('<input type="checkbox" name="chart_field_disabled" disabled="disabled" class="chart-field-disabled" />');
        $radio.closest('tr').find('.chart-field-color input').hide();
      }
    });
    $radio.triggerHandler('change');
  });

};

})(jQuery);