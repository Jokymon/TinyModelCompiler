cmake_minimum_required(VERSION 3.13)

project(xml2 LANGUAGES C)
include(CheckIncludeFile)

set(VERSION "2.9.10")
set(LIBXML_VERSION_NUMBER 20910)
set(LIBXML_VERSION_EXTRA GIT)
set(WITH_TRIO 0)
set(WITH_THREADS 1)
set(WITH_THREAD_ALLOC 1)
set(WITH_TREE 1)
set(WITH_OUTPUT 1)
set(WITH_PUSH 1)
set(WITH_READER 1)
set(WITH_PATTERN 1)
set(WITH_WRITER 1)
set(WITH_SAX1 1)
set(WITH_FTP 0)
set(WITH_HTTP 0)
set(WITH_VALID 1)
set(WITH_HTML 0)
set(WITH_LEGACY 0)
set(WITH_C14N 1)
set(WITH_CATALOG 1)
set(WITH_DOCB 0)
set(WITH_XPATH 1)
set(WITH_XPTR 1)
set(WITH_XINCLUDE 1)
set(WITH_ICONV 0)
set(WITH_ICU 0)
set(WITH_ISO8859X 0)
set(WITH_DEBUG 0)
set(WITH_MEM_DEBUG 0)
set(WITH_RUN_DEBUG 0)
set(WITH_REGEXPS 0)
set(WITH_SCHEMAS 0)     # seem to depend on REGEXPS
set(WITH_SCHEMATRON 0)
set(WITH_MODULES 0)
set(WITH_ZLIB 0)
set(WITH_LZMA 0)
CHECK_INCLUDE_FILE(ctype.h HAVE_CTYPE_H)
CHECK_INCLUDE_FILE(errno.h HAVE_ERRNO_H)
CHECK_INCLUDE_FILE(float.h HAVE_FLOAT_H)
CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILE(math.h HAVE_MATH_H)
CHECK_INCLUDE_FILE(stdarg.h HAVE_STDARG_H)
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE(time.h HAVE_TIME_H)
CHECK_INCLUDE_FILE(fcntl.h HAVE_FCNTL_H)

configure_file(
    config.h.in
    config.h
)

configure_file(
    include/libxml/xmlversion.h.in
    include/libxml/xmlversion.h
)

add_library(xml2)

target_include_directories(xml2
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/include/
)

file(GLOB LIBXML2_HEADERS ${CMAKE_SOURCE_DIR}/include/libxml/*.h)
list(APPEND LIBXML2_HEADERS ${CMAKE_BINARY_DIR}/include/libxml/xmlversion.h)

target_sources(xml2
    PRIVATE
        SAX.c
        entities.c
        encoding.c
        error.c
        parserInternals.c
		parser.c
        tree.c
        hash.c
        list.c
        xmlIO.c
        xmlmemory.c
        uri.c
		valid.c
        xlink.c
        HTMLparser.c
        HTMLtree.c
        debugXML.c
        xpath.c
		xpointer.c
        xinclude.c
        nanohttp.c
        nanoftp.c
		catalog.c
        globals.c
        threads.c
        c14n.c
        xmlstring.c
        buf.c
		xmlregexp.c
        xmlschemas.c
        xmlschemastypes.c
        xmlunicode.c
		xmlreader.c
        relaxng.c
        dict.c
        SAX2.c
		xmlwriter.c
        legacy.c
        chvalid.c
        pattern.c
        xmlsave.c
		xmlmodule.c
        schematron.c
        xzlib.c
)

set_target_properties(xml2
    PROPERTIES
    PUBLIC_HEADER "${LIBXML2_HEADERS}"
)

install(
    TARGETS xml2
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml"
)
