CREATE TABLE `aggregator_feed` (
  `fid` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary Key: Unique feed ID.',
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Title of the feed.',
  `url` text NOT NULL COMMENT 'URL to the feed.',
  `refresh` int(11) NOT NULL DEFAULT '0' COMMENT 'How often to check for new feed items, in seconds.',
  `checked` int(11) NOT NULL DEFAULT '0' COMMENT 'Last time feed was checked for new items, as Unix timestamp.',
  `queued` int(11) NOT NULL DEFAULT '0' COMMENT 'Time when this feed was queued for refresh, 0 if not queued.',
  `link` text NOT NULL COMMENT 'The parent website of the feed; comes from the <link> element in the feed.',
  `description` longtext NOT NULL COMMENT 'The parent website’s description; comes from the <description> element in the feed.',
  `image` longtext NOT NULL COMMENT 'An image representing the feed.',
  `hash` varchar(64) NOT NULL DEFAULT '' COMMENT 'Calculated hash of the feed data, used for validating cache.',
  `etag` varchar(255) NOT NULL DEFAULT '' COMMENT 'Entity tag HTTP response header, used for validating cache.',
  `modified` int(11) NOT NULL DEFAULT '0' COMMENT 'When the feed was last modified, as a Unix timestamp.',
  `block` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'Number of items to display in the feed’s block.',
  PRIMARY KEY (`fid`),
  UNIQUE KEY `title` (`title`),
  KEY `queued` (`queued`),
  KEY `url` (`url`(255))
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores feeds to be parsed by the aggregator.'