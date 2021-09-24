CREATE TABLE `date_format_locale` (
  `format` varchar(100) NOT NULL COMMENT 'The date format string.',
  `type` varchar(64) NOT NULL COMMENT 'The date format type, e.g. medium.',
  `language` varchar(12) NOT NULL COMMENT 'A languages.language for this format to be used with.',
  PRIMARY KEY (`type`,`language`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores configured date formats for each locale.'