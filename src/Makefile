#!make
LIBS = -lGL -lz -lSDL -lGLU -lgd

%.o: %.cpp
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CC) $(CFLAGS) -D__USESDL__ -c -o $@ $<


../browedit:  RSMModel.o common.o filesystem.o font.o frustum.o graphics.o main.o texture.o texturecache.o texturemodel.o world.o 

	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CC) $(LDFLAGS) -o $@  RSMModel.o common.o filesystem.o font.o frustum.o graphics.o main.o texture.o texturecache.o texturemodel.o world.o $(LIBS)
RSMModel.o: RSMModel.cpp RSMModel.h common.h mymath.h texture.h filesystem.h graphics.h world.h texturecache.h texturemodel.h font.h frustum.h
common.o: common.cpp common.h mymath.h
filesystem.o: filesystem.cpp common.h mymath.h filesystem.h
font.o: font.cpp font.h common.h mymath.h texture.h filesystem.h
frustum.o: frustum.cpp frustum.h common.h mymath.h
graphics.o: graphics.cpp main.h common.h mymath.h graphics.h world.h texture.h filesystem.h RSMModel.h texturecache.h texturemodel.h font.h frustum.h menu.h
main.o: main.cpp common.h mymath.h filesystem.h main.h graphics.h world.h texture.h RSMModel.h texturecache.h texturemodel.h font.h frustum.h menu.h md5.h
texture.o: texture.cpp texture.h common.h mymath.h filesystem.h
texturecache.o: texturecache.cpp texturecache.h common.h mymath.h texture.h filesystem.h
texturemodel.o: texturemodel.cpp texturemodel.h common.h mymath.h texture.h filesystem.h texturecache.h
world.o: world.cpp main.h common.h mymath.h graphics.h world.h texture.h filesystem.h RSMModel.h texturecache.h texturemodel.h font.h frustum.h



clean:
	rm -f *.o
