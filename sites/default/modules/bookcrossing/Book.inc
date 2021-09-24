<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 07.05.2017
 * Time: 11:09
 */

namespace bookcrossing;


class Book
{
    protected $id;
    protected $title;
    protected $title_sort;
    protected $author_ids;
    protected $rating;
    protected $book_ids;   //isbn, isbn13, asin, etc.
    protected $cover;
    protected $language;

    /**
     * Book constructor.
     */
    public function __construct()
    {

    }

    /**
     * Function returns book metadata.
     */
    // @todo: Use getMetadata function to generate different formats of metadata, e.g. Facebook, Google, etc.
    public function getMetadata()
    {

    }
}