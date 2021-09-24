CREATE TABLE `bookcrossing_history` (
  `uid` int(10) unsigned NOT NULL,
  `place_id` int(10) unsigned NOT NULL DEFAULT '0',
  `place_id_left` int(11) unsigned NOT NULL DEFAULT '0',
  `bid` int(11) unsigned NOT NULL,
  `time_left` int(15) unsigned NOT NULL DEFAULT '0',
  `found` int(15) unsigned NOT NULL DEFAULT '0',
  KEY `uid` (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8