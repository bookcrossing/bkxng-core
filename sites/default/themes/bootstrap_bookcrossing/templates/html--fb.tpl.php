<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
    "https://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns:fb="https://www.facebook.com/2008/fbml" xmlns="https://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <?php print $head; ?>
    <title><?php print $head_title; ?></title>
    <style>
      html {font-family: Helvetica, Arial, 'lucida grande', tahoma, verdana, arial, sans-serif;}
      a {color: #3b5998; cursor: pointer; text-decoration: none;}
      section.container.preheader {display: none;}
      header.header {display: none;}
      div.catalog-controls {display: none;}
      footer#footer-wrap {display: none;}
    </style>
    <!-- HTML5 element support for IE6-8 -->
    <!--[if lt IE 9]>
    <script src="//html5shiv.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
</head>
<body>
  <?php print $page; ?>
</body>
