# CMake generated Testfile for 
# Source directory: /home/rkunds/projects/petdb
# Build directory: /home/rkunds/projects/petdb/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[PetDB_test]=] "/home/rkunds/projects/petdb/build/PetDB_test")
set_tests_properties([=[PetDB_test]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/rkunds/projects/petdb/CMakeLists.txt;45;add_test;/home/rkunds/projects/petdb/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
