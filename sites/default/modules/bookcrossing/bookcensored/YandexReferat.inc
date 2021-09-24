<?php
/**
 * @file
 * Class for requesting Yandex.Referats
 *
 */

// namespace bookcrossing;


class YandexReferat
{
    // Type of Referat
    protected $type;
    // Title of Referat
    protected $title;
    // Body of Referat
    protected $body;
    // Topics of Referat
    protected $topics;
    // Avalible topics for service Yandex.Referats
    protected static $avalibleTopics = array(
        'astronomy'     => 'Astronomy',
        'geology'       => 'Geology',
        'gyroscope'     => 'Gyroscopes',
        'literature'    => 'Literary criticism',
        'marketing'     => 'Marketing',
        'mathematics'   => 'Mathematics',
        'music'         => 'Musicology',
        'polit'         => 'Political science',
        'agrobiologia'  => 'Agrobiology',
        'law'           => 'Jurisprudence',
        'psychology'    => 'Psychology',
        'geography'     => 'Geography',
        'physics'       => 'Physics',
        'philosophy'    => 'Philosophy',
        'chemistry'     => 'Chemistry',
        'estetica'      => 'Aesthetics',
    );
    // Topic settings
    protected $userTopics;
    // Built query to Yandex.Referats
    private $query;
    // Avalible hosts for service Yandex.Referats
    private static $hosts = array(
        'https://yandex.ru',
//        'http://referats.yandex.ru',
//        'http://vesna.yandex.ru'
    );

    /**
     * YandexReferat constructor.
     * @param array $user_topics
     */
    public function __construct(array $user_topics = array())
    {
        $this->userTopics = $user_topics;
        $this->prepareTopics();
        $this->prepareQuery();
    }

    /**
     * @param $name
     * @return bool
     */
    function __get($name)
    {
        if (isset($this->{$name})) {
            return $this->{$name};
        }
        return FALSE;
    }

    /**
     * @return array
     */
    protected function prepareTopics()
    {
        if (empty($this->userTopics)) {
            $topics = self::$avalibleTopics;
            // Random count of topics.
            $cnt = rand(1, count($topics));
            $this->topics = (array)array_rand( array_keys($topics), $cnt );
        }
        else {
            $this->topics = $this->userTopics;
        }

        return $this->topics;
    }

    /**
     * @return string
     *
     * Query example:
     *      http://yandex.ru/referats/?t=astronomy+estetica
     *
     */
    protected function prepareQuery()
    {
        $query = '';

        // Get one of two avalible hosts.
        $host = self::$hosts[ rand(0, count(self::$hosts)-1) ];
        $query = $host . "/referats/?t=" . implode("+", $this->topics);

        $this->query = $query;
        return $this->query;
    }

    /**
     * @param bool $reset_query
     * @return bool
     */
    public function request($reset_query = FALSE)
    {
        if ($reset_query) {
            $this->prepareQuery();
        }

        $result = drupal_http_request($this->query);
        // I'm sorry. We need some delay.
//        $delay = variable_get('bookcensored_yandex_referat_delay_mks', 100000);
//        usleep($delay);
        if (empty($result->error)) {
            // Faster then preg_match().
            $doc = new DOMDocument();
            // DOMDocument wants ISO-8859-1. But we have utf-8. Fast fix.
            if (@$doc->loadHTML('<?xml encoding="utf-8" ?>' . $result->data)) {
                $xpath = new DOMXpath($doc);
                // Referat type string
                $typeNodes = $xpath->query('//div');
                $this->type = $typeNodes->item(0)->nodeValue;
                // Topic string
                $headerNodes = $xpath->query('//strong[1]');
                $this->title = $headerNodes->item(0)->nodeValue;
                // Trim quotes.
                if (preg_match('/«(.*)»/iu', $this->title, $matches)) {
                    $this->title = $matches[1];
                }
                // Array of paragraphs
                $this->body = array();
                $paragraphNodes = $xpath->query('//p');
                foreach ($paragraphNodes as $Node) {
                    $this->body[] = str_replace(array("\r", "\r\n", "\n"), '', $Node->nodeValue);
                }

                return TRUE;
            }
            else {
                watchdog('bookcensored', 'Error while parsing HTML for URL: %url', array('%url' => $this->query), WATCHDOG_ERROR);
            }
        }
        else {
            watchdog('bookcensored', 'Error "%error" (@code) for URL: %url',
                array('%error' => $result->error, '@code' => $result->code, '%url' => $this->query),
                WATCHDOG_ERROR
            );
        }

        return FALSE;
    }

    /**
     * @param null $topic_key
     * @return array|string
     */
    public function getAvalibleTopics($topic_key = NULL) {
        if (!isset($topic_key)) {
            return self::$avalibleTopics;
        }
        elseif (isset(self::$avalibleTopics[$topic_key])) {
            return self::$avalibleTopics[$topic_key];
        }

        return FALSE;

    }

    /**
     * @return string
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * @return string
     */
    public function getTitle()
    {
        return $this->title;
    }

    /**
     * @return array
     */
    public function getBody()
    {
        return $this->body;
    }
}