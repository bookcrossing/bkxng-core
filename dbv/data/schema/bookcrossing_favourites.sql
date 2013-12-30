CREATE TABLE `bookcrossing_favourites` (
  `uid` int(10) unsigned NOT NULL,
  `bid` int(10) unsigned NOT NULL,
  KEY `bid` (`bid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8