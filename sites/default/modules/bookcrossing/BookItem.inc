<?php
/**
 * Created by PhpStorm.
 * User: alexe
 * Date: 07.05.2017
 * Time: 11:51
 */

namespace bookcrossing;


class BookItem extends BookEdition
{
    public $bcid;       // unique BookCrossing ID, that identifies book item

    public function __construct()
    {
    }

    public function loadByBcid($bcid)
    {
        if (!empty($bcid)) {
            $query = db_select('bookcrossing_books', 'book');
            $query->innerJoin('node', 'node', 'book.bid = history.book_id');
            $states = $query->fields('book', array('bid', 'bcid', 'nid', 'status', 'owner', 'color'))
                ->fields('history', array('user_id', 'place_id', 'event_type', 'date_time'))
                ->condition('book.bid', $bids, 'IN')
                ->execute();
            $states = $states->fetchAllAssoc('bid');

            $nids = array();
            $tids = array();
            $uids = array();

            // Creating array of entity ids for loading multiple in one time.
            foreach ($states as $state) {
                $tids[$state->place_id] = $state->place_id;
                $uids[$state->user_id] = $state->user_id;
                $nids[$state->nid] = $state->nid;
            }

            // Loading needed entities.
            $places = taxonomy_term_load_multiple($tids);
            $users = user_load_multiple($uids);
            $nodes = node_load_multiple($nids);

            // Creating array of books
            foreach ($states as $state) {
                $book = (array)$state;
                $book['user'] = $users[$state->user_id];
                $book['node'] = $nodes[$state->nid];
                $book['place'] = $places[$state->place_id];
                $books[] = $book;
            }
            return TRUE;
        }
        else
            return FALSE;

    }
}