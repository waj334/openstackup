# Various settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(Qt5
	COMPONENTS
		Core
		Svg
		Widgets
	REQUIRED
)

find_package(Boost 1.75.0 REQUIRED)
find_package(unofficial-http-parser REQUIRED)
find_package(fmt REQUIRED)
find_package(restinio CONFIG REQUIRED)

set(srcs
	delaymatchsolver.cxx
	enums.cxx
	layer.cxx
	main.cxx
	material.cxx
	materialmanager.cxx
	materialmodel.cxx
	net.cxx
	netclass.cxx
	netclassmodel.cxx
	netmodel.cxx
	permittivitydelegate.cxx
	permittivitymodel.cxx
	pluginserver.cxx
	sessionmanager.cxx
	signalpropagationdelaymodel.cxx
	signalpropagationdelayproxymodel.cxx
	stackupdelegate.cxx
	stackupmodel.cxx
	stackupproxymodel.cxx
	uiabout.cxx
	uimainwindow.cxx
	uimaterials.cxx
	uinetproperties.cxx
	uistackup.cxx
)

set(ui_files
	uiabout.ui
	uimainwindow.ui
	uimaterials.ui
	uinetproperties.ui
	uistackup.ui
)

set(qrc_files
	resources.qrc
)

add_executable(openstackup ${srcs} ${ui_files} ${qrc_files} "delaymatchsolver.h")
target_link_libraries(openstackup
	PRIVATE
		Qt5::Core
		Qt5::Widgets
		Qt5::Svg
		restinio::restinio
)

target_include_directories(openstackup
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CPP_HTTPLIB_INCLUDE_DIRS}
	)