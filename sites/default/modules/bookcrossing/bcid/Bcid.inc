<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 29.06.2017
 * Time: 17:19
 */

//namespace bookcrossing;


class Bcid
{
    protected $bcid;        // BCID without prefix
    protected $prefix;      // Prefix that will be added to BCID on render
    protected $charset;     // String that contains characters used to generate BCID
    protected $config;      // Defines pattern (length of blocks between separators) and start/end position in charset to build corresponding blocks
    protected $errors;      // Array of validation errors

    /**
     * Constructor for Bcid class.
     *
     * @param array $config
     * List of bcid parts keyed by part length, where value defines start and end points inside $charset
     * [ int =>     // defines length of subset that will be
     *  [
     *      '#start' => int,
     *      '#end' => int,
     *  ],
     * ]
     * @param string $prefix
     * @param string $charset
     */
    public function __construct($config, $prefix = "", $charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ")
    {
        $this->prefix = $prefix;
        $this->charset = $charset;
        $this->config = $config;
        $this->errors = array();

        $this->resetBcid($config, $prefix, $charset);
    }

    /**
     * Reset Bcid instance using new/updated config, prefix and charset.
     *
     * @param array $config
     * @param string $prefix
     * @param string $charset
     * @return boolean
     */
    public function resetBcid($config, $prefix = "", $charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ")
    {
        $this->prefix = $prefix;
        $this->charset = $charset;
        $this->config = $config;
        $this->errors = array();
        if ( !$this->validateConfig() )
        {
            $this->bcid = NULL;
            return FALSE;
        }
        $this->bcid = $this->generateBcid();
        return TRUE;
    }

    /**
     * Helper function: generates BCID.
     */
    public function generateBcid()
    {
        $bcid = "";
        foreach ($this->config as $key => $block)
        {
            for ($i = 0; $i < $block['#length']; $i++) {
                $index = mt_rand($block['#start'], $block['#end'] - 1);
                $bcid .= $this->charset[$index];
            }
        }
        var_dump($bcid);

        return $bcid;

        // check if the generated bcid is already exists
//        $exists = db_select('bookcrossing_books', 'b')->fields('b', array('bcid'))->condition('bcid', $bcid, '=')->execute()->fetchField();
//        } while ($exists);
    }

    public function renderBcid()
    {
        $result = "";
        $i = 0;
        if ($this->bcid)
        {
            foreach ($this->config as $key => $block)
            {
                if ($i > 0) $result .= "-";
                $result .= substr($this->bcid, $i, $block['#length']);
                $i += $block['#length'];
            }
            if (!empty($this->prefix))
            {
                $result = $this->prefix . $result;
            }
        }

        return $result;
    }

    /**
     * @return mixed
     */
    public function getBcid()
    {
        return $this->bcid;
    }

    /**
     * @param mixed $config
     */
    public function setConfig($config)
    {
        $this->config = $config;
    }

    /**
     * @return mixed
     */
    public function validateConfig()
    {
        $this->errors = array();
        foreach ($this->config as $key => $block)
        {
            if ( !is_array($block) || !in_array('#length', array_keys($block)) || !in_array('#start', array_keys($block)) || !in_array('#end', array_keys($block)) )
            {
                $this->errors[] = "Config should contain associative array keyed with #length, #start and #end.";
            }
            else
            {
                if ( !is_int($block['#length']) || !is_int($block['#start']) || !is_int($block['#end']) ) $this->errors[] = "Bcid block #length={$block['#length']}, #start={$block['#start']}, #end={$block['#end']} values should be integer.";
                if ( $block['#length'] <= 0 || $block['#end'] <= 0 ) $this->errors[] = "Bcid block #length={$block['#length']}, #end={$block['#end']} values should be > 0.";
                if ( $block['#start'] < 0 ) $this->errors[] = "Bcid block #start={$block['#start']} value should be >= 0.";
                if ($block['#start'] >= $block['#end']) $this->errors[] = "Bcid block #start={$block['#start']} value should be less than #end={$block['#end']} value.";
            }
        }
        if (sizeof($this->errors) > 0)
            return FALSE;
        else
            return TRUE;
    }

    /**
     * @return array
     */
    public function getErrors()
    {
        return $this->errors;
    }
}
