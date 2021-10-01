
Menu translation (Node) allows a single node menu item to point to different 
node translations belonging to the same translation set depending on the 
current language.

In conjunction with the Menu translation module, shipped with the 
Internationalization (i18n) project [1], it allows to have a fully translated 
menu without needing to replicate the node menu items per language.

USAGE
-----

Enable Menu translation (Node), create a node menu item and check the "Enable 
node translation" checkbox on the menu item editing or node form. You can 
change the default value of the checkbox for a specific menu in the menu edit 
form.

When node translation is enabled for a translation set, the edit form of the 
source node will show the usual menu item form. The node translations instead 
will have a textfield allowing to translate the menu item title through the 
String translation feature of the i18n project [1].

Note that if you need to translate titles into the default language you need to
add the following line to your settings.php:

$conf["i18nstrings_translate_langcode_$langcode"] = TRUE;

where $langcode is the language code of the default language.

Note also that node translation works only for node types having translation
support enabled.

People using Menu Translation (Node) for the first time might experience some
problems in making it work properly. The main cause of these problems is the
language selection feature of the Internationalization project. Language
selection is designed to hide from node listings nodes not matching the
configured criteria. Due to how the menu system internals work, language
selection is applied also to menu items pointing to nodes, therefore if a site
is not set up correctly, its menus might not work as intended.

To work around this problem Menu Translation (Node) provides two ways which
share the same basic concept: temporarily disabling language selection while
rendering the menu tree.

- The first method is transparent to the user and concerns primary and secondary
links directly output in the page template through the $primary_links and
$secondary_links variables. If they are enabled in the theme settings, both
variables should contain the correct menu output without incurring in language
selection problems.
- The second method concerns menu blocks and requires the user to replace the
usual menu blocks with their [i18n] version, which can be found in the blocks
administration page as usual. Note that if primary/secondary links variables are
enabled through the theme settings (even if they are not actually output in the
page template), the corresponding menu blocks will work without needing to be
replaced with their [i18n] version. This is due to the fact that the (correct)
menu tree is computed just once and then reused by all the code needing it. For
the same reason also the menu blocks provided by Menu Block [2] will work only
if they involve primary or secondary links.

If you still cannot get everything working one possible reason is you just found
a bug, but please be sure to have explored the other possibilities.

DEPENDENCIES
------------

This module depends on the Menu translation module shipped with the i18n 
project [2].

CREDITS
-------
The initial development of this module has been sponsored by psegno [3].

LINKS
-----

[1] http://drupal.org/project/menu_block
[2] http://drupal.org/project/issues/i18n
[3] http://www.psegno.it
