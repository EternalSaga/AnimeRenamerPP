add_executable (TestCppWinrt "main.cpp")
 find_package(cppwinrt CONFIG REQUIRED)
target_link_libraries(TestCppWinrt PRIVATE Microsoft::CppWinRT)
