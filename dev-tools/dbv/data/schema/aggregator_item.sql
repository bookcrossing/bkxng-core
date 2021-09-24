CREATE TABLE `aggregator_item` (
  `iid` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary Key: Unique ID for feed item.',
  `fid` int(11) NOT NULL DEFAULT '0' COMMENT 'The aggregator_feed.fid to which this item belongs.',
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Title of the feed item.',
  `link` text NOT NULL COMMENT 'Link to the feed item.',
  `author` varchar(255) NOT NULL DEFAULT '' COMMENT 'Author of the feed item.',
  `description` longtext NOT NULL COMMENT 'Body of the feed item.',
  `timestamp` int(11) DEFAULT NULL COMMENT 'Posted date of the feed item, as a Unix timestamp.',
  `guid` text NOT NULL COMMENT 'Unique identifier for the feed item.',
  PRIMARY KEY (`iid`),
  KEY `fid` (`fid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores the individual items imported from feeds.'