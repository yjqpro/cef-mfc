cmake_minimum_required(VERSION 3.15...3.17)


set(VCPKG_TARGET_TRIPLET "x86-windows" CACHE STRING "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

project(cefmfcdemo)


include(FetchContent)

# set (CEF_VERSION 95.7.18%2Bg0d6005e%2Bchromium-95.0.4638.69)
set (CEF_VERSION 95.7.18+g0d6005e+chromium-95.0.4638.69)
# set (CEF_VERSION 3.2840.1513.gf7effc7)
set (CEF_FILE_NAME "cef_binary_${CEF_VERSION}_windows32")

set (CEF_URL "https://cef-builds.spotifycdn.com/${CEF_FILE_NAME}.tar.bz2")
FetchContent_Declare(
  download_cef
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  URL ${CEF_URL}
  DOWNLOAD_NO_PROGRESS NO
  DOWNLOAD_NO_EXTRACT true
)

FetchContent_MakeAvailable(download_cef)

file(ARCHIVE_EXTRACT  INPUT "${CMAKE_BINARY_DIR}/downloads/${CEF_FILE_NAME}.tar.bz2")
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(PROXYAUTHCLIENT_ROOT "${CMAKE_SOURCE_DIR}/third_party/ProxyAuthClient")

set(CEF_ROOT "${CMAKE_BINARY_DIR}/${CEF_FILE_NAME}")

find_package(CEF REQUIRED)


add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

#
# Shared configuration.
#

# Target executable names.
set(CEF_TARGET "cefmfcdemo")
if(OS_MACOSX)
  set(CEF_HELPER_TARGET "cefmfcdemo Helper")
endif()

# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

# Determine the target output directory.
SET_CEF_TARGET_OUT_DIR()

set(CEFMFCDEMO_SRCS
    CefMfcDemo.cpp
    CefMfcDemo.h
    CefMfcDemo.rc
    CefMfcDemoDoc.cpp
    CefMfcDemoDoc.h
    CefMfcDemoView.cpp
    CefMfcDemoView.h
    CMakeLists.txt
    framework.h
    MainFrm.cpp
    MainFrm.h
    pch.cpp
    pch.h
    res
    Resource.h
    targetver.h
    ClientHandler.h
    ClientHandler.cpp
    CefWindowsHelpers.h
    CefWindowsHelpers.cpp
)


#
# Windows configuration.
#

if(OS_WINDOWS)
  # Executable target.
  add_executable(${CEF_TARGET} WIN32 ${CEFMFCDEMO_SRCS})
  add_dependencies(${CEF_TARGET} libcef_dll_wrapper)
  SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})
  target_link_libraries(${CEF_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${CEF_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
  endif()


  target_link_options(${CEF_TARGET} PRIVATE "/entry:wWinMainCRTStartup")

  # Add the custom manifest files to the executable.
  # ADD_WINDOWS_MANIFEST("${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
  COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")
endif()
