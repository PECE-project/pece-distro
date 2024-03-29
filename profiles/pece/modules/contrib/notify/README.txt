NOTIFY README.txt
=================


CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Recommended modules
* Installation
* Permissions
* Configuration
  - Administration form
  - User's settings
* EU GDPR Considerations
* Troubleshooting
* Testing


INTRODUCTION
------------

Notify is a simple, lightweight module for sending email
notifications about new content and comments posted on a Drupal
web site.

* For a full description of the module, visit the project page:
  https://www.drupal.org/project/notify

* For more documentation about its use, visit the documentation page:
  https://www.drupal.org/docs/7/modules/notify

* To submit bug reports and feature suggestions, or to track changes,
  visit: https://www.drupal.org/project/issues/notify

If you enable node revisions (https://www.drupal.org/node/320614), the
notification email will also mention the name of the last person to
edit the node.


REQUIREMENTS
------------

This module requires a supported version of Drupal and cron to be
running.


RECOMMENDED MODULES
-------------------

* Advanced help hint (https://www.drupal.org/project/advanced_help_hint)
  Will link standard help text to online help and advanced help.

* Advanced help (https://www.drupal.org/project/advanced_help)
  When this module is enabled the administrator will have access to
  more extensive help.


INSTALLATION
------------

1. Install as you would normally install a contributed Drupal 7
   module. See: Installing modules
   (https://www.drupal.org/documentation/install/modules-themes/modules-7)
   for further information.

2. Check if you need to run the update script by visting the Status
   Report.

3. Modify permissions on the People » Permissions page.

   To adminster the notify general settings, default settings and
   users grant the permission "administer notify".

   To adminster the queue (flush, truncate and skip flags), grant the
   permission "administer notify queue".

   To set the notification checkbox default on new user registration
   form, or let new users opt in for notifications during
   registration, you must grant the anonymous user the right to
   "access notify".  To allow users to control their own notification
   settings (recommended) you must also grant authenticated users the
   right to "access notify".

4. Configure the other general notification settings.

   See the "Administration form" section below for details.

Notify will not automatically subscribe anyone to notifications upon
installation.  Before anyone is subscribed, no notifications will be
sent.


PERMISSIONS
-----------

To set up permissions for Notify navigate to: Administration » People
» Permissions.

There are four permissions:

1. access notify: to subscribe to and receive notifications when there is new content
2. administer notify: to administer general notify settings, default settings, and users
3. administer notify queue: to administer the notify queue operations
4. administer notify skip flags: to administer the notify skip flags



CONFIGURATION
-------------

Administration form
-------------------

The administrative interface is at: Administration » Configuration »
People » Notification settings.

There are five tabs:

1. Settings: All the main options for this module.
2. Queue: Operations on the notification queue.
3. Skip flags: Inspect the notification queue and flag postings to skip.
4. Defaults: Default settings for new users.
5. Users: Review and alter per-user settings.


Settings

The Settings tab allow you to configure how the module shall work.

You can specify how often notifications are sent, the hour to send
them (if the frequency is one day or greater), the number of failed
sends after which notifications are disabled, and the maximum number
of users to process out per cron run.

When setting how often notifications are sent, note that email
updates can only happen as frequently as the cron is set to run.

To reset the count of failed sends to zero, look at the notification
settings in the user's profile and save it by pressing "Save settings"
(there is no need to change anything).

If you check "Include updated posts in notifications", any change to a
node or content will cause it to be included in the next notification.
Note that even minor changes, such as correcting a trivial typo or
setting or unsetting the "sticky" attribute for the node will flag it
as updated, so use this option with caution, or in combination with
skip flags (see below) in order to avoid excess notificatons.

If you check "Exclude contents from unverified authors from user
notifications", notify will not notify about postings from unverified
(i.e. anonymous) authors.  You need only care about this setting if
you permit postings from anonymous authors.  Even if you have spam
protection in the shape of CAPTCHA or other measures, you may
experience that some spammers still manage to post contents on your
site.  By checking this setting, you will at least save your
subscribers from being notified about spam.  As with most of these
settings, it doesn't apply to administrators. Even when checked
administrators will be notified, in order to intervene and delete the
spam before it gets much exposure.  Note that if you check this
setting, there is currently no way to keep track of the content that
is excluded due this setting.  If you use it, your users will never
receive any notification email about new content from unverified
authors.  That's not a bug, it is a feature.

If you check "Administrators shall be notified about unpublished
content", users belonging to roles with the "administer nodes" and
"administer comments" permissions granted will receive notifications
about unpublished content.  This is mainly to make the module useful
to manage moderation queues.

If you've set up a multilingual site, there should also be three radio
buttons that allow you to filter notifications about new nodes against
the user's language setting (it may be set by editing the user
profile).  The first setting ("All contents") will notify a user about
all new content on the site. If a piece of subscribed contents exists
in more than one language, all versions will be notified about.  The
setting "Contents in the user's preferred language + contents not yet
translated" will notify about content in the user's preferred language
and also about content that is in some other language if no
translation of it exists. The last setting, "Only contents in the
user's preferred language", will only notify about subscribed contents
in the user's preferred language.  However, please note that
subscribed contents that are marked as "language neutral" will always
be included in notifications.  The multilingual settings do not apply
to administrators.  Administrators will always be notified about all
new contents.  Note that if you use the second setting, contents that
is not in the user's preferred language will be excluded from the
notification if some translation of exists, even if that translation
is not to the user's preferred language.  To avoid this having
unexpected effects, when you provide translation of a node, you should
translate it to all langauages the site supports.

The "Watchdog log level" setting lets you specify how much to log.
The setting "All" will make a log record of every notification mail
sent.  The setting "Failures+Summary" will only log failed
notification attempts. It will also insert a summary of how many sent
and how many failures at the end of each batch.  The "Failures"
setting will omit the summary.  The "Nothing" setting will turn off
logging for Notify.

The "Weight of notification field in user registration form" setting
lets you specify the weight that determines the position of the
notification field group when it appears in the user registration
form.  The number is relative to the row weights that can be inspected
on Administer » Configuration » People » Account settings.  Pick a
higher (heavier) weight to make the field group positoned below a
specific field, and vice versa.


Queue

The Queue tab gives access to notification queue operatons and the
notification queue status panel.

The radio buttons below the heading "Notification queue operations"
has the following meanings:

 - Send batch now: Force sending a notification batch without waiting
   for the next cron run.  Note that if the number of notifications
   queue exceeds the maximum number of users to process out per cron
   run, only the maximum number of users are processed.  The rest will
   be queued for the next cron run or the next manual send batch
   (whatever happens first).

 - Truncate queue: Truncate the queue of pending notifications without
   sending out any notifications.

 - Override timestamp: Change the value of the last notification
   timestamp.  To resend postings that has already been sent, set pick
   a value before the oldest posting you want to resend.

The text field "Last notification timestamp" can be used to override
the value of the last notification timestamp.  This value is only used
to override of the operation "Override timestamp" is selected.

The status panel at the bottom of the page gives the administrator a
rough overview of the current state of the notification queue, as well
as the Default MailSystem. The main use of the status panel is to
provide information useful for debugging.


Skip flags

The Skip flags tab will show a list of all the postings that are
candidates for being sent in the next notification.  Each has a
checkbox that can be checked to exclude the posting from all
notification emails, including the one sent to the administrator.


Defaults

The checkbox under "Notification default for new users" is used as the
default value for the notification master switch on the new user
registration.  Note that this setting has no effect unless you grant
the anonymous user the right to access notify.

The "Initial settings panel" let you set up the initial settings that
will apply to new users registering, and to users that are enrolled in
notifications with batch subscription. These settings have no effect
on users that already have the master switch set to "Enabled".

The final panel under the Settings tab let you set up notification
subscriptions by node type.  Having nothing checked defaults to making
all content types available for subscription.


Users

The Users tab is to review and alter per-user settings for those users
that have the master switch for notifications set to "Enabled".

If you tick the box "Subscribe all users", all non-blocked users that
do not already subscribe to notifications will be subscribed with the
initial settings set under the default tab.


User's settings
---------------

To manage your own notification preferences, click on the
"Notification settings" tab on your "My account" page.

The "Master switch" overrides all other settings for Notify. You can
use it to suspend notifications without having to disturb any of your
settings under "Detailed settings" and "Subscriptions".

The "Detailed settings" determine what is included in each
notification.  You can turn on or off notification of new content and
new comments.

The "Subscriptions" panel allow each user to manage individual
notifications by content type.

Note that for users with the permission "administer notify queue", it
is possible to subscribe to content types that is not generally
available for subscription.  This allows administrators to monitor all
new content on the site, without making it subscribable for
non-administrators.


EU GDPR CONSIDERATIONS
----------------------

The body of a node may contain personal data. Email is not always sent
encrypted (unless the site's MTA is set up with Enforce TSL). Also:
Some European users use mail systems located outside the EU
(e.g. GMail) that are subject to laws like the US Patriot Act that
overrule the protections and freedoms EU citizens should enjoy. The
only way to make this module GDPR compliant is to make sure that no
personal data is sent in unencrypted emails or in emails processed by
email systems processed outside of the EU.  To minimize the risk of
this module making a site non-compliant with the EU GDPR,
notifications will never include the body of a node.


TROUBLESHOOTING
---------------

* If Notify does not send out any notification emails, first check
  that Drupal can send email otherwise (e.g. request a password reset
  email).  If this does not work, the problem is with your site's
  email configuration, not Notify.

* If inbound links in the notification email is rendered as
  http://default, you may need to set the $base_url in your
  settings.php file. Examples for how to do this are provided in
  settings.php.

* If your site is multilingual, and your problem is that Notify is not
  sending all notifications to all subscribed users, the first thing
  to try is to visit the multilingual settings and turn off any
  language filter (i.e. set Notify to notify about "All contents").
  If changing this setting makes a difference, you need to review your
  multiligual settings for nodes, users and Notify and make sure that
  they match.

* If Notify makes the site crash, and you have the core PHP Filter
  module enabled, nodes which include bad PHP code will break your
  site when they're processed by Notify. Please see the following
  issue for further details: https://www.drupal.org/node/146521. If
  this happens, you may try to disable the PHP Filter module.

If the above does not help you, to file bug reports, use the issue
queue linked to from the Notify project page.


TESTING
-------

The test suite is removed in this tagged release. It creates problems,
for details, see this issue:
https://www.drupal.org/project/notify/issues/3246817
