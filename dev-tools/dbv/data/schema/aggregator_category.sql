CREATE TABLE `aggregator_category` (
  `cid` int(11) NOT NULL AUTO_INCREMENT COMMENT 'Primary Key: Unique aggregator category ID.',
  `title` varchar(255) NOT NULL DEFAULT '' COMMENT 'Title of the category.',
  `description` longtext NOT NULL COMMENT 'Description of the category',
  `block` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'The number of recent items to show within the category block.',
  PRIMARY KEY (`cid`),
  UNIQUE KEY `title` (`title`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores categories for aggregator feeds and feed items.'