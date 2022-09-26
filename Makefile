all:
	g++ -D LINUX \
	src/Main/main.cpp \
	src/Screen/screen.cpp \
	src/Menus/menu.cpp \
	src/Menus/mainmenu.cpp \
	src/Menus/modeSelector.cpp \
	src/Menus/settingsMenu.cpp \
	src/Menus/controllsMenu.cpp \
	src/Modes/SnakeGame/snakeGame.cpp \
	src/Modes/SnakeGame/Models/snakeModel.cpp \
	src/Modes/SnakeGame/Models/foodModel.cpp \
	src/Modes/SnakeGame/Menus/pauseMenu.cpp \
	src/Modes/SnakeGame/Menus/snakeSettingsMenu.cpp \
	src/Modes/Evolution/evolution.cpp \
	src/Modes/Evolution/Field/map.cpp \
	src/Modes/Evolution/Menus/SubMenus/infoSubMenu.cpp \
	src/Modes/Evolution/Menus/SubMenus/toolsSubMenu.cpp \
	src/Modes/Evolution/Models/snakeEvolutionModel.cpp \
	src/Modes/Evolution/Models/foodEvolutionModel.cpp \
	-lncurses -o snake

	
