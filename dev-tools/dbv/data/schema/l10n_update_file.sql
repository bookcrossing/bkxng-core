CREATE TABLE `l10n_update_file` (
  `project` varchar(50) NOT NULL COMMENT 'A unique short name to identify the project.',
  `language` varchar(12) NOT NULL COMMENT 'Reference to the languages.language for this translation.',
  `type` varchar(50) NOT NULL DEFAULT '' COMMENT 'File origin: download or localfile',
  `filename` varchar(255) NOT NULL DEFAULT '' COMMENT 'Link to translation file for download.',
  `fileurl` varchar(255) NOT NULL DEFAULT '' COMMENT 'Link to translation file for download.',
  `uri` varchar(255) NOT NULL DEFAULT '' COMMENT 'File system path for importing the file.',
  `timestamp` int(11) DEFAULT '0' COMMENT 'Unix timestamp of the time the file was downloaded or saved to disk. Zero if not yet downloaded',
  `version` varchar(128) NOT NULL DEFAULT '' COMMENT 'Version tag of the downloaded file.',
  `status` int(11) NOT NULL DEFAULT '1' COMMENT 'Status flag. TBD',
  `last_checked` int(11) DEFAULT '0' COMMENT 'Unix timestamp of the last time this translation was downloaded from or checked at remote server and confirmed to be the most recent release available.',
  PRIMARY KEY (`project`,`language`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='File and download information for project translations.'