#Generated by VisualGDB project wizard.
#Note: VisualGDB will automatically update this file when you add new sources to the project.

cmake_minimum_required(VERSION 2.7)
project(network_libevent)
set(LIBRARIES_FROM_REFERENCES "")
add_executable(network_libevent Acceptor.cpp EventReactor.cpp IoHandler.cpp network_libevent.cpp SessionHandler.cpp SessionManager.cpp)
target_link_libraries(network_libevent -levent -lpthread "${LIBRARIES_FROM_REFERENCES}")
