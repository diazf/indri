#ifndef INDRI_HASHSET_HPP
#define INDRI_HASHSET_HPP

#include "indri/Transformation.hpp"
#include <string>
#include <vector>
#include "indri/Parameters.hpp"

#ifdef WIN32
#include <hash_set>
#else
// Move this somewhere
#ifndef HAVE_GCC_VERSION
#define HAVE_GCC_VERSION(MAJOR, MINOR) \
  (__GNUC__ > (MAJOR) || (__GNUC__ == (MAJOR) && __GNUC_MINOR__ >= (MINOR)))
#endif /* ! HAVE_GCC_VERSION */
#if HAVE_GCC_VERSION(4,3)
// if GCC 4.3+
#include <tr1/unordered_set>
#else
#include <ext/hash_set>
#endif
// 3.3 does not use __gnu_cxx, 3.4+ does.
using namespace __gnu_cxx;
#endif

namespace indri
{
  namespace utility
  {
    
#ifdef WIN32
      struct ltstr {
        bool operator()( const char* s1,  const char* s2) const {
          return (strcmp(s1, s2) < 0);
        }
      };
      //studio 7 hash_set provides hash_compare, rather than hash
      // needing an < predicate, rather than an == predicate.
      typedef stdext::hash_set< const char *, stdext::hash_compare< const char *, ltstr> > HashSet;
#else
      struct eqstr {
        bool operator()(char* s1, char* s2) const {
          return strcmp(s1, s2) == 0;
        }
      };
#if HAVE_GCC_VERSION(4,3)
      typedef std::tr1::unordered_set<char *, std::tr1::hash<std::string>, eqstr> HashSet;
#else
      typedef hash_set<char *, hash<char *>, eqstr> HashSet;
#endif
#endif
  }
}

#endif // INDRI_STOPPERTRANSFORMATION_HPP

