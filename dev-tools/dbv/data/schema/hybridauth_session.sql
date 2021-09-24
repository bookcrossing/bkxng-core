CREATE TABLE `hybridauth_session` (
  `uid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The users.uid that owns this HybridAuth session data.',
  `data` mediumtext NOT NULL COMMENT 'A serialized array containing session data from HybridAuth library.',
  `updated` int(11) NOT NULL DEFAULT '0' COMMENT 'The Unix timestamp when the session was saved.',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Holds sessions data from HybridAuth library.'