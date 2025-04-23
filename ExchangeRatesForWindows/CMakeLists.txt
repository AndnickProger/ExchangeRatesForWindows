set (BOOST_ROOT "D:\\Boost\\boost_1_85_0\\boost_1_85_0")

set (CMAKE_CXX_STANDARD 20) 
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK ON)

find_package(Boost 1.85.0 REQUIRED COMPONENTS) 

include_directories (${Boost_INCLUDE_DIRS})

set_target_properties(Boost::dynamic_linking PROPERTIES
  INTERFACE_COMPILE_DEFINITIONS "BOOST_ALL_DYN_LINK")


add_executable (ExchangeRatesForWindows "main.cpp"  "ExchangeRates.cpp" "ExchangeRates.h" "headers.h")

#target_include_directories(ExchangeRatesForWindows PRIVATE ${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

target_link_libraries (ExchangeRatesForWindows
${Boost_LIBRARIES})

