
find_library(FOO_LIBRARY NAMES foo HINTS "/Users/ridwan/Documents/CS/Udacity_C++/CppND-Route-Planning-Project/cmake/Tests/FindPackageModeMakefileTest" )
find_path(FOO_INCLUDE_DIR NAMES foo.h HINTS "/Users/ridwan/Documents/CS/Udacity_C++/CppND-Route-Planning-Project/cmake/Tests/FindPackageModeMakefileTest" )

set(FOO_LIBRARIES ${FOO_LIBRARY})
set(FOO_INCLUDE_DIRS "${FOO_INCLUDE_DIR}"  "/some/path/with a space/include" )

add_library(Foo::Foo INTERFACE IMPORTED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Foo  DEFAULT_MSG  FOO_LIBRARY FOO_INCLUDE_DIR )
