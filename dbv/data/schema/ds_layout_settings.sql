CREATE TABLE `ds_layout_settings` (
  `id` varchar(255) NOT NULL DEFAULT '' COMMENT 'The unique id the layout.',
  `entity_type` varchar(32) NOT NULL DEFAULT '' COMMENT 'The name of the entity.',
  `bundle` varchar(64) NOT NULL DEFAULT '' COMMENT 'The name of the entity.',
  `view_mode` varchar(32) NOT NULL DEFAULT '' COMMENT 'The name of the view_mode.',
  `layout` varchar(64) NOT NULL DEFAULT '' COMMENT 'The name of the layout.',
  `settings` longblob COMMENT 'The settings for this layout.',
  PRIMARY KEY (`id`),
  KEY `ds_entity` (`entity_type`),
  KEY `ds_bundle` (`bundle`),
  KEY `ds_view_mode` (`view_mode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='The table that holds the layouts configuration.'