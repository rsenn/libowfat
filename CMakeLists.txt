set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.8)

project(libowfat C)

set(LIBOWFAT_VERSION 1.0.0)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckPrototypeDefinition)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(CheckTypeSize)
include(CheckCCompilerFlag)

# exec_program("${CMAKE_C_COMPILER}"
#   ARGS "-dumpmachine"
#   OUTPUT_VARIABLE CC_OUTPUT
#   RETURN_VALUE CC_RETVAL)
# message("Output: ${CC_OUTPUT}")
# 
if(NOT TCC AND NOT CYGWIN AND NOT UNIX)
  check_symbol_exists(__MINGW32__ "" MINGW32)
  if(NOT MINGW32)
    check_symbol_exists(__MINGW64__ "" MINGW64)
    if(NOT MINGW64)
    check_symbol_exists(__BORLANDC__ "" BORLANDC)
    if(NOT BORLANDC)
      check_symbol_exists(__LCC__ "" LCC)
    endif(NOT BORLANDC)
    endif(NOT MINGW64)
  endif(NOT MINGW32)
endif(NOT TCC AND NOT CYGWIN AND NOT UNIX)

if(MINGW32 OR MINGW64)
  set(MINGW TRUE)
  set(MSYS FALSE)
  set(CYGWIN FALSE)
else(MINGW32 OR MINGW64)
  check_symbol_exists(__MSYS__ "" MSYS)
  if(NOT MSYS) 
    check_symbol_exists(__CYGWIN__ "" CYGWIN)
  endif(NOT MSYS)
endif(MINGW32 OR MINGW64)

if(MINGW OR BORLANDC OR TCC OR LCC OR MSVC)
  set(WINDOWS TRUE)
  set(MSYS FALSE)
  set(CYGWIN FALSE)
endif()
#check_symbol_exists(_MSC_VER "" MSVC)

function(sizeof_type TYPE VAR)
  check_type_size("${TYPE}" "${VAR}")
  message("${TYPE} size: ${${VAR}}")
endfunction()

#sizeof_type("int" INT_SIZE)
#sizeof_type("long" LONG_SIZE)
#sizeof_type("long long" LONG_SIZE)
#sizeof_type("size_t" SIZE_T_SIZE)
#sizeof_type("ptrdiff_t" PTRDIFF_T_SIZE)
sizeof_type("void*" VOID_PTR_SIZE)

if(VOID_PTR_SIZE EQUAL 8)
  set(BITS 64)
elseif(VOID_PTR_SIZE EQUAL 4)
  set(BITS 32)
elseif(VOID_PTR_SIZE EQUAL 2)
  set(BITS 16)
endif()

if(MSYS)
  set(SYSTEM "msys${BITS}")
elseif(CYGWIN)
  set(SYSTEM "cygwin${BITS}")
elseif(MINGW)
  set(SYSTEM "mingw${BITS}")
elseif(MSVC OR WIN32 OR WIN64)
  set(SYSTEM "win${BITS}")
endif()

if(MSYS OR CYGWIN OR MINGW OR MSVC OR WIN32 OR WIN64)
set(WINDOWS TRUE)
endif()

  set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS_MINSIZEREL)
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" FLAG "${${CompilerFlag}}")
    unset(${CompilerFlag} CACHE)
    set(${CompilerFlag} "${FLAG}" CACHE STRING "Compiler flags ${CompilerFlag}")
    message("${CompilerFlag}: ${${CompilerFlag}}")
  endforeach()

if(WINDOWS)
  link_libraries(ws2_32 wsock32)
endif(WINDOWS)

message("SYSTEM: ${SYSTEM}")

check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)

if(NOT HAVE_INTTYPES_H)
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/msvc/inttypes")
  list(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/msvc/inttypes")
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}/msvc/inttypes")
  set(INTTYPES_CFLAGS "${INTTYPES_CFLAGS} -I../../../../msvc/inttypes -Imsvc/inttypes")

  if(NOT HAVE_STDINT_H)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/msvc/stdint")
    list(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/msvc/stdint")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}  -I${CMAKE_CURRENT_SOURCE_DIR}/msvc/stdint")
    set(INTTYPES_CFLAGS "${INTTYPES_CFLAGS} -I../../../../msvc/stdint -Imsvc/stdint")
  endif()
endif()

if(BORLANDC)
  check_c_compiler_flag(-An HAVE_BCC_C99_FLAG)
  if(HAVE_BCC_C99_FLAG)
    add_definitions(-An)
  endif()
  check_c_compiler_flag(-q HAVE_BCC_Q_FLAG)
  if(HAVE_BCC_Q_FLAG)
    add_definitions(-q)
  endif()
endif()

if(MSVC)
  set(INLINE_CFLAGS "-Dinline=__inline")
