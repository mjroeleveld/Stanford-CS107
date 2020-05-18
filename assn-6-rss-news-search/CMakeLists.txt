cmake_minimum_required(VERSION 3.13)
project(rss_news_search)
set(CMAKE_CXX_STANDARD 14)

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES rss-news-search.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(rss_news_search ${SOURCE_FILES})