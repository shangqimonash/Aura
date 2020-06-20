cmake_minimum_required(VERSION 3.16)
project(Aura)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(OpenSSL REQUIRED)

INCLUDE_DIRECTORIES(. BF BF/Hash GGM Util)

ADD_EXECUTABLE(Aura main.cpp SSEClient.cpp SSEServer.cpp GGM/GGMTree.cpp BF/Hash/SpookyV2.cpp Util/CommonUtil.c)

ADD_EXECUTABLE(AESTest Test/AESTest.cpp Util/CommonUtil.c)

ADD_EXECUTABLE(BloomFilterTest BF/Hash/SpookyV2.cpp)

TARGET_LINK_LIBRARIES(Aura OpenSSL::Crypto)

TARGET_LINK_LIBRARIES(AESTest OpenSSL::Crypto)

