CREATE TABLE `l10n_update_project` (
  `name` varchar(50) NOT NULL COMMENT 'A unique short name to identify the project.',
  `project_type` varchar(50) NOT NULL COMMENT 'Project type, may be core, module, theme',
  `core` varchar(128) NOT NULL DEFAULT '' COMMENT 'Core compatibility string for this project.',
  `version` varchar(128) NOT NULL DEFAULT '' COMMENT 'Human readable name for project used on the interface.',
  `l10n_server` varchar(255) NOT NULL DEFAULT '' COMMENT 'Localization server for this project.',
  `l10n_path` varchar(255) NOT NULL DEFAULT '' COMMENT 'Server path this project updates.',
  `status` int(11) NOT NULL DEFAULT '1' COMMENT 'Status flag. TBD',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Update information for project translations.'