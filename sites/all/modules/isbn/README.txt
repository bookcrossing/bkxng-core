ISBN Module provides a way to keep track of ISBN to node relationships.  The module also handles the the issue of 10 and 13 digit ISBNs. 

The main purpose of creating this module was to work with library catalogs and the issue of record duplication.

But this module is also being designed to be used with a number of web service APIs, namely LibraryThings's ThingISBN, Amazon's API and Google's book API and could potentially work with paid APIs like Syndetic Solutions and xISBN from OCLC.  All of which are ISBN.

Installation
------------

Copy ISBN.module to your module directory and then enable on the admin
modules page. 

Presently, the module expects a CCK text field named field_isbn, this will be improved upon in the future, and I would expect a helper module that provided a CCK field for an ISBN .

Author
------
Andy Austin
austinone@gmail.com
