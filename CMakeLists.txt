cmake_minimum_required(VERSION 3.9)
project(DuelSandbox
	DESCRIPTION "idk description yet lmao"
	LANGUAGES CXX
	VERSION 1.0.0)

add_subdirectory(engine)

tz_add_executable(
	TARGET duel_sandbox
	SOURCES
		src/entity/api.cpp
		src/entity/api.hpp
		src/entity/creature.cpp
		src/entity/creature.hpp
		src/entity/generic_human.cpp
		src/entity/generic_human.hpp
		src/entity/player_human.cpp
		src/entity/player_human.hpp
		src/entity/system.cpp
		src/entity/system.hpp
		src/entity/system.inl

		src/renderer/terrain.cpp
		src/renderer/terrain.hpp

		src/skeleton/api.cpp
		src/skeleton/api.hpp
		src/skeleton/human_male.cpp
		src/skeleton/human_male.hpp

		src/main.cpp
)
add_shader(TARGET duel_sandbox SHADERS
	src/renderer/terrain.vertex.tzsl
	src/renderer/terrain.tesscon.tzsl
	src/renderer/terrain.tesseval.tzsl
	src/renderer/terrain.fragment.tzsl
)