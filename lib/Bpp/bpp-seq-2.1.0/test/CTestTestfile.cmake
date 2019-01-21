# CMake generated Testfile for 
# Source directory: /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0/test
# Build directory: /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-seq-2.1.0/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_alphabets "test_alphabets")
set_tests_properties(test_alphabets PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_sequences "test_sequences")
set_tests_properties(test_sequences PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_io "test_io")
set_tests_properties(test_io PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_containers "test_containers")
set_tests_properties(test_containers PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_alignment_scores "test_alignment_scores")
set_tests_properties(test_alignment_scores PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_walker "test_walker")
set_tests_properties(test_walker PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_bowker "test_bowker")
set_tests_properties(test_bowker PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
