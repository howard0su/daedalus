add_library(System OBJECT CompressedStream.cpp DataSink.cpp SystemInit.cpp)


list(APPEND daed_libs "System")
set(daed_libs ${daed_libs} PARENT_SCOPE)