endif()

#if(BORLANDC)
#set(INLINE_CFLAGS "-Dinline=__inline")
#endif()
#
if(INLINE_CFLAGS)
  add_definitions("${INLINE_CFLAGS}")
endif()


function(output_have NAME)
  if(HAVE_${NAME})
    message("  ${NAME} enabled")
  endif(HAVE_${NAME})
endfunction(output_have NAME)


function(write_have_h NAME)
  string(TOLOWER "${ARGV0}" HAVE_H)
  if(NOT ARGV1 AND NOT ARGV2)
    set(CONTENT_TRUE "#define HAVE_${ARGV0} 1")
    set(CONTENT_FALSE "#undef HAVE_${ARGV0}")
  else()
    set(CONTENT_TRUE "${ARGV1}\n")
    set(CONTENT_FALSE "${ARGV2}\n")
  endif()
  if(NOT ARGV3)
    set(OUTPUT_FILE "have${HAVE_H}.h")
  else()
    set(OUTPUT_FILE "${ARGV3}")
  endif()
  if(HAVE_${ARGV0})
    file(WRITE "${CMAKE_BINARY_DIR}/${OUTPUT_FILE}" "${CONTENT_TRUE}\n")
  else(HAVE_${ARGV0})
    file(WRITE "${CMAKE_BINARY_DIR}/${OUTPUT_FILE}" "${CONTENT_FALSE}\n")
  endif(HAVE_${ARGV0})
endfunction(write_have_h)

### Checks for alloca (tryalloca.c)

#check_function_exists(alloca HAVE_ALLOCA)
check_include_file(alloca.h HAVE_ALLOCA_H)

check_prototype_definition(alloca "extern void *alloca (size_t __size)" "NULL" "alloca.h;stdlib.h" HAVE_ALLOCA)

write_have_h(ALLOCA "#include <alloca.h>" "")

### Checks for bsdsf (trybsdsf.c)

if(NOT WINDOWS)
  #check_include_file(netinet/in.h NETINET_IN_H)
  check_include_file(netinet/tcp.h NETINET_TCP_H)
  #check_include_file(sys/socket.h SYS_SOCKET_H)
  #check_include_file(sys/types.h SYS_TYPES_H)
  #check_include_file(sys/uio.h SYS_UIO_H)

  check_function_exists(sendfile HAVE_BSDSF)
endif(NOT WINDOWS)
write_have_h(BSDSF)

### Checks for devpoll (trydevpoll.c)

if(NOT WINDOWS)
  check_include_file(sys/devpoll.h SYS_DEVPOLL_H)
  check_include_file(sys/errno.h SYS_ERRNO_H)

  check_symbol_exists(DP_POLL "sys/devpoll.h;sys/ioctl.h" HAVE_DEVPOLL)
endif(NOT WINDOWS)
write_have_h(DEVPOLL)

### Checks for epoll (tryepoll.c)
if(NOT WINDOWS)
  check_include_file(sys/epoll.h SYS_EPOLL_H)

  check_function_exists(epoll_wait HAVE_EPOLL)

endif(NOT WINDOWS)
write_have_h(EPOLL "#define HAVE_EPOLL 1")

### Checks for inline (tryinline.c)

write_have_h(INLINE)

### Checks for ip6 (tryip6.c)
#check_include_file(netinet/in.h NETINET_IN_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)

check_symbol_exists(PF_INET6 "netinet/in.h;sys/socket.h;sys/types.h" HAVE_IP6)

write_have_h(IP6 "#define LIBC_HAS_IP6")

### Checks for kqueue (trykqueue.c)
if(NOT WINDOWS)
  check_include_file(sys/event.h SYS_EVENT_H)
  #check_include_file(sys/time.h SYS_TIME_H)
  #check_include_file(sys/types.h SYS_TYPES_H)

  check_function_exists(kevent HAVE_KQUEUE)
endif(NOT WINDOWS)
write_have_h(KQUEUE)

### Checks for n2i (tryn2i.c)
if(NOT WINDOWS)
  check_include_file(net/if.h NET_IF_H)
  #check_include_file(sys/socket.h SYS_SOCKET_H)
  #check_include_file(sys/types.h SYS_TYPES_H)

  check_function_exists(if_indextoname HAVE_N2I)
endif(NOT WINDOWS)
write_have_h(N2I "#define HAVE_N2I")

### Checks for poll (trypoll.c)
#check_include_file(sys/types.h SYS_TYPES_H)

check_function_exists(poll HAVE_POLL)

### Checks for scope (tryscope.c)
#check_include_file(netinet/in.h NETINET_IN_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)
check_struct_has_member(sockaddr_in6 sin6_scope_id "sys/types.h;netinet/in.h" HAVE_SCOPE)

