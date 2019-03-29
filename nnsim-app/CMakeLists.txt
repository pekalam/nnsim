cmake_minimum_required(VERSION 3.8.2)

project(nnsim-app)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS 
	Widgets 
	Network 
	Charts
	Concurrent
	OpenGL
	Qml
	Quick
	QuickWidgets
	WebSockets
	Test
	PrintSupport)


file(GLOB_RECURSE NSIMAPP_SOURCES "src/*\.cpp" "src/*\.h")
file(GLOB_RECURSE NSIMAPP_RES "resources/*\.qrc")
file(GLOB_RECURSE NSIMAPP_UI "ui/*\.ui")

file(GLOB_RECURSE LIB_INCLUDES "lib/*\.h")
file(GLOB_RECURSE LIB_SOURCES "lib/*\.cpp")

source_group("src" FILES ${NSIMAPP_SOURCES})
source_group("resources" FILES ${NSIMAPP_RES})

add_executable(nsim-app ${NSIMAPP_SOURCES} ${NSIMAPP_RES} ${NSIMAPP_UI} ${LIB_INCLUDES} ${LIB_SOURCES})



target_link_libraries(nsim-app
			nnsim::nn-library
			Qt5::Widgets 
			Qt5::Network 
			Qt5::OpenGL
			Qt5::Concurrent
			Qt5::Charts 
			Qt5::Qml
			Qt5::Quick
			Qt5::QuickWidgets
			Qt5::WebSockets
			Qt5::Test
			Qt5::PrintSupport)

