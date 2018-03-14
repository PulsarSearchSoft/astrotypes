
# Requirements

Starting point for the requirements discussed during the astrotype sprint in Amsterdam (March 2018).

## Components

* Defined data types
    * Time-Frequency
    * Frequency-Time
    * Time-DM
    * Units
* SIGPROC file format
    * Reader
    * Writer
* FITS file format
    * Reader
    * Writer

## Requirements

* Data types
    * They should have an arbitrary number of dimensions
        * The size of each dimension is specified in the constructor
        * Dimensions are ordered in the way they appear in the template declaration e.g. the rightmost dimension is the 
            include(doxygen) fastest changing one
        * Dimensions must have an associated unit
        * It should be possible to address dimensions e.g. to transpose a data type
    * They should provide views
        * A view is specified by expressing, for each dimension, a pair containing the index of the first element included in 
            the view, and the number of elements included in the view
        * To express a view with gaps is necessary to use a collection of views over the same data type
    * They should provide iterators
        * Iterators should make them compatibles with C++ STL algorithms
    * It should be possible to associate metadata to each object e.g. through inheritance
    * It should be possible to use an iterator from one data type to point to data in another data type
        * An index mapper must be provided
    * They should wrap memory and allow operations on memory that is not owned by them e.g. a PSRDADA buffer