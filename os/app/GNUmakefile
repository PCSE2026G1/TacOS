ifdef DEBUG
CMAKE_BUILD_TYPE := Debug
else
CMAKE_BUILD_TYPE := Release
endif


mod_0.o: build/mod_0.o
	cp $< $@

.PHONY: build/mod_0.o
build/mod_0.o: CMakeLists.txt
	test -d build || cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
	cmake --build build

.PHONY: clean
clean:
	rm -rf build
	rm -f *.o
