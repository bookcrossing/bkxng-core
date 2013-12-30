CREATE TABLE `fb_social_preset` (
  `name` varchar(255) NOT NULL DEFAULT '' COMMENT 'The primary identifier for a plugin preset.',
  `description` varchar(255) NOT NULL DEFAULT '' COMMENT 'Description for this plugin preset.',
  `plugin_type` varchar(255) NOT NULL DEFAULT '' COMMENT 'Type of this preset.',
  `settings` text COMMENT 'Serialized storage of drupal related plugin settings.',
  `fb_attrs` text COMMENT 'Serialized storage of facebook related plugin settings',
  PRIMARY KEY (`name`),
  KEY `type` (`plugin_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Storage for user-defined fb plugins templates.'