write_have_h(SCOPE "#define LIBC_HAS_SCOPE_ID")

### Checks for sendfile (trysendfile.c)
if(NOT WINDOWS)
  check_include_file(linux/unistd.h LINUX_UNISTD_H)
  check_include_file(sys/sendfile.h SYS_SENDFILE_H)
  #check_include_file(sys/socket.h SYS_SOCKET_H)
  #check_include_file(sys/types.h SYS_TYPES_H)
  #check_include_file(sys/uio.h SYS_UIO_H)
  check_function_exists(sendfile HAVE_SENDFILE)
endif(NOT WINDOWS)
write_have_h(SENDFILE "#define HAVE_SENDFILE")

### Checks for sigio (trysigio.c)
if(NOT WINDOWS)
  check_include_file(sys/poll.h SYS_POLL_H)
  #check_include_file(sys/types.h SYS_TYPES_H)
  check_symbol_exists(F_SETSIG "sys/types.h;fcntl.h;signal.h" HAVE_SIGIO)
endif(NOT WINDOWS)
write_have_h(SIGIO "#define HAVE_SIGIO")

### Checks for sl (trysl.c)
#check_include_file(netinet/in.h NETINET_IN_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)
set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h;sys/socket.h;netinet/in.h")
check_type_size(socklen_t HAVE_SL)

write_have_h(SL)

### Checks for socket (trysocket.c)
#check_include_file(netinet/in.h NETINET_IN_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)
check_function_exists(socket HAVE_SOCKET)

### Checks for sysel (trysysel.c)
check_include_file(sys/select.h SYS_SELECT_H)
#check_include_file(sys/time.h SYS_TIME_H)
#check_include_file(sys/types.h SYS_TYPES_H)
check_function_exists(select HAVE_SELECT)

write_have_h(SELECT 
  "#ifndef SELECT_H\n#define SELECT_H\n\n/* sysdep: +sysselect */\n\n#include <sys/types.h>\n#include <sys/time.h>\n#include <sys/select.h>\n\n/* braindead BSD uses bzero in FD_ZERO but doesn't #include string.h */\n#include <string.h>\n\nextern int select();\n\n#endif"
  "#ifndef SELECT_H\n#define SELECT_H\n\n/* sysdep: -sysselect */\n\n#include <sys/types.h>\n#include <sys/time.h>\nextern int select();\n\n#endif" 
  "select.h")

### Checks for poll
check_function_exists(poll HAVE_POLL)

write_have_h(POLL
  "/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */\n#ifndef IOPAUSE_H\n#define IOPAUSE_H\n\n/* sysdep: +poll */\n#define IOPAUSE_POLL\n\n#include <sys/types.h>\n#include <sys/poll.h>\n\ntypedef struct pollfd iopause_fd;\n#define IOPAUSE_READ POLLIN\n#define IOPAUSE_WRITE POLLOUT\n\n#include \"libowfat/taia.h\"\n\nextern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);\n\n#endif"
  "/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */\n#ifndef IOPAUSE_H\n#define IOPAUSE_H\n\n/* sysdep: -poll */\n\ntypedef struct {\n  int fd;\n  short events;\n  short revents;\n} iopause_fd;\n\n#define IOPAUSE_READ 1\n#define IOPAUSE_WRITE 4\n\n#include \"libowfat/taia.h\"\n\nextern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);\n\n#endif"
  "iopause.h")

### Checks for uint128 (tryuint128.c)
set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h" )
check_type_size(__uint128_t HAVE_UINT128)

write_have_h(UINT128 "#define HAVE_UINT128")

foreach(N SIGIO ALLOCA)
  output_have(${N})
endforeach()

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

try_compile(RCOMP_ENT 
  "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/ent.c"
  CMAKE_FLAGS 
  "-DINCLUDE_DIRECTORIES=${CMAKE_BINARY_DIR};${CMAKE_SOURCE_DIR} -DCOMPILE_DEFINITIONS=${INTTYPES_CFLAGS};${INLINE_CFLAGS}"
  COMPILE_DEFINITIONS "${INTTYPES_CFLAGS} ${INLINE_CFLAGS}"
     OUTPUT_VARIABLE COMPILE_ENT
  COPY_FILE "${CMAKE_BINARY_DIR}/ent")

#try_run(RUN_ENT COMP_ENT 
#  "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/ent.c"
#  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${CMAKE_BINARY_DIR}"
#  COMPILE_OUTPUT_VARIABLE COMPOUT_ENT
#  RUN_OUTPUT_VARIABLE RUNOUT_ENT)
#
#  message(STATUS "Compile output variable ${COMPOUT_ENT}")
#  message(STATUS "Run output variable ${RUNOUT_ENT}")
#
#if(COMP_ENT)
#  message(STATUS "Compiling ent.c succeeded")
#else()
#  message(STATUS "Compiling ent.c failed")
#endif()
#if(RUN_ENT)
#  message(STATUS "Running ent.c succeeded")
#else()
#  message(STATUS "Running ent.c failed")
#endif()

