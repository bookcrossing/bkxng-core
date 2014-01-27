CREATE TABLE `aggregator_category_feed` (
  `fid` int(11) NOT NULL DEFAULT '0' COMMENT 'The feed’s aggregator_feed.fid.',
  `cid` int(11) NOT NULL DEFAULT '0' COMMENT 'The aggregator_category.cid to which the feed is being assigned.',
  PRIMARY KEY (`cid`,`fid`),
  KEY `fid` (`fid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Bridge table; maps feeds to categories.'