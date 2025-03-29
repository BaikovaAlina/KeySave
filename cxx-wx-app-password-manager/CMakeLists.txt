cmake_minimum_required(VERSION 3.10)

project(lin-app-cxx-password-manager VERSION 1.0.0)

set(ROOT "/home/lina/PasswordManager")
set(RESOURCES "${ROOT}/resources")


file(READ "${RESOURCES}/token" TOKEN_RAW)
string(STRIP ${TOKEN_RAW} TOKEN)

find_package(wxWidgets REQUIRED COMPONENTS core base xml net)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(STATIC_JDK_LINK OFF)

set(OUTPUT_NAME ${PROJECT_NAME}-${CMAKE_BUILD_TYPE}-${PROJECT_VERSION})

execute_process(COMMAND wx-config --cxxflags OUTPUT_VARIABLE WX_FLAGS)
string(STRIP ${WX_FLAGS} CMAKE_CXX_FLAGS)

set(JDK_DIR /usr/lib/jvm/java-21-openjdk-amd64)
set(OPENSSL_DIR /usr/local/ssl)

add_definitions(-DJDK="${JDK_DIR}")

find_package(wxWidgets 3.2 REQUIRED core base xml)

add_definitions(-DRESOURCES="${RESOURCES}")
add_definitions(-DTOKEN="${TOKEN}")

if(STATIC_JDK_LINK MATCHES ON)

    add_definitions(-DSTATIC_JDK_LINK)

    set(OUTPUT_DIRECTORY ${RESOURCES}/jdk-lite/bin/server)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
else()
    add_definitions(-DDYNAMIC_JDK_LINK)
endif()

include_directories(${JDK_DIR}/include)
include_directories(${JDK_DIR}/include/linux)
include_directories(${OPENSSL_DIR}/include)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_link_options(-s)
endif()

link_directories(${OPENSSL_DIR}/lib64)

if(STATIC_JDK_LINK MATCHES ON)
  link_directories(${JDK_DIR}/lib)
endif()

add_executable(${OUTPUT_NAME}

    src/main.cpp
    src/jnicloudelib.cpp
    src/cipher.cpp
    src/xmlfile.cpp
    src/mainframe.cpp

    res/iv.o
    res/salt.o
)

target_link_libraries(${OUTPUT_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${OUTPUT_NAME} ssl)
target_link_libraries(${OUTPUT_NAME} crypto)

if(STATIC_JDK_LINK MATCHES ON)
    target_link_libraries(${OUTPUT_NAME} jvm)
endif()

