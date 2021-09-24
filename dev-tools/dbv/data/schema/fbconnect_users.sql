CREATE TABLE `fbconnect_users` (
  `uid` int(10) unsigned NOT NULL DEFAULT '0',
  `fbuid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `timestamp` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`fbuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8