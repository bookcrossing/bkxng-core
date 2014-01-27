CREATE TABLE `languages` (
  `language` varchar(12) NOT NULL DEFAULT '' COMMENT 'Language code, e.g. ’de’ or ’en-US’.',
  `name` varchar(64) NOT NULL DEFAULT '' COMMENT 'Language name in English.',
  `native` varchar(64) NOT NULL DEFAULT '' COMMENT 'Native language name.',
  `direction` int(11) NOT NULL DEFAULT '0' COMMENT 'Direction of language (Left-to-Right = 0, Right-to-Left = 1).',
  `enabled` int(11) NOT NULL DEFAULT '0' COMMENT 'Enabled flag (1 = Enabled, 0 = Disabled).',
  `plurals` int(11) NOT NULL DEFAULT '0' COMMENT 'Number of plural indexes in this language.',
  `formula` varchar(255) NOT NULL DEFAULT '' COMMENT 'Plural formula in PHP code to evaluate to get plural indexes.',
  `domain` varchar(128) NOT NULL DEFAULT '' COMMENT 'Domain to use for this language.',
  `prefix` varchar(128) NOT NULL DEFAULT '' COMMENT 'Path prefix to use for this language.',
  `weight` int(11) NOT NULL DEFAULT '0' COMMENT 'Weight, used in lists of languages.',
  `javascript` varchar(64) NOT NULL DEFAULT '' COMMENT 'Location of JavaScript translation file.',
  PRIMARY KEY (`language`),
  KEY `list` (`weight`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='List of all available languages in the system.'