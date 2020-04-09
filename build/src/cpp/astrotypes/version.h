#ifndef PSS_ASTROTYPES_VERSION_H
#define PSS_ASTROTYPES_VERSION_H


#define ASTROTYPES_BUILD_TYPE "release"
#define ASTROTYPES_VERSION "0.0.0 e8dcd3ed932d81ee9d92b477583cc8b44a46a669"
#define PROJECT_NAME "astrotypes"
#define ASTROTYPES_RCS_VERSION "e8dcd3ed932d81ee9d92b477583cc8b44a46a669"
#define ASTROTYPES_BOOST_MAJOR_VERSION 1
#define ASTROTYPES_BOOST_MINOR_VERSION 60

#if defined(__DATE__) && defined(__TIME__)
#   define ASTROTYPES_VERSION_LONG "0.0.0 e8dcd3ed932d81ee9d92b477583cc8b44a46a669 (release), " __DATE__ " " __TIME__
#   define ASTROTYPES_BUILD_INFO "(release) v" VERSION " (" __DATE__ " " __TIME__")"
#else
#   define ASTROTYPES_VERSION_LONG "0.0.0 e8dcd3ed932d81ee9d92b477583cc8b44a46a669 (release)"
#   define ASTROTYPES_BUILD_INFO "(release) v" ASTROTYPES_VERSION
#endif

namespace pss {
namespace astrotypes {

extern const char* version;
extern const char* full_version;
extern const char* boost_version;

} // namespace astrotypes
} // namespace pss

#endif // PSS_ASTROTYPES_VERSION_H
