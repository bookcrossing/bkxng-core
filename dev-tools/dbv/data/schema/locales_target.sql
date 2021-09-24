CREATE TABLE `locales_target` (
  `lid` int(11) NOT NULL DEFAULT '0' COMMENT 'Source string ID. References locales_source.lid.',
  `translation` blob NOT NULL COMMENT 'Translation string value in this language.',
  `language` varchar(12) NOT NULL DEFAULT '' COMMENT 'Language code. References languages.language.',
  `plid` int(11) NOT NULL DEFAULT '0' COMMENT 'Parent lid (lid of the previous string in the plural chain) in case of plural strings. References locales_source.lid.',
  `plural` int(11) NOT NULL DEFAULT '0' COMMENT 'Plural index number in case of plural strings.',
  `l10n_status` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`language`,`lid`,`plural`),
  KEY `lid` (`lid`),
  KEY `plid` (`plid`),
  KEY `plural` (`plural`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores translated versions of strings.'