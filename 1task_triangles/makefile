all:
	@g++ -o triangles main.cpp -lgtest -pthread
N2:
	@g++ -o triangles main.cpp -DN2 -lgtest -pthread
gtests:
	@g++ -o triangles main.cpp -DGTESTS -lgtest -pthread
colllision_amount:
	@g++ -o triangles main.cpp -DCOLLISION_AMOUNT -lgtest -pthread
gentests:
	@g++ -o tests testGenerator.cpp
	@./tests
