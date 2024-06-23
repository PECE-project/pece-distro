Drupal.settings.spotlight_settings = Drupal.settings.spotlight_settings || {};

(function ($) {

  /**
   * Form behavior for Spotlight
   */
  Drupal.behaviors.panopolySpotlight = {
    attach: function (context, settings) {
      if ($('.field-name-field-basic-spotlight-items', context).length) {
        $('.field-name-field-basic-spotlight-items', context).each(function() {
          var rotation_time = $(this).find('.panopoly-spotlight-buttons-wrapper').data('rotation-time'),
              $widget = $(this),
              $slides = $widget.find('.panopoly-spotlight'),
              $controls = $widget.find('.panopoly-spotlight-buttons-wrapper li'),
              current = 0,
              timer = null,
              timer_update = false;

          function start() {
            if (timer === null) {
              timer = setTimeout(rotate, rotation_time);
            }
          }

          function stop() {
            if (timer !== null) {
              clearTimeout(timer);
              timer = null;
            }
          }

          function rotate() {
            // Increment the current slide.
            current++;
            if (current >= $controls.length) {
              current = 0;
            }

            // Click the control for the next slide.
            timer_update = true;
            $controls.eq(current).children('a').trigger('click.panopoly-widgets-spotlight');
            timer_update = false;
          }

          // Navigation is hidden by default, display it if JavaScript is enabled.
          $widget.find('.panopoly-spotlight-buttons-wrapper').css('display', 'block');

          // Hide all the slides but the first one.
          $slides.hide();
          $slides.eq(0).show();
          $controls.eq(0).addClass('active');

          // Bind the event for the slide numbers.
          $controls.once('panopoly-spotlight').children('a').bind('click.panopoly-widgets-spotlight', function (event) {
            var selector = $(this).attr('href');
            if (selector.indexOf('#') === 0) {
              event.preventDefault();

              // Mark the slide number as active.
              $controls.removeClass('active');
              $(this).parent().addClass('active');

              // Hide all slides but the selected one.
              $slides.hide();
              $slides.filter(selector).show();

              if (timer_update) {
                // Reset the timer so we advance to the next slide.
                stop();
                start();
              }
              else {
                // Set the new slide to current.
                var match = selector.match(/^#panopoly-spotlight-(\d+)$/)
                if (match) {
                  current = parseInt(match[1]);
                }

                // Stop the timer (if it's running).
                if (timer !== null) {
                  $widget.find('.panopoly-spotlight-pause-play').trigger('click.panopoly-widgets-spotlight');
                }
              }

              return false;
            }
          });

          // Bind events to all the extra buttons.
          $widget.find('.panopoly-spotlight-pause-play').once('panopoly-spotlight').bind('click.panopoly-widgets-spotlight', function(event) {
            event.preventDefault();
            if ($(this).hasClass('paused')) {
              start();
              $(this).text(Drupal.t('Pause'));
              $(this).removeClass('paused');
            }
            else {
              stop();
              $(this).text(Drupal.t('Play'));
              $(this).addClass('paused');
            }
          });
          if ($widget.find('.panopoly-spotlight-previous').length && $widget.find('.panopoly-spotlight-next').length) {
            $widget.find('.panopoly-spotlight-previous').once('panopoly-spotlight').bind('click.panopoly-widgets-spotlight', function (event) {
              event.preventDefault();
              $widget.find('.panopoly-spotlight-pause-play:not(.paused)').trigger('click.panopoly-widgets-spotlight');
              var activeControl = $($controls.filter('.active'));

              if (activeControl.prev().length != 0) {
                activeControl.prev().children('a').trigger('click.panopoly-widgets-spotlight');
              }
              else {
                $controls.last().children('a').trigger('click.panopoly-widgets-spotlight');
              }
            });
            $widget.find('.panopoly-spotlight-next').once('panopoly-spotlight').bind('click.panopoly-widgets-spotlight', function (event) {
              event.preventDefault();
              $widget.find('.panopoly-spotlight-pause-play:not(.paused)').trigger('click.panopoly-widgets-spotlight');
              var activeControl = $($controls.filter('.active'));

              if (activeControl.next().length != 0) {
                activeControl.next().children('a').trigger('click.panopoly-widgets-spotlight');
              }
              else {
                $controls.first().children('a').trigger('click.panopoly-widgets-spotlight');
              }
            });
          }

          // Detect focus on any links inside of slides and pause, until focus
          // transitions away.
          $slides.find('a')
            .focus(function () {
              stop();
            })
            .blur(function () {
              if (!$widget.find('.panopoly-spotlight-pause-play').hasClass('paused')) {
                start();
              }
            });

          var prefers_reduced_motion = window.matchMedia("(prefers-reduced-motion: reduce)");
          if (!prefers_reduced_motion || !prefers_reduced_motion.matches) {
            start()
          }
          else {
            $widget.find('.panopoly-spotlight-pause-play')
              .text(Drupal.t('Play'))
              .addClass('paused');
          }
        });
      }
    }
  };

})(jQuery);
