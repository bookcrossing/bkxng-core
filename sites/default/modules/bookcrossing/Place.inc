<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 26.06.2017
 * Time: 16:36
 */

namespace bookcrossing;


class Place
{
    protected $address;

    public function __construct($country = '', $city = '', $place = '')
    {
        $this->address = ['country' => $country, 'city' => $city, 'place' => $place];
    }

}