project(huffman)

add_definitions(-std=c++17)
set(COMMON myHuffMan.cpp myHuffMan.h)
set(ENCODE Encoder.cpp)
set(DECODE Decoder.cpp)
add_executable(huffEncode ${COMMON} ${ENCODE})
add_executable(huffDecode ${COMMON} ${DECODE})