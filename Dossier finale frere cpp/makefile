.SILENT:

# Compiler and Flags
CXX = g++
CXXFLAGS = -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CXXUIFLAGS = -DQT_QML_DEBUG -g
CXXUILIBS = /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so /usr/lib64/libGL.so
INCPATHS = -I/usr/include/qt5 \
           -I/usr/include/qt5/QtWidgets \
           -I/usr/include/qt5/QtGui \
           -I/usr/include/qt5/QtCore \
           -I/usr/lib/qt5/mkspecs/linux-g++
LIBS = -lSDL -lpthread -lQt5Widgets -lQt5Gui -lQt5Core -lGL

# Object Files
OBJS = Dimension.o Image.o ImageNG.o MyQT.o Couleur.o ImageRGB.o ImageB.o Exception.o RGBException.o XYException.o ArrayList.o Iterateur.o Traitements.o PhotoShop.o moc_mainwindowphotoshop.o mainwindowphotoshop.o main.o

# Executable
TARGET = PhotoShop

# All Target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXUILIBS) -o $(TARGET) $(OBJS) $(LIBS)

# Compilation Rules
Dimension.o: Dimension.cpp Dimension.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c Dimension.cpp

Image.o: Image.cpp Image.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c Image.cpp

ImageNG.o: ImageNG.cpp ImageNG.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c ImageNG.cpp

MyQT.o: MyQT.cpp MyQT.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c MyQT.cpp

Couleur.o: Couleur.cpp Couleur.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c Couleur.cpp

ImageRGB.o: ImageRGB.cpp ImageRGB.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c ImageRGB.cpp

ImageB.o: ImageB.cpp ImageB.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c ImageB.cpp

Exception.o: Exception.cpp Exception.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c Exception.cpp

RGBException.o: RGBException.cpp RGBException.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c RGBException.cpp

XYException.o: XYException.cpp XYException.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c XYException.cpp

ArrayList.o: ArrayList.cpp ArrayList.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c ArrayList.cpp

Iterateur.o: Iterateur.cpp Iterateur.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c Iterateur.cpp

Traitements.o: Traitements.cpp Traitements.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c Traitements.cpp

PhotoShop.o: PhotoShop.cpp PhotoShop.h
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c PhotoShop.cpp

moc_mainwindowphotoshop.o:	moc_mainwindowphotoshop.cpp mainwindowphotoshop.h
	$(CXX) $(CXXFLAGS) $(CXXUIFLAGS) $(INCPATHS) -c moc_mainwindowphotoshop.cpp

mainwindowphotoshop.o:	mainwindowphotoshop.cpp mainwindowphotoshop.h ui_mainwindowphotoshop.h 	
	$(CXX) $(CXXFLAGS) $(CXXUIFLAGS) $(INCPATHS) -c mainwindowphotoshop.cpp

main.o:	main.cpp
	$(CXX) $(CXXFLAGS) $(CXXUIFLAGS) $(INCPATHS) -c  main.cpp


# Clean
clean:
	rm -f *.o $(TARGET)
