all:
	@echo "g++ -c -std=c++0x src/*.cpp -I /usr/include/sigc++-2.0/"
	@echo "g++ -o ForlynsQuest *.o -lSDL_image"
	@echo "./ForlynsQuest"
