<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 28.06.2017
 * Time: 19:27
 */

namespace bookcrossing;


class BookCatalog
{
    protected $sort_type;
    protected $view_type;

    /**
     * Book Catalog constructor.
     */
    public function __construct()
    {

    }

    public function getSortType()
    {
        return $this->sort_type;
    }

    public function getViewType()
    {
        return $this->view_type;
    }

    /**
     * Function returns book catalog metadata.
     */
    // @todo: Use getMetadata function to generate different formats of metadata, e.g. Facebook, Google, etc.
    public function getMetadata()
    {

    }

}