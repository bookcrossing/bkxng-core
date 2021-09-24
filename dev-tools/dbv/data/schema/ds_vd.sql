CREATE TABLE `ds_vd` (
  `vd` varchar(128) NOT NULL DEFAULT '' COMMENT 'The primary identifier for the views display.',
  `label` varchar(132) NOT NULL DEFAULT '' COMMENT 'The label for the views display.',
  PRIMARY KEY (`vd`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='The base table for views displays.'