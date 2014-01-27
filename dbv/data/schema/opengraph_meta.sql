CREATE TABLE `opengraph_meta` (
  `nid` int(10) unsigned NOT NULL,
  `title` varchar(255) NOT NULL DEFAULT '',
  `description` longtext NOT NULL,
  `image` varchar(255) NOT NULL DEFAULT '',
  `type` varchar(255) NOT NULL DEFAULT '',
  `optional` text,
  PRIMARY KEY (`nid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores Open Graph meta tag info useful for when sharing...'