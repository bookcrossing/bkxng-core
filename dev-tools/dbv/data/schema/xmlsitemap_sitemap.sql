CREATE TABLE `xmlsitemap_sitemap` (
  `smid` varchar(64) NOT NULL COMMENT 'The sitemap ID (the hashed value of xmlsitemap.context.',
  `context` text NOT NULL COMMENT 'Serialized array with the sitemaps context',
  `updated` int(10) unsigned NOT NULL DEFAULT '0',
  `links` int(10) unsigned NOT NULL DEFAULT '0',
  `chunks` int(10) unsigned NOT NULL DEFAULT '0',
  `max_filesize` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`smid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8