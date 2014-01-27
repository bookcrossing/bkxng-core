CREATE TABLE `ds_field_settings` (
  `id` varchar(255) NOT NULL DEFAULT '' COMMENT 'The unique id this setting.',
  `entity_type` varchar(32) NOT NULL DEFAULT '' COMMENT 'The name of the entity.',
  `bundle` varchar(64) NOT NULL DEFAULT '' COMMENT 'The name of the entity.',
  `view_mode` varchar(32) NOT NULL DEFAULT '' COMMENT 'The name of the view_mode.',
  `settings` longblob COMMENT 'The display suite field settings for this layout.',
  PRIMARY KEY (`id`),
  KEY `ds_entity` (`entity_type`),
  KEY `ds_bundle` (`bundle`),
  KEY `ds_view_mode` (`view_mode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='The table that holds display suite field settings per...'