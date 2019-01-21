# CMake generated Testfile for 
# Source directory: /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0/test
# Build directory: /Users/ali_nayeem/Projects/MO-Phylogenetics/lib/Bpp/bpp-phyl-2.1.0/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_tree "test_tree")
set_tests_properties(test_tree PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_models "test_models")
set_tests_properties(test_models PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_detailed_simulations "test_detailed_simulations")
set_tests_properties(test_detailed_simulations PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_simulations "test_simulations")
set_tests_properties(test_simulations PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_parsimony "test_parsimony")
set_tests_properties(test_parsimony PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_likelihood "test_likelihood")
set_tests_properties(test_likelihood PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_likelihood_nh "test_likelihood_nh")
set_tests_properties(test_likelihood_nh PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_likelihood_clock "test_likelihood_clock")
set_tests_properties(test_likelihood_clock PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_mapping "test_mapping")
set_tests_properties(test_mapping PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_mapping_codon "test_mapping_codon")
set_tests_properties(test_mapping_codon PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_nhx "test_nhx")
set_tests_properties(test_nhx PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
add_test(test_bowker "test_bowker")
set_tests_properties(test_bowker PROPERTIES  ENVIRONMENT "DYLD_LIBRARY_PATH=:../src")
