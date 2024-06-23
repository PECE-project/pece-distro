/**
 * @file
 * Masonry script.
 */

(function($) {
    
  Drupal.behaviors.masonry = {
    attach: function(context, settings) {

      // Apply Masonry on the page.
      applyMasonry(false);

      // Hack for tabs: when the tab is open, it takes to reload Masonry.
      // @todo: what is the effect of this on performance ?
      $('a[data-toggle="tab"]').on('shown.bs.tab', function () {
        applyMasonry(true);
      });

      /**
       * Apply Masonry
       * @param forceInit (boolean)
       *   Force the initialisation of Masonry display (necessary hack for tabs).
       */
      function applyMasonry(forceInit) {

        // Iterate through all Masonry instances
        $.each(Drupal.settings.masonry, function (container, settings) {
          // Set container
          var $container = $(container);

          // Set options
          var $options = new Object();
          if (settings.item_selector) {
            $options.itemSelector = settings.item_selector;
          }
          if (settings.column_width) {
            if (settings.column_width_units == 'px') {
              $options.columnWidth = parseInt(settings.column_width);
            }
            else if (settings.column_width_units == '%') {
              $options.columnWidth = ($container.width() * (settings.column_width / 100)) - settings.gutter_width ;
            }
            else {
              $options.columnWidth = settings.column_width;
            }
          }
          if (settings.stamp) {
            $options.stamp = settings.stamp;
          }
          $options.gutter = settings.gutter_width;
          $options.isResizeBound = settings.resizable;
          $options.isFitWidth = settings.fit_width;
          if (settings.rtl) {
            $options.isOriginLeft = false;
          }
          if (settings.animated) {
            $options.transitionDuration = settings.animation_duration + 'ms';
          }
          else {
            $options.transitionDuration = 0;
          }
          if(settings.percent_position){
            $options.percentPosition = true;
          }

          // Apply Masonry to container
          if (settings.images_first) {
            $container.imagesLoaded(function () {
              if (forceInit) {
                $container.masonry($options);
              }
              else if ($container.hasClass('masonry-processed')) {
                $container.masonry('reloadItems').masonry('layout');
              }
              else {
                $container.once('masonry').masonry($options);
              }
            });
          }
          else {
            if (forceInit) {
              $container.masonry($options);
            }
            else if (!forceInit && $container.hasClass('masonry-processed')) {
              $container.masonry('reloadItems').masonry('layout');
            }
            else {
              $container.once('masonry').masonry($options);
            }
          }
        });
      }
    }
  };
})(jQuery);