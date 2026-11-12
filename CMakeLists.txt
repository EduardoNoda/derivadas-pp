cmake_minimum_required(VERSION 3.16)
project(CalculadoraDerivadas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SFML_FETCH "Baixar a SFML 3.0.0 via FetchContent" OFF)

add_library(core STATIC
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/factory/FunctionFactory.cpp
    src/formatter/Formatter.cpp
    src/application/DerivativeService.cpp
)
target_include_directories(core PUBLIC include)
if(MSVC)
    target_compile_options(core PRIVATE /W4)
else()
    target_compile_options(core PRIVATE -Wall -Wextra)
endif()

enable_testing()
foreach(name lexer parser math pipeline service)
    add_executable(test_${name} tests/test_${name}.cpp)
    target_link_libraries(test_${name} PRIVATE core)
    add_test(NAME ${name} COMMAND test_${name})
endforeach()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/apps/main_console.cpp)
    add_executable(derivadas_console apps/main_console.cpp)
    target_link_libraries(derivadas_console PRIVATE core)
endif()

if(SFML_FETCH)
    include(FetchContent)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON)
    FetchContent_MakeAvailable(SFML)
    set(SFML_FOUND TRUE)
else()
    find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
endif()

if(SFML_FOUND)
    add_library(ui STATIC src/ui/CalculatorWindow.cpp)
    target_link_libraries(ui PUBLIC core SFML::Graphics)
    if(MSVC)
        target_compile_options(ui PRIVATE /W4)
    else()
        target_compile_options(ui PRIVATE -Wall -Wextra)
    endif()

    add_executable(derivadas apps/main_sfml.cpp)
    target_link_libraries(derivadas PRIVATE ui)

    if(WIN32 AND NOT CMAKE_VERSION VERSION_LESS 3.21)
        add_custom_command(TARGET derivadas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:derivadas> $<TARGET_FILE_DIR:derivadas>
            COMMAND_EXPAND_LISTS)
    endif()
else()
    message(WARNING "SFML 3 nao encontrada: a interface grafica NAO sera compilada. "
                    "Use -DSFML_FETCH=ON ou -DCMAKE_PREFIX_PATH=<pasta da SFML>.")
endif()
