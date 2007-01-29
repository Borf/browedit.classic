#!make
LIBS = -lGL -lz -lSDL -lGLU -lgd

%.o: %.cpp
	@echo -e "    [1mCC\033[1m\t\033[22;34m$<\033[39m"
	@$(CC) $(CFLAGS) -D__USESDL__ -c -o $@ $<


../browedit:  RSMModel.o common.o filesystem.o font.o frustum.o graphics.o main.o texture.o texturecache.o texturemodel.o wm/windowinputbox.o wm/windowobject.o wm/windowbutton.o wm/window.o wm/windowlabel.o wm/windowroundbutton.o wm/windowpicturebox.o wm/windowlistbox.o wm/windowtabpanel.o wm/windowcheckbox.o wm/windowmainbutton.o wm/wm.o wm/windowscrollpanel.o wm/windowprogressbar.o world.o 

	@echo -e "    \033[1mLD\033[1m\t\033[22;35m$@\033[39m"
	@$(CC) $(LDFLAGS) -o $@  RSMModel.o common.o filesystem.o font.o frustum.o graphics.o main.o texture.o texturecache.o texturemodel.o wm/windowinputbox.o wm/windowobject.o wm/windowbutton.o wm/window.o wm/windowlabel.o wm/windowroundbutton.o wm/windowpicturebox.o wm/windowlistbox.o wm/windowtabpanel.o wm/windowcheckbox.o wm/windowmainbutton.o wm/wm.o wm/windowscrollpanel.o wm/windowprogressbar.o world.o $(LIBS)
