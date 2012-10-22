BASEDIR = ..

#OPENNI_DIR = /Users/otavio/libs/OpenNI-Bin-MacOSX-v1.3.2.3
#NITE_DIR = /Users/otavio/libs/NITE-Bin-MacOSX-v1.4.1.2
#OPENCV_DIR = /Users/otavio/libs/OpenCV
OPENNI_DIR = /usr
NITE_DIR = /usr
OPENCV_DIR = /usr/local

LIBDIR = -L$(OPENNI_DIR)/lib \
         -L$(NITE_DIR)/lib \
         -L$(OPENCV_DIR) \
         -L/opt/local/lib \

INCDIR = -I. \
         -I$(OPENNI_DIR)/include \
         -I$(NITE_DIR)/include/nite \
	 -I$(NITE_DIR)/include/ni \
	 -I$(OPENCV_DIR)/include \
         -I/opt/local/include \
         -I/usr/include/eigen3 \

BINDIR = .

CXX = g++
CPPFLAGS = -O3 -msse2 -msse3 -mfpmath=sse -Wl,-no-as-needed
#CPPFLAGS += -g

UNAME := $(shell uname)

SRC = main.cpp \
      KinectInterface.cpp \
      kmeans_segmentation.cpp \
      histogram.cpp \
      gmm_segmentation.cpp \
      threshold.cpp \
      kmeans_color.cpp \
      gmm_color.cpp \
      mincut_segmentation.cpp \
      graph.cpp \
      maxflow.cpp \
      PlanePointCloudIntersect.cpp \
      AngularSkeleton.cpp \
      FitEllipse.cpp \
      CrossSections.cpp

OBJ = $(SRC:.cpp=.o)
LIBS = -lm \
       -lopencv_core \
       -lopencv_highgui \
       -lopencv_imgproc \
       -lopencv_objdetect \
       -lOpenNI \
       -lXnVNite 

ifeq ($(UNAME), Darwin)
    LIBS += -framework OpenGL \
            -framework GLUT \
            /usr/lib/libiconv.dylib
else
    LIBS += -lGL \
            -lGLU \
            -lglut \
            -lGLEW
endif

TARGET = $(BINDIR)/BodyMeasurements

VPATH = $(SGDIR)/lib

all: $(TARGET)

.cpp.o:
	$(CXX) $(CPPFLAGS) -c $(INCDIR) $<

$(TARGET): $(OBJ)
	$(CXX) $(CPPFLAGS) $(LIBDIR) $(LIBS) $^ -o $@

clean:
	rm -f $(OBJ) *.o $(TARGET) *.d *.d.*

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $(INCDIR) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

-include $(patsubst %.cpp,%.d, $(wildcard *.cpp))
