CREATE TABLE `bookcrossing_books` (
  `bid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `bcid` varchar(32) NOT NULL DEFAULT '',
  `nid` int(10) unsigned NOT NULL DEFAULT '0',
  `status` int(2) NOT NULL DEFAULT '0',
  `owner` int(11) NOT NULL DEFAULT '-1',
  `current_place` int(10) unsigned NOT NULL,
  `color` varchar(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`bid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Each bcid identifies a unique book.'