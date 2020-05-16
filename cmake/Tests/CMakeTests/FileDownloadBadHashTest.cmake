if(NOT "/Users/ridwan/Documents/CS/Udacity_C++/CppND-Route-Planning-Project/cmake/Tests/CMakeTests" MATCHES "^/")
  set(slash /)
endif()
set(url "file://${slash}/Users/ridwan/Documents/CS/Udacity_C++/CppND-Route-Planning-Project/cmake/Tests/CMakeTests/FileDownloadInput.png")
set(dir "/Users/ridwan/Documents/CS/Udacity_C++/CppND-Route-Planning-Project/cmake/Tests/CMakeTests/downloads")

file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  STATUS status
  EXPECTED_HASH SHA1=5555555555555555555555555555555555555555
  )
