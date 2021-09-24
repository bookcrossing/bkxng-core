CREATE TABLE `ds_view_modes` (
  `view_mode` varchar(32) NOT NULL DEFAULT '' COMMENT 'The machine name of the view mode.',
  `label` varchar(32) NOT NULL DEFAULT '' COMMENT 'The label of the view mode.',
  `entities` longblob COMMENT 'The entities for this view mode.',
  PRIMARY KEY (`view_mode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='The table that holds custom view modes managed by Display...'