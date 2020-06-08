OPENGLLIB= -framework OpenGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include

BIN=bin/Earth
SRCS=Earth.cpp utility/readin.cpp utility/shader.cpp utility/stb_image.cpp utility/texture2D.cpp utility/camera.cpp utility/sphere.cpp utility/skybox.cpp 
INCLUDES=utility/readin.hpp utility/shader.hpp utility/stb_image.h utility/texture2D.hpp utility/camera.hpp utility/sphere.hpp utility/skybox.hpp

all: $(BIN)
$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)	
clean:
	rm -f *~ *.o $(BIN1) 
