CREATE TABLE `bookcrossing_comment` (
  `cid` int(11) NOT NULL AUTO_INCREMENT,
  `bid` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL,
  `comment` varchar(1024) NOT NULL DEFAULT '',
  PRIMARY KEY (`cid`),
  KEY `bid` (`bid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8