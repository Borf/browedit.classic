#ifndef __MENUCOMMANDS_H__
#define __MENUCOMMANDS_H__

#include "menu.h"
#define MENUCOMMAND(x) bool MenuCommand_ ## x (cMenuItem* src)


MENUCOMMAND(new);
MENUCOMMAND(open);
MENUCOMMAND(save);
MENUCOMMAND(saveAs);
MENUCOMMAND(importalpha);
MENUCOMMAND(exit);
MENUCOMMAND(random1);
MENUCOMMAND(random2);
MENUCOMMAND(random3);
MENUCOMMAND(random4);
MENUCOMMAND(random5);
MENUCOMMAND(shading);
MENUCOMMAND(exportheight);
MENUCOMMAND(mode);
MENUCOMMAND(flatten);
MENUCOMMAND(grid);
MENUCOMMAND(mode_detail);
MENUCOMMAND(speed);
MENUCOMMAND(fill);
MENUCOMMAND(fillarea);
MENUCOMMAND(showobjects);
MENUCOMMAND(model);
MENUCOMMAND(slope);
MENUCOMMAND(picktexture);
MENUCOMMAND(quadtree);
MENUCOMMAND(gatheight);
MENUCOMMAND(dolightmaps);
MENUCOMMAND(dolightmapsall);
MENUCOMMAND(dolightmapslights);
MENUCOMMAND(dolightmaps2);
MENUCOMMAND(fixcolors);
MENUCOMMAND(clearobjects);
MENUCOMMAND(cleareffects);
MENUCOMMAND(clearlights);
MENUCOMMAND(savelightmaps);
MENUCOMMAND(loadlightmaps);
MENUCOMMAND(addwalls);
MENUCOMMAND(gatcollision);
MENUCOMMAND(clearlightmaps);
MENUCOMMAND(cleanuplightmaps);
MENUCOMMAND(tempfunc);
MENUCOMMAND(snaptofloor);
MENUCOMMAND(clearstuff);
MENUCOMMAND(effect);
MENUCOMMAND(toggle);
MENUCOMMAND(water);
MENUCOMMAND(ambientlight);
MENUCOMMAND(cleantextures);
MENUCOMMAND(modelwindow);
MENUCOMMAND(texturewindow);
MENUCOMMAND(properties);
MENUCOMMAND(preferences);
MENUCOMMAND(rsmedit);
MENUCOMMAND(exportmapfiles);
MENUCOMMAND(99dun);
MENUCOMMAND(eascript);

#endif