RSMModel.o: RSMModel.cpp RSMModel.h common.h mymath.h texture.h filesystem.h graphics.h world.h texturecache.h texturemodel.h font.h frustum.h wm/wm.h wm/../common.h wm/../mymath.h wm/../font.h wm/../texture.h wm/../filesystem.h wm/confirmwindow.h wm/window.h wm/windowobject.h wm/windowroundbutton.h wm/windowbutton.h wm/windowlabel.h
common.o: common.cpp common.h mymath.h
filesystem.o: filesystem.cpp common.h mymath.h filesystem.h
font.o: font.cpp font.h common.h mymath.h texture.h filesystem.h
frustum.o: frustum.cpp frustum.h common.h mymath.h
graphics.o: graphics.cpp main.h common.h mymath.h graphics.h world.h texture.h filesystem.h RSMModel.h texturecache.h texturemodel.h font.h frustum.h wm/wm.h wm/../common.h wm/../mymath.h wm/../font.h wm/../texture.h wm/../filesystem.h wm/confirmwindow.h wm/window.h wm/windowobject.h wm/windowroundbutton.h wm/windowbutton.h wm/windowlabel.h menu.h
main.o: main.cpp common.h mymath.h filesystem.h main.h graphics.h world.h texture.h RSMModel.h texturecache.h texturemodel.h font.h frustum.h wm/wm.h wm/../common.h wm/../mymath.h wm/../font.h wm/../texture.h wm/../filesystem.h wm/confirmwindow.h wm/window.h wm/windowobject.h wm/windowroundbutton.h wm/windowbutton.h wm/windowlabel.h menu.h md5.h wm/objectwindow.h wm/windowinputbox.h wm/effectwindow.h wm/waterwindow.h
texture.o: texture.cpp texture.h common.h mymath.h filesystem.h
texturecache.o: texturecache.cpp texturecache.h common.h mymath.h texture.h filesystem.h
texturemodel.o: texturemodel.cpp texturemodel.h common.h mymath.h texture.h filesystem.h texturecache.h
wm/windowinputbox.o: wm/windowinputbox.cpp wm/windowinputbox.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h
wm/windowobject.o: wm/windowobject.cpp wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h wm/../graphics.h wm/../world.h wm/../RSMModel.h wm/../texturecache.h wm/../texturemodel.h wm/../frustum.h wm/../wm/wm.h wm/../wm/../common.h wm/../wm/../mymath.h wm/../wm/../font.h wm/../wm/../texture.h wm/../wm/../filesystem.h wm/../wm/confirmwindow.h wm/../wm/window.h wm/../wm/windowobject.h wm/../wm/windowroundbutton.h wm/../wm/windowbutton.h wm/../wm/windowlabel.h
wm/windowbutton.o: wm/windowbutton.cpp wm/windowbutton.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h
wm/window.o: wm/window.cpp wm/window.h wm/../common.h wm/../mymath.h wm/../texture.h wm/../filesystem.h wm/../font.h wm/windowobject.h wm/../graphics.h wm/../world.h wm/../RSMModel.h wm/../texturecache.h wm/../texturemodel.h wm/../frustum.h wm/../wm/wm.h wm/../wm/../common.h wm/../wm/../mymath.h wm/../wm/../font.h wm/../wm/../texture.h wm/../wm/../filesystem.h wm/../wm/confirmwindow.h wm/../wm/window.h wm/../wm/windowobject.h wm/../wm/windowroundbutton.h wm/../wm/windowbutton.h wm/../wm/windowlabel.h wm/windowinputbox.h
wm/windowlabel.o: wm/windowlabel.cpp wm/windowlabel.h wm/../common.h wm/../mymath.h wm/windowobject.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h
wm/windowroundbutton.o: wm/windowroundbutton.cpp wm/windowroundbutton.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h wm/../graphics.h wm/../world.h wm/../RSMModel.h wm/../texturecache.h wm/../texturemodel.h wm/../frustum.h wm/../wm/wm.h wm/../wm/../common.h wm/../wm/../mymath.h wm/../wm/../font.h wm/../wm/../texture.h wm/../wm/../filesystem.h wm/../wm/confirmwindow.h wm/../wm/window.h wm/../wm/windowobject.h wm/../wm/windowroundbutton.h wm/../wm/windowbutton.h wm/../wm/windowlabel.h
wm/windowpicturebox.o: wm/windowpicturebox.cpp wm/windowpicturebox.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/../texture.h wm/../filesystem.h wm/../texturecache.h wm/window.h wm/../font.h
wm/windowlistbox.o: wm/windowlistbox.cpp wm/windowlistbox.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h
wm/windowtabpanel.o: wm/windowtabpanel.cpp wm/windowtabpanel.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h wm/../graphics.h wm/../world.h wm/../RSMModel.h wm/../texturecache.h wm/../texturemodel.h wm/../frustum.h wm/../wm/wm.h wm/../wm/../common.h wm/../wm/../mymath.h wm/../wm/../font.h wm/../wm/../texture.h wm/../wm/../filesystem.h wm/../wm/confirmwindow.h wm/../wm/window.h wm/../wm/windowobject.h wm/../wm/windowroundbutton.h wm/../wm/windowbutton.h wm/../wm/windowlabel.h
wm/windowcheckbox.o: wm/windowcheckbox.cpp wm/windowcheckbox.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h
wm/windowmainbutton.o: wm/windowmainbutton.cpp wm/windowmainbutton.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/windowbutton.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h
wm/wm.o: wm/wm.cpp wm/wm.h wm/../common.h wm/../mymath.h wm/../font.h wm/../texture.h wm/../filesystem.h wm/confirmwindow.h wm/window.h wm/windowobject.h wm/windowroundbutton.h wm/windowbutton.h wm/windowlabel.h wm/../graphics.h wm/../world.h wm/../RSMModel.h wm/../texturecache.h wm/../texturemodel.h wm/../frustum.h wm/../wm/wm.h wm/../wm/../common.h wm/../wm/../mymath.h wm/../wm/../font.h wm/../wm/../texture.h wm/../wm/../filesystem.h wm/../wm/confirmwindow.h wm/../wm/window.h wm/../wm/windowobject.h wm/../wm/windowroundbutton.h wm/../wm/windowbutton.h wm/../wm/windowlabel.h wm/messagewindow.h
wm/windowscrollpanel.o: wm/windowscrollpanel.cpp wm/windowscrollpanel.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/window.h wm/../texture.h wm/../filesystem.h wm/../font.h wm/../graphics.h wm/../world.h wm/../RSMModel.h wm/../texturecache.h wm/../texturemodel.h wm/../frustum.h wm/../wm/wm.h wm/../wm/../common.h wm/../wm/../mymath.h wm/../wm/../font.h wm/../wm/../texture.h wm/../wm/../filesystem.h wm/../wm/confirmwindow.h wm/../wm/window.h wm/../wm/windowobject.h wm/../wm/windowroundbutton.h wm/../wm/windowbutton.h wm/../wm/windowlabel.h
wm/windowprogressbar.o: wm/windowprogressbar.cpp wm/windowprogressbar.h wm/windowobject.h wm/../common.h wm/../mymath.h wm/../texture.h wm/../filesystem.h wm/window.h wm/../font.h
world.o: world.cpp main.h common.h mymath.h graphics.h world.h texture.h filesystem.h RSMModel.h texturecache.h texturemodel.h font.h frustum.h wm/wm.h wm/../common.h wm/../mymath.h wm/../font.h wm/../texture.h wm/../filesystem.h wm/confirmwindow.h wm/window.h wm/windowobject.h wm/windowroundbutton.h wm/windowbutton.h wm/windowlabel.h menu.h



clean:
	rm -f *.o
