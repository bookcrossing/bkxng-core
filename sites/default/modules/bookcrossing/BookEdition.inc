<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 07.05.2017
 * Time: 11:35
 */

namespace bookcrossing;


class BookEdition extends Book
{
    protected $title_original;
    protected $translator;
    protected $annotation;
    protected $published;
    protected $publisher;
    protected $format;

    /**
     * BookEdition constructor.
     */
    public function __construct()
    {

    }
}