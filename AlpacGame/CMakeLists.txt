cmake_minimum_required(VERSION 3.8)
project(AlpacGame)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp entity/wolf/Wolf.h entity/wolf/Wolf.cpp state/StateMachine.cpp state/StateMachine.h state/StateMenu.cpp state/StateMenu.h state/State.h state/StateExit.cpp state/StateExit.h state/StateInitial.cpp state/StateInitial.h resources/ConfigWindow.cpp resources/ConfigWindow.h state/StateGame.cpp state/StateGame.h entity/planet/planet.cpp entity/planet/planet.h entity/player/Farmer.cpp entity/player/Farmer.h entity/alpaca/Alpaca.cpp entity/alpaca/Alpaca.h resources/ConfigGame.cpp resources/ConfigGame.h state/StateOption.cpp state/StateOption.h entity/Entity.h entity/EntityWarm.h entity/EntityCold.h entity/Mob.h entity/CollisionListener.cpp entity/CollisionListener.h resources/SpriteInfo.h entity/shotgun/Shotgun.cpp entity/shotgun/Shotgun.h entity/Usable.h entity/shotgun/Bullet.cpp entity/shotgun/Bullet.h Resources/ConfigMenu.cpp Resources/ConfigMenu.h Resources/ConfigSound.cpp Resources/ConfigSound.h entity/Holdable.h scenery/DayCycle/DayCycle.cpp scenery/DayCycle/DayCycle.cpp scenery/DayCycle/DayCycle.h scenery/Scenery.h scenery/Sun/Sun.cpp scenery/Sun/Sun.h scenery/Sky/Sky.cpp scenery/Sky/Sky.h entity/trap/Trap.cpp entity/trap/Trap.h entity/cooldown/CooldownBarometer.cpp entity/cooldown/CooldownBarometer.h entity/HitPoint/HitPointBarometer.cpp entity/HitPoint/HitPointBarometer.h state/StatePause.cpp state/StatePause.h scenery/Trees/TreeHigh.cpp scenery/Trees/TreeHigh.h scenery/Trees/TreeMedium.cpp scenery/Trees/TreeMedium.h scenery/Trees/TreeLow.cpp scenery/Trees/TreeLow.h scenery/Cave/Cave.cpp scenery/Cave/Cave.h scenery/Stone/Stone.cpp scenery/Stone/Stone.h)

add_executable(AlpacGame ${SOURCE_FILES})
target_link_libraries(AlpacGame Box2D sfml-graphics sfml-window sfml-system tgui tgui-d)

target_include_directories(AlpacGame PUBLIC plog/include)