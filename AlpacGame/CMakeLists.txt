cmake_minimum_required(VERSION 3.8)
project(AlpacGame)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp entity/wolf/Wolf.h entity/wolf/Wolf.cpp state/StateMachine.cpp state/StateMachine.h state/StateMenu.cpp state/StateMenu.h state/State.h state/StateExit.cpp state/StateExit.h state/StateInitial.cpp state/StateInitial.h resources/ConfigWindow.cpp resources/ConfigWindow.h state/StateGame.cpp state/StateGame.h planet/planet.cpp planet/planet.h entity/player/Farmer.cpp entity/player/Farmer.h entity/alpaca/Alpaca.cpp entity/alpaca/Alpaca.h resources/ConfigGame.cpp resources/ConfigGame.h state/StateOption.cpp state/StateOption.h entity/Entity.h entity/EntityWarm.h entity/EntityCold.h entity/Mob.h entity/CollisionListener.cpp entity/CollisionListener.h)

add_executable(AlpacGame ${SOURCE_FILES})
target_link_libraries(AlpacGame Box2D sfml-graphics sfml-window sfml-system)
target_include_directories(AlpacGame PUBLIC plog/include)