#
#
#
# MAKEFILE FOR libLevGC
#
#
#

INSTALL_PATH = /usr/lib

INSTALL_DIR = $(INSTALL_PATH)/LevGC


#use "g++" to compile source files.
CC = g++

#use "g++" to link the object files.
LD = ld

#Compiler flags:
CFLAGS = -DOUTPUT_DEBUG -D_LINUX -Wall -pedantic  `sdl-config --cflags` `freetype-config --cflags` -DTIXML_USE_STL=1 -DLGC_USE_FTGL=1

#Linker flags:
LDFLAGS = $(shell sdl-config --libs)

#Dependency generator:
MDEPEND = g++ -M

# system commands
RM = /bin/rm -f
CP = /bin/cp
LDCONFIG = /sbin/ldconfig
MKDIR = /bin/mkdir

# dirs to search for files in
VPATH = ..

# The list of source files:
SRCS = tinyxml.cpp tinystr.cpp tinyxmlparser.cpp tinyxmlerror.cpp Commaize.cpp Delay.cpp RenderObject.cpp RenderMesh.cpp Material.cpp Light.cpp Camera.cpp RenderingLayer.cpp Settings.cpp Decal.cpp ImgButton.cpp TextBox.cpp Slider.cpp DropBox.cpp Label.cpp Button.cpp FaderMessageBox.cpp Fader.cpp DrawingTools.cpp GameState.cpp CoreInterface.cpp Interpolator.cpp Texture.cpp MusicTrack.cpp Sound.cpp Fonts.cpp ResourceMgr.cpp BasicWidget.cpp UniversalObserver.cpp main.cpp InputHandler.cpp InputActions.cpp Commands.cpp GameApplication.cpp Timer.cpp ColorCurve.cpp PolygonTools.cpp Vector.cpp Particle.cpp ParticleSource.cpp PollingObject.cpp CollisionObjects.cpp PhysicalObject.cpp BackgroundImage.cpp GameSpace.cpp LifeObject.cpp QuadtreeNode.cpp Quadtree.cpp LogManager.cpp Animation.cpp LightSphere.cpp LightFlare.cpp PollingEngine.cpp Sequencer.cpp extra_math.cpp

#Libraries to include when linking:
LIBS =  -L/usr/local/lib -lSDL -lSDL_image -lSDL_mixer -lfreetype -lGL -lGLU /usr/local/lib/libftgl.a

# library name.
PROG = libLevGC.so
MAJOR_VR = 0
MINOR_VR = 1
BUILD_VR = 0

# list of generated object files.
TINYXML_OBJS = src/utilities/TinyXML/tinyxml.o src/utilities/TinyXML/tinystr.o src/utilities/TinyXML/tinyxmlparser.o src/utilities/TinyXML/tinyxmlerror.o
OBJS = $(TINYXML_OBJS) src/etc/Commaize.o src/fx/Delay.o src/geo/RenderObject.o src/geo/RenderMesh.o src/view/Material.o src/view/Light.o src/view/Camera.o src/core/RenderingLayer.o src/etc/Settings.o src/UI/widgets/Decal.o src/UI/widgets/ImgButton.o src/UI/widgets/TextBox.o src/UI/widgets/Slider.o src/UI/widgets/DropBox.o src/UI/widgets/Label.o src/UI/widgets/Button.o src/UI/widgets/FaderMessageBox.o src/fx/transitions/Fader.o src/core/DrawingTools.o src/core/GameState.o src/core/CoreInterface.o src/etc/Interpolator.o src/core/Texture.o src/core/MusicTrack.o src/core/Sound.o src/core/Fonts.o src/etc/Timer.o src/core/ResourceMgr.o src/UI/widgets/BasicWidget.o src/objects/UniversalObserver.o src/core/InputHandler.o src/core/InputActions.o src/core/Commands.o src/core/GameApplication.o src/etc/ColorCurve.o src/algo/collision/PolygonTools.o src/algo/collision/Vector.o src/objects/PollingEngine.o src/core/Sequencer.o  src/core/main.o src/fx/Particle.o src/fx/ParticleSource.o src/objects/PollingObject.o src/objects/CollisionObjects.o src/objects/PhysicalObject.o src/fx/backgrounds/BackgroundImage.o src/core/GameSpace.o src/objects/LifeObject.o src/algo/QuadtreeNode.o src/algo/Quadtree.o src/utilities/LogManager.o src/fx/animation/Animation.o src/fx/light/LightSphere.o src/fx/light/LightFlare.o src/core/extra_math.o





########## RULES ###########

# Top-level rule: compile everything
all: $(PROG)

# The program link rule:
$(PROG): $(OBJS)
	$(CC) $(LDFLAGS) -fPIC -shared $(OBJS) $(LIBS) -o $(PROG).$(MAJOR_VR).$(MINOR_VR).$(BUILD_VR)
	$(RM) $(PROG).$(MAJOR_VR).$(MINOR_VR)
	$(RM) $(PROG).$(MAJOR_VR)
	$(RM) $(PROG)
	ln -s $(PROG).$(MAJOR_VR).$(MINOR_VR).$(BUILD_VR) $(PROG).$(MAJOR_VR).$(MINOR_VR)
	ln -s $(PROG).$(MAJOR_VR).$(MINOR_VR) $(PROG).$(MAJOR_VR)
	ln -s $(PROG).$(MAJOR_VR) $(PROG)

# Meta rule for compiling ".cpp" files
%.o: %.cpp
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Rule for cleaning up before a recompile:
clean:
	$(RM) $(OBJS) $(PROG).$(MAJOR_VR).$(MINOR_VR).$(BUILD_VR) $(PROG).$(MAJOR_VR).$(MINOR_VR) $(PROG).$(MAJOR_VR) $(PROG)

# rule for making links
links:

# Rule for creating dependency lists and writing them into a dependency file:
.depend:
	$(RM) .depend
	$(MDEPEND) $(SRCS) > .depend

install:
# 	$(MKDIR) $(INSTALL_DIR)
# 	$(CP) $(PROG) $(INSTALL_DIR)
# 	ln -s $(INSTALL_DIR)/$(PROG) $(INSTALL_DIR)/$(PROG).$(MAJOR_VR)
# 	ln -s $(INSTALL_DIR)/$(PROG).$(MAJOR_VR) $(INSTALL_DIR)/$(PROG).$(MAJOR_VR).$(MINOR_VR)
# 	ln -s $(INSTALL_DIR)/$(PROG).$(MAJOR_VR).$(MINOR_VR) $(INSTALL_DIR)/$(PROG).$(MAJOR_VR).$(MINOR_VR).$(BUILD_VR)
	$(CP) $(PROG) $(INSTALL_PATH)
	ln -s $(INSTALL_PATH)/$(PROG) $(INSTALL_PATH)/$(PROG).$(MAJOR_VR)
	ln -s $(INSTALL_PATH)/$(PROG).$(MAJOR_VR) $(INSTALL_PATH)/$(PROG).$(MAJOR_VR).$(MINOR_VR)
	ln -s $(INSTALL_PATH)/$(PROG).$(MAJOR_VR).$(MINOR_VR) $(INSTALL_PATH)/$(PROG).$(MAJOR_VR).$(MINOR_VR).$(BUILD_VR)
	$(LDCONFIG) $(INSTALL_PATH)

uninstall:
#	$(RM) -R $(INSTALL_DIR)
	$(RM) $(INSTALL_PATH)/$(PROG)*
# 	$(RM) $(INSTALL_PATH)/$(PROG).$(MAJOR_VR)
# 	$(RM) $(INSTALL_PATH)/$(PROG).$(MAJOR_VR).$(MINOR_VR)
# 	$(RM) $(INSTALL_PATH)/$(PROG).$(MAJOR_VR).$(MINOR_VR).$(BUILD_VR)
