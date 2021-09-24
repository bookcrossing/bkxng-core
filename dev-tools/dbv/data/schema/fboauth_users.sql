CREATE TABLE `fboauth_users` (
  `uid` int(10) unsigned NOT NULL DEFAULT '0',
  `fbid` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`fbid`),
  KEY `uid_fbid` (`uid`,`fbid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8