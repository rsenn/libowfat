project(libowfat C)

set(LIBOWFAT_VERSION 1.0.0)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckPrototypeDefinition)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(CheckTypeSize)

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

#check_include_file(netinet/in.h NETINET_IN_H)
check_include_file(netinet/tcp.h NETINET_TCP_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)
#check_include_file(sys/uio.h SYS_UIO_H)

check_function_exists(sendfile HAVE_BSDSF)

write_have_h(BSDSF)

### Checks for devpoll (trydevpoll.c)

check_include_file(sys/devpoll.h SYS_DEVPOLL_H)
check_include_file(sys/errno.h SYS_ERRNO_H)

check_symbol_exists(DP_POLL "sys/devpoll.h;sys/ioctl.h" HAVE_DEVPOLL)

write_have_h(DEVPOLL)

### Checks for epoll (tryepoll.c)
check_include_file(sys/epoll.h SYS_EPOLL_H)

check_function_exists(epoll_wait HAVE_EPOLL)

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
check_include_file(sys/event.h SYS_EVENT_H)
#check_include_file(sys/time.h SYS_TIME_H)
#check_include_file(sys/types.h SYS_TYPES_H)

check_function_exists(kevent HAVE_KQUEUE)

write_have_h(KQUEUE)

### Checks for n2i (tryn2i.c)
check_include_file(net/if.h NET_IF_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)

check_function_exists(if_indextoname HAVE_N2I)

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
check_include_file(linux/unistd.h LINUX_UNISTD_H)
check_include_file(sys/sendfile.h SYS_SENDFILE_H)
#check_include_file(sys/socket.h SYS_SOCKET_H)
#check_include_file(sys/types.h SYS_TYPES_H)
#check_include_file(sys/uio.h SYS_UIO_H)
check_function_exists(sendfile HAVE_SENDFILE)

write_have_h(SENDFILE "#define HAVE_SENDFILE")

### Checks for sigio (trysigio.c)
check_include_file(sys/poll.h SYS_POLL_H)
#check_include_file(sys/types.h SYS_TYPES_H)
check_symbol_exists(SIGIO "sys/types.h;signal.h" HAVE_SIGIO)

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
  "/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */\n#ifndef IOPAUSE_H\n#define IOPAUSE_H\n\n/* sysdep: +poll */\n#define IOPAUSE_POLL\n\n#include <sys/types.h>\n#include <sys/poll.h>\n\ntypedef struct pollfd iopause_fd;\n#define IOPAUSE_READ POLLIN\n#define IOPAUSE_WRITE POLLOUT\n\n#include \"taia.h\"\n\nextern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);\n\n#endif"
  "/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */\n#ifndef IOPAUSE_H\n#define IOPAUSE_H\n\n/* sysdep: -poll */\n\ntypedef struct {\n  int fd;\n  short events;\n  short revents;\n} iopause_fd;\n\n#define IOPAUSE_READ 1\n#define IOPAUSE_WRITE 4\n\n#include \"taia.h\"\n\nextern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);\n\n#endif"
  "iopause.h")

### Checks for uint128 (tryuint128.c)
set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h" )
check_type_size(__uint128_t HAVE_UINT128)

write_have_h(UINT128 "#define HAVE_UINT128")

include_directories(".")
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_BINARY_DIR}")

try_compile(RCOMP_ENT 
  "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/ent.c"
  CMAKE_FLAGS "-DINCLUDE_DIRECTORIES=${CMAKE_BINARY_DIR}" "-DLINK_FLAGS=-static"
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


file(GLOB SOURCES array/*.c buffer/*.c byte/*.c case/*.c cdb/*.c critbit/*.c dns/*.c fmt/*.c io/*.c mmap/*.c mult/*.c open/*.c scan/*.c socket/*.c str/*.c stralloc/*.c tai/*.c taia/*.c textcode/*.c uint/*.c unix/*.c)

set(BUILD_SHARED_LIBS ON)

add_library(owfat ${SOURCES})
set_target_properties(owfat PROPERTIES
  SOVERSION "${LIBOWFAT_VERSION}"
)

write_have_h(ALLOCA "#include <stdlib.h>
#include <alloca.h>
#include <malloc.h>")


install(TARGETS owfat 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)


