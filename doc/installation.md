# Installation

## Boost

This project requires boost_units. If you have a local install of boost, that can be used, otherwise a packaged version of units (and its dependencies) are provided in this repository.

cmake will first look for an installed version of boost. If cmake is unable to find boost, it will default to using the version provided in this repo. If you have boost, but want to use the packaged version instead, there is a cmake flag (USE_THIRDPARTY_BOOST) which can be set which will cause cmake to ignore any boost versions installed on the system. Note that units is a header-only library and does not need to be built.

USE_THIRDPARTY_BOOST		Force the use of the packaged boost version, ignoring any installed boost versions (default: false)
