CREATE TABLE `views_fb_like` (
  `nid` int(10) unsigned NOT NULL COMMENT 'The node id.',
  `share_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of times users have shared the page on Facebook.',
  `like_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of times Facebook users have "Liked" the page.',
  `comment_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The number of comments users have made on the shared story.',
  `total_count` int(10) unsigned NOT NULL DEFAULT '0' COMMENT 'The total number of times the URL has been shared, liked, or commented on.',
  `last_updated` int(11) NOT NULL COMMENT 'The last updated timestamp.',
  PRIMARY KEY (`nid`),
  KEY `last_updated` (`last_updated`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores aggregated counts of nodes from FQL.'