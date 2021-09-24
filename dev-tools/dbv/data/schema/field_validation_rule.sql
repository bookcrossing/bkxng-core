CREATE TABLE `field_validation_rule` (
  `ruleid` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique identifier of the validation rule',
  `rulename` varchar(255) NOT NULL DEFAULT '' COMMENT 'Name of the validation rule',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT 'Machine name of the validation rule',
  `field_name` varchar(32) NOT NULL DEFAULT '',
  `col` varchar(32) NOT NULL DEFAULT 'value',
  `entity_type` varchar(32) NOT NULL DEFAULT '',
  `bundle` varchar(128) NOT NULL DEFAULT '',
  `validator` varchar(255) NOT NULL DEFAULT '' COMMENT 'The validator key',
  `error_message` varchar(255) DEFAULT NULL COMMENT 'Rule error message',
  `settings` longtext COMMENT 'Serialized settings for the validator to be used',
  PRIMARY KEY (`ruleid`),
  KEY `field_name_bundle` (`field_name`,`entity_type`,`bundle`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores rule definitions'