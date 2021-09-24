CREATE TABLE `aggregator_category_item` (
  `iid` int(11) NOT NULL DEFAULT '0' COMMENT 'The feed item’s aggregator_item.iid.',
  `cid` int(11) NOT NULL DEFAULT '0' COMMENT 'The aggregator_category.cid to which the feed item is being assigned.',
  PRIMARY KEY (`cid`,`iid`),
  KEY `iid` (`iid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Bridge table; maps feed items to categories.'