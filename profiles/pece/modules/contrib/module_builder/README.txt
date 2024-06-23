Module Builder
==============

Welcome to Module Builder!

Module Builder is a system that simplifies the process of creating code, by
creating files complete with scaffold code that can be filled in.

For example, for generating a custom module, simply fill out the form, select
the hooks you want and the script will automatically generate a skeleton module
file for you, along with PHPDoc comments and function definitions. This saves
you the trouble of looking at api.drupal.org 50 times a day to remember what
arguments and what order different hooks use. Score one for laziness! ;)

What Module Builder can create
------------------------------

Module builder can generate the following for a module:
- code files, containing hook implementations
- info file
- api.php file
- README file
- test case classes

Furthermore, complex subcomponents can generate multiple code elements:
- an admin settings form with form builder functions and an admin permission
- hook_menu() items
- permission names add the scaffold for the permission definition

Installing module builder
-------------------------

1. Place this folder into your modules/ directory like you would any
   other module.
2. Download and enable Libraries module.
3. Download the Drupal Code Builder library from
   https://github.com/drupal-code-builder/drupal-code-builder. You need a 3.x.x
   version, NOT a version 4.x.x.
4. Place Drupal Code Builder into sites/all/libraries (or similar).
5. Enable Module Builder from Administration > Modules.
6. At Administration › Configuration › Development › Module Builder › Settings,
   specify the path to save the hook documentation files.
7. At Administration › Configuration › Development › Module Builder › Update
   hooks, run the code analysis which allows Module Builder to detect hooks in
   your site's codebase.

You should run the code analysis again when you install new modules, or add
new custom modules, so that Module Builder knows about their hooks.

Optionally, you can:

- Create custom function declaration template file(s) if you don't like the
  default output.
- Create your own hook groupings if you don't like the default ones.

Using Module Builder
--------------------

1. Go to Administration › Modules › Module Builder.
   (Note: you will require 'access module builder' privileges to see this link.)
2. Enter a module name, description, and so on.
3. Select from one of the available hook groupings to automatically
   select hook choices for you, or expand the fieldsets and choose
   hooks individually.
4. Click the "Submit" button and watch your module's code generated
   before your eyes! ;)
5. Copy and paste the code into a files called <your_module>.module,
   <your_module>.info and <your_module>.install and save them to
   a <your_module> directory under one of the modules directories.
6. Start customizing it to your needs; most of the tedious work is
   already done for you! ;)

Todo/wishlist
-------------

* Maybe some nicer theming/swooshy boxes on hook descriptions or
  something to make the form look nicer/less cluttered
* I would like to add the option to import help text from a Drupal.org
  handbook page, to help encourage authors to write standardized
  documentation in http://www.drupal.org/handbook/modules/

Known issues
------------

* Can't set default values in PHP 5 for some strange reason
* Fieldsets in Opera look mondo bizarr-o

CONTRIBUTORS
------------
* Owen Barton (grugnog2), Chad Phillips (hunmonk), and Chris Johnson
  (chrisxj) for initial brainstorming stuff @ OSCMS in Vancouver
* Jeff Robbins for the nice mockup to work from and some great suggestions
* Karthik/Zen/|gatsby| for helping debug some hairy Forms API issues
* Steven Wittens and David Carrington for their nice JS checkbox magic
* jsloan for the excellent "automatically generate module file" feature
* Folks who have submitted bug reports and given encouragement, thank you
  so much! :)
