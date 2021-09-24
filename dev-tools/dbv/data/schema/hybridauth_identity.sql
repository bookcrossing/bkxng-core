CREATE TABLE `hybridauth_identity` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Unique ID of HybridAuth identity.',
  `uid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The users.uid that owns this HybridAuth identity.',
  `provider` varchar(32) NOT NULL DEFAULT '' COMMENT 'The authentication provider for this HybridAuth identity.',
  `provider_identifier` varchar(32) NOT NULL DEFAULT '' COMMENT 'The authentication provider UID for this HybridAuth identity.',
  `data` blob NOT NULL COMMENT 'A serialized array containing information from HybridAuth library.',
  PRIMARY KEY (`id`),
  UNIQUE KEY `provider_provider_identifier` (`provider`,`provider_identifier`),
  KEY `uid` (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Holds identities from HybridAuth library.'