CREATE TABLE `locales_source` (
  `lid` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Unique identifier of this string.',
  `location` longtext COMMENT 'Drupal path in case of online discovered translations or file path in case of imported strings.',
  `textgroup` varchar(255) NOT NULL DEFAULT 'default' COMMENT 'A module defined group of translations, see hook_locale().',
  `source` blob NOT NULL COMMENT 'The original string in English.',
  `context` varchar(255) NOT NULL DEFAULT '' COMMENT 'The context this string applies to.',
  `version` varchar(20) NOT NULL DEFAULT 'none' COMMENT 'Version of Drupal, where the string was last used (for locales optimization).',
  PRIMARY KEY (`lid`),
  KEY `source_context` (`source`(30),`context`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='List of English source strings.'