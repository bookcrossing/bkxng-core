CREATE TABLE `ds_fields` (
  `field` varchar(32) NOT NULL DEFAULT '' COMMENT 'The machine name of the field.',
  `label` varchar(32) NOT NULL DEFAULT '' COMMENT 'The label of the field.',
  `field_type` smallint(6) NOT NULL DEFAULT '0' COMMENT 'The type of of the field',
  `entities` longblob COMMENT 'The entities for this field.',
  `properties` longblob COMMENT 'The properties for this field.',
  `ui_limit` longblob COMMENT 'The UI limit for this field.',
  PRIMARY KEY (`field`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='The table that holds custom fields managed by Display Suite.'