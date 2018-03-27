
# Requirements

Starting point for the requirements discussed during the astrotype sprint in Amsterdam (March 2018).

## Components

* Defined data types
    * Time-Frequency (TF)
    * Frequency-Time (FT)
    * Time-DM (TD)
    * Units (U)
* SIGPROC file format
    * Reader
    * Writer
* FITS file format
    * Reader
    * Writer

## Requirements

### Data Types

| Level | Description | Interested Object | Status |
|:-----:|:-----------:|:-----------------:|:------------:|
| 0 | A data type should have an arbitrary number of dimensions | [MultiArray](https://pulsarsearchsoft.github.io/astrotypes/html/classpss_1_1astrotypes_1_1MultiArray.html) | |
| 1 | Dimensions are specified in the object constructor | [MultiArray](https://pulsarsearchsoft.github.io/astrotypes/html/classpss_1_1astrotypes_1_1MultiArray.html) | |
| 1 | Dimensions are ordered in memory by the template parameters (outer to inner) | [MultiArray](https://pulsarsearchsoft.github.io/astrotypes/html/classpss_1_1astrotypes_1_1MultiArray.html) | |
| 1 | Dimensions have an associated unit| | |
| 1 | Dimensions can be addressed | | |
| 1 | Dimensions can be transposed | | |
| 0 | A data type should provide views over arbitrary sections of it | [Slice](https://pulsarsearchsoft.github.io/astrotypes/html/classpss_1_1astrotypes_1_1Slice.html) | |
| 0 | A data type should provide access iterators | | |
| 1 | Iterators should provide compatibility with C++ STL algorithms| | |
| 2 | A mapping between iterators of certain data types should be possible | | |
| 0 | A data type could have associated metadata | | |
| 0 | A data type should be able to provide access to memory it does not own | | |
| 0 | A data type should be resizeable at runtime | [MultiArray](https://pulsarsearchsoft.github.io/astrotypes/html/classpss_1_1astrotypes_1_1MultiArray.html) | |