execute_process(COMMAND "${CMAKE_BINARY_DIR}/ent" "${CMAKE_SOURCE_DIR}/entities.json" 
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  OUTPUT_VARIABLE OUT_ENT
  RESULT_VARIABLE RESULT_ENT)

message(STATUS "Compile 'ent' result: ${RCOMP_ENT}")
message(STATUS "Compile 'ent' output: ${COMPILE_ENT}")
message(STATUS "Command 'ent' output: ${OUT_ENT}")
message(STATUS "Command 'ent' result: ${RESULT_ENT}")

set(INCDIR "${CMAKE_CURRENT_BINARY_DIR}/libowfat")
file(MAKE_DIRECTORY "${INCDIR}")
foreach(H array buffer byte case cdb cdb_make compiletimeassert critbit dns errmsg fmt iarray io iob ip4 ip6 isset mmap ndelay open openreadclose rangecheck readclose safemult scan socket str stralloc tai taia textcode uint16 uint32 uint64 va_narg)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${H}.h" "${INCDIR}/${H}.h" COPYONLY)
  list(APPEND INCLUDES "${INCDIR}/${H}.h")
endforeach()
foreach(H iopause select entities)
  configure_file("${CMAKE_CURRENT_BINARY_DIR}/${H}.h" "${INCDIR}/${H}.h" COPYONLY)
  list(APPEND INCLUDES "${INCDIR}/${H}.h")
endforeach()


file(GLOB SOURCES array/array_*.c array/iarray_*.c buffer/buffer_*.c buffer/errmsg_*.c byte/byte_*.c case/case_*.c cdb/*.c critbit/*.c dns/*.c fmt/fmt_*.c io/io_*.c mmap/mmap_*.c mult/*.c open/*.c open/open_*.c scan/scan_*.c socket/*.c str/str_*.c stralloc/stralloc_*.c tai/tai_*.c taia/taia_*.c textcode/*.c uint/uint16_*.c uint/uint32_*.c uint/uint64_*.c unix/*.c unix/ndelay_*.c)

set(BUILD_SHARED_LIBS ON)

option(ENABLE_SHARED "build shared library" OFF)
option(ENABLE_STATIC "build static library" ON)
unset(TARGETS)

if(ENABLE_SHARED)
  set(DLLPREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}")
  set(DLLSUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")

  list(APPEND TARGETS libowfat-shared)
  add_library(libowfat-shared SHARED ${SOURCES})
endif(ENABLE_SHARED)
if(ENABLE_STATIC)
  list(APPEND TARGETS libowfat-static)
  add_library(libowfat-static STATIC ${SOURCES})
endif(ENABLE_STATIC)

set(LIBPREFIX "${CMAKE_STATIC_LIBRARY_PREFIX}")
set(LIBSUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}")
set(IMPPREFIX "${CMAKE_IMPORT_LIBRARY_PREFIX}")
set(IMPSUFFIX "${CMAKE_IMPORT_LIBRARY_SUFFIX}")
set(DLLVERSION "1")

if(MSYS)
  set(DLLPREFIX "msys-")
  set(DLLSUFFIX "-${DLLVERSION}.dll")
  set(LIBPREFIX "lib")
  set(LIBSUFFIX ".a")
endif(MSYS)
if(CYGWIN)
  set(DLLPREFIX "cyg")
  set(DLLSUFFIX "-${DLLVERSION}.dll")
  set(LIBPREFIX "lib")
  set(LIBSUFFIX ".a")
endif(CYGWIN)

if(NOT IMPPREFIX)  
  set(IMPPREFIX "lib")
endif(NOT IMPPREFIX)  
if(NOT IMPSUFFIX)  
  set(IMPSUFFIX ".dll.a")
endif(NOT IMPSUFFIX)  

foreach(T ${TARGETS})
  set_target_properties(${T} PROPERTIES
    OUTPUT_NAME "owfat"
    PREFIX "${DLLPREFIX}"
    SUFFIX "${DLLSUFFIX}"
    DLLVERSION "${DLLVERSION}"
    IMPORT_PREFIX "${IMPPREFIX}"
    IMPORT_SUFFIX "${IMPSUFFIX}"
    )
endforeach()

write_have_h(ALLOCA "#include <stdlib.h>
#include <alloca.h>
#include <malloc.h>")

install(TARGETS ${TARGETS}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(FILES ${INCLUDES} DESTINATION include/libowfat)
