<?php
/**
 * @file
 * This file contains API documentation for the Facebook OAuth module. Note that
 * all of this code is merely for example purposes, it is never executed when
 * using the Facebook OAuth module.
 */

/**
 * Hook to register new Facebook OAuth actions.
 *
 * The Facebook OAuth module includes two default actions. The "connect" action
 * links a Facebook account with a Drupal user account. The "deauth" action
 * revokes Drupal's Facebook access for a user and deassociates the accounts.
 * You can write additional actions (such as data imports) by using this hook in
 * your own modules.
 *
 * A full example of implementing this hook is included with the README.txt file
 * included with the Facebook OAuth module.
 *
 * @return
 *   An array of Facebook OAuth actions keyed by a unique action name. Each
 *   action must specify at least the following properties:
 *   - title: A title for the action.
 *   - callback: The name of a function to execute after gaining access.
 *   - permissions: A list of Facebook permissions to request.
 *
 * @see fboauth_fboauth_actions().
 */
function hook_fboauth_actions() {
  // Give each action a unique key, such as "mymodule_photo_import" for a photo
  // import. This function should begin with the name of your module.
  $actions['mymodule_photo_import'] = array(
    // Give you action a human-readable name. This will be used when showing
    // the user a link to start this action.
    'title' => t('Import my Facebook photos'),

    // Specify the name of your callback function that contains the import.
    'callback' => 'mymodule_fboauth_action_photo_import',

    // Specify permissions you need to do this action. See the Facebook API for
    // a list: http://developers.facebook.com/docs/authentication/permissions/
    'permissions' => array(
      'user_photos', // Gets access to a user's photos.
    ),

    // Optionally specify a file that contains your callback function. If you
    // put your callback function in the .module file, this is unnecessary.
    // 'file' => 'mymodule.inc',

    // Optionally define a theme function for printing out your link (not
    // including the "theme_" prefix). If you use this option, you must register
    // this function in hook_theme(). If you don't use this option, the link
    // will be output with the theme_fboauth_action() function.
    // 'theme callback' => 'mymodule_fboauth_action',
  );
  return $actions;
}

/**
 * Alter the list of Facebook Actions provided through hook_fboauth_actions().
 *
 * @see fboauth_fboauth_actions().
 */
function hook_fboauth_actions_alter(&$actions) {
  // Replace the normal login callback with custom login callback.
  $actions['connect']['callback'] = 'mymodule_fboauth_action_connect';
}

/**
 * Hook to manually map Facebook data to a Drupal user account upon connecting.
 *
 * This hook is fired before a Drupal user account is created by the Facebook
 * OAuth module.
 *
 * @param $edit
 *   A user account array, not yet including the UID. Make modifications to this
 *   array if other modules will then save this information for you in
 *   hook_user_presave() or hook_user_insert().
 * @param $fbuser
 *   The Facebook user account. Note that the contents of this object may change
 *   depending on what access the user has granted.
 * @return
 *   None. Modify the $edit array by reference.
 *
 * @see hook_fboauth_user_save()
 */
function hook_fboauth_user_presave(&$edit, $fbuser) {
  // Save the user's first name into a field provided by Profile module.
  if (isset($fbuser->first_name)) {
    $edit['profile_first_name'] = $fbuser->first_name;
  }
}

/**
 * Hook to manually save Facebook data after a user has connected.
 *
 * This hook is fired after a Drupal user account is created by the Facebook
 * OAuth module.
 *
 * @param $account
 *   A full Drupal user account object.
 * @param $fbuser
 *   The Facebook user account. Note that the contents of this object may change
 *   depending on what access the user has granted.
 * @return
 *   None.
 *
 * @see hook_fboauth_user_presave()
 */
function hook_fboauth_user_save($account, $fbuser) {
  // Save the Facebook user ID into a custom module table.
  $mydata = array(
    'uid' => $account->uid,
    'fbid' => $fbuser->id,
    'real_name' => $fbuser->name,
  );
  drupal_write_record('mytable', $mydata);
}
