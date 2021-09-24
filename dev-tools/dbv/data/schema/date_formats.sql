CREATE TABLE `date_formats` (
  `dfid` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'The date format identifier.',
  `format` varchar(100) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL COMMENT 'The date format string.',
  `type` varchar(64) NOT NULL COMMENT 'The date format type, e.g. medium.',
  `locked` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'Whether or not this format can be modified.',
  PRIMARY KEY (`dfid`),
  UNIQUE KEY `formats` (`format`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores configured date formats.'