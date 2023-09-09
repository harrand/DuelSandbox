cmake_minimum_required(VERSION 3.9)
project(DuelSandbox
	DESCRIPTION "idk description yet lmao"
	LANGUAGES CXX
	VERSION 1.0.0)

add_subdirectory(engine)

tz_add_executable(
	TARGET duel_sandbox
	SOURCES
		src/main.cpp
)