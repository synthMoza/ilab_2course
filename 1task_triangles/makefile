all:
	@g++ -o triangles main.cpp -lgtest
N2:
	@g++ -o triangles main.cpp -DN2 -lgtest
gtests:
	@g++ -o triangles main.cpp -DGTESTS -lgtest
colllision_amount:
	@g++ -o triangles main.cpp -DCOLLISION_AMOUNT -lgtest
gentests:
	@g++ -o tests testGenerator.cpp
	@./tests
