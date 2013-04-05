
LIBS = -lboost_system -lboost_program_options

makeimage: main.cpp Makefile
	$(CXX) $(CXXFLAGS) -o makeimage main.cpp `GraphicsMagick++-config --cppflags --cxxflags --ldflags --libs` $(LIBS)