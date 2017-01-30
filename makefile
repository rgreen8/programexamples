CMP = g++ gfxnew_mac.o
CLASS = ship
MAIN = final
EXEC = lander
FLAGS =   -lX11 -I/opt/X11/include/ -L/opt/X11/lib/

$(EXEC): $(CLASS).o $(MAIN).o
	$(CMP) $(FLAGS) $(MAIN).o $(CLASS).o -o $(EXEC)

$(CLASS).o: $(CLASS).cpp $(CLASS).h
	$(CMP) $(FLAGS) -c $(CLASS).cpp -o $(CLASS).o

$(MAIN).o: $(MAIN).cpp $(CLASS).h
	$(CMP) $(FLAGS) -c $(MAIN).cpp -o $(MAIN).o

clean:
	rm $(CLASS).o $(MAIN).o
	rm $(EXEC)
