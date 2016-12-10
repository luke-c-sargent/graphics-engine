HEADERS= Debug.h Game.h GameWindow.h LinuxWindow.h LinuxTime.h TimeKeeper.h ./Renderer/Vulkan.h ./Window/Linux/XCB.h
SOURCES= Game.c++ GameWindow.c++ ./Renderer/Vulkan.c++ ./Window/Linux/XCB.c++
TESTFILE=test.c++
MAINFILE=main.c++


G++FLAGS= -lGL -lX11 -lXext -lvulkan -lxcb
DEBUGMSG= -D DEBUGMSGS
G++TESTFLAGS= -lxcb -lvulkan
C++STD= c++0x

test: $(HEADERS) $(SOURCES)
# execution conditional on successful compilation
ifeq ($(shell g++ $(DEBUGMSG) $(HEADERS) $(SOURCES) $(TESTFILE) -o test_exec -std=$(C++STD) $(G++TESTFLAGS); echo $$?),0)
	./test_exec
else
	@echo "\n*** ${REDFG}${WHITEBG}Compilation failed${RESETCOLOR}: execution suppressed ***"
endif

game: $(HEADERS) $(SOURCES)
	g++ $(HEADERS) $(SOURCES) $(MAINFILE) -o main_exec -std=$(C++STD) $(G++FLAGS)
	./main_exec

#text decoration variables
REDFG=`tput setaf 1`
WHITEBG=`tput setab 7`
RESETCOLOR=`tput sgr0`