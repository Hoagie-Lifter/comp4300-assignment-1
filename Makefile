# ---- per-project settings ----
SFML   = E:/libraries/SFML-3.1.0
IMGUI  = libs/imgui
DLLS   = libs/sfml
LIBS   = -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

# ---- generic part ----
CXX      = g++
CXXFLAGS = -std=c++20 -Iinc -I$(SFML)/include -I$(IMGUI) -include imconfig-SFML.h -MMD -MP
SRCS     = $(wildcard src/*.cpp $(IMGUI)/*.cpp $(IMGUI)/misc/cpp/*.cpp)
OBJS     = $(addprefix build/,$(notdir $(SRCS:.cpp=.o)))
vpath %.cpp $(dir $(SRCS))

.PHONY: run clean

main: $(OBJS)
	$(CXX) $(OBJS) -o main -L$(SFML)/lib $(LIBS)

build/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: main
	copy $(subst /,\,$(DLLS))\*.dll .
	main.exe
	-del $(notdir $(wildcard $(DLLS)/*.dll))

clean:
	-del /Q $(subst /,\,$(OBJS) $(OBJS:.o=.d)) main.exe

-include $(OBJS:.o=.d)