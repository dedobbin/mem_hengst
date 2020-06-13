all: mem_hengst.cpp mem_hengst.hpp
	g++ -c -fPIC mem_hengst.cpp -o libmem_hengst.so
install: 
	cp libmem_hengst.so /usr/local/lib/
	cp mem_hengst.hpp /usr/local/include/
uninstall:
	rm /usr/local/lib/libmem_hengst.so /usr/local/include/mem_hengst.hpp
example: example.cpp
	g++ example.cpp -lmem_hengst -o example
clean:
	rm *.o libmem_hengst.so