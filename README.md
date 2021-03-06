Turbodbc
========

[![Build Status](https://travis-ci.org/blue-yonder/turbodbc.svg?branch=master)](https://travis-ci.org/blue-yonder/turbodbc)

Turbodbc is a Python module to access relational databases via the Open Database
Connectivity (ODBC) interface. The module complies with the Python Database API
Specification 2.0.


Why should I use turbodbc instead of other ODBC modules?
--------------------------------------------------------

Short answer: turbodbc is faster.

Slightly longer answer: I have tested turbodbc and pyodbc (probably the most
popular Python ODBC module) with various databases (Exasol, PostgreSQL, MySQL)
and corresponding ODBC drivers. I found turbodbc to be consistently faster.

For retrieving result sets, I found speedups between 1.5 and 7. For inserting
data, I found speedups of up to 100.

Is this completely scientific? Not at all. I have not told you about which
hardware I used, which operating systems, drivers, database versions, network
bandwidth, database layout, SQL queries, what is measured, and how I performed
was measured.

All I can tell you is that if I exchange pyodbc with turbodbc, my benchmarks
took less time, often approaching one (reading) or two (writing) orders of
magnitude. Give it a spin for yourself, and tell me if you liked it.


Smooth. What is the trick?
--------------------------

There is not really a trick. Turbodbc implements both sending parameters and
retrieving result sets using buffers of multiple rows/parameter sets. This
avoids round trips to the ODBC driver and (depending how well the ODBC driver
is written) to the database.

In addition, turbodbc uses asynchronous I/O to interleave Python object
conversion and direct database interaction (see performance options
below).


Features
--------

*   Bulk retrieval of select statements
*   Bulk transfer of query parameters
*   Asynchronous I/O for result sets
*   Automatic conversion of decimal type to integer, float, and string as
    appropriate
*   Supported data types for both result sets and parameters:
    `int`, `float`, `str`, `bool`, `datetime.date`, `datetime.datetime`
*   Also provides a high-level C++11 database driver under the hood


Installation
------------

To install turbodbc, please make sure you have the following things installed:

*   A modern g++ compiler (works with 4.7.2+)
*   Boost development headers (typical package name: libboost-all-devel)
*   Unixodbc development headers
*   Python 2.7 development headers

To install turbodbc, please use the following command:

    pip install turbodbc


Basic usage
-----------

Turbodbc follows the specification of the Python database API v2, which you can
find at https://www.python.org/dev/peps/pep-0249/. Here is a short summary,
including the parts not specified.

To establish a connection, use any of the following commands:

    >>> from turbodbc import connect
    >>> connection = connect(dsn='My data source name as given by odbc.ini')
    >>> connection = connect(dsn='my dsn', user='my user has precedence')
    >>> connection = connect(dsn='my dsn', username='field names may depend on the driver')

To execute a query, you need a `cursor` object:

    >>> cursor = connection.cursor()

Here is how to execute a `SELECT` query:

    >>> cursor.execute('SELECT 42')
    >>> for row in cursor:
    >>>     print list(row)

Here is how to execute an `INSERT` query with many parameters:

    >>> parameter_sets = [['hi', 42],
                          ['there', 23]]
    >>> cursor.executemany('INSERT INTO my_table VALUES (?, ?)',
                           parameter_sets)


Performance options
-------------------

Turbodbc offers some options to tune the performance for your database:

    >>> connection.connect(dsn="my dsn",
                           rows_to_buffer=10000,
                           parameter_sets_to_buffer=5000,
                           use_async_io=True)

`rows_to_buffer` affects how many result set rows are retrieved per batch
of results. Larger numbers may yield better performance because database
roundtrips are avoided. However, larger numbers also improve the memory
footprint, since more data is kept in internal buffers.

Similarly, `parameter_sets_to_buffer` changes the number of parameter sets
which are transferred per batch of parameters (e.g., as sent with `executemany()`).

Finally, set `use_async_io` to `True` if you would like to benefit from
asynchronous I/O operations (limited to result sets for the time being).
Asynchronous I/O means that while the main thread converts result set rows
retrieved from the database to Python objects, another thread fetches a
new batch of results from the database in the background. This may yield
significant speedups when retrieving and converting are similarly fast
operations.

    Asynchronous I/O is experimental and has to fully prove itself yet.
    Don't be afraid to give it a try, though.


Development version
-------------------

To use the latest version of turbodbc, you need to follow these steps:

*   Get the source code from github
*   Check the source build requirements (see below) are installed on your computer
*   Create a build directory. Make this your working directory.
*   Execute the following command:

        cmake -DCMAKE_INSTALL_PREFIX=./dist /path/to/source/directory

    This will prepare the build directory for the actual build step.

*   Execute the `make` command to build the code.
*   You can execute the tests with `ctest`.
*   To create a Python source distribution for simple installation, use
    the following commands:
    
        make install
        cd dist
        python setup.py sdist
    
    This will create a `.tar.gz` file in the folder `dist/dist` in your
    build directory. This file is self-contained and can be installed by
    other users using `pip install`.


Source build requirements
-------------------------

For the development build, you also require the following additional 
dependencies:

*   CMake


Supported environments
----------------------

*   Linux (successfully built on Ubuntu 12, Ubuntu 14, Debian 7)
*   Python 2.7 only (yet) 