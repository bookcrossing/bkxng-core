<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 07.05.2017
 * Time: 11:33
 */

namespace bookcrossing;


class Author
{
    protected $id;
    protected $name;
    protected $name_sort;
    protected $related_authors;    // ids of related authors
    protected $is_master;          // is it main author from all related (e.g. original name vs translations)

    /**
     * Author constructor.
     * @param string $name
     */
    public function __construct($name = '')
    {
        $this->name = $name;
        if($name != NULL)
        {
            $name_parts = explode(" ", $name);
            $this->name_sort = array_pop($name_parts) . ", ";
            $this->name_sort .= implode(" ", $name_parts);

        }
    }

    /**
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * @return string
     */
    public function getNameSort()
    {
        return $this->name_sort;
    }
}
