#ifndef __MENUCOMMANDS_H__
#define __MENUCOMMANDS_H__

#include "menu.h"
#define MENUCOMMAND(x) bool MenuCommand_ ## x (cMenuItem* src)


#define ADDMENUITEM(m, p, t, pr) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawStyle = 1; ((cMenuItem*)m)->proc = pr; p->items.push_back(m);
#define ADDMENUITEMDATA(m, p, t, pr,d) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawStyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->data = d; p->items.push_back(m);
#define ADDMENUITEMDATA2(m, p, t, pr,d,d2) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawStyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->data = d; ((cMenuItem*)m)->data2 = d2; p->items.push_back(m);
#define ADDMENUITEMDATAP(m, p, t, pr,d) m = new cMenuItem(); m->parent = p; m->title = t; m->item = true; m->drawStyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->pdata = d; p->items.push_back(m);
#define ADDMENUITEMDATALINK(m, p, t, pr,d) m = new cMenuItemLinked(); m->parent = p; m->title = t; m->item = true; m->drawStyle = 1; ((cMenuItem*)m)->proc = pr; ((cMenuItem*)m)->pdata = d; p->items.push_back(m);
#define ADDMENU(m,p,t,xpos,width) m = new cMenu(); m->parent = p; m->title = t; m->item = false; m->drawStyle = 1; m->y = 20; m->x = xpos; m->w = width; p->items.push_back(m);
#define ADDMENU2(m,p,t,xpos) m = new cMenu(); m->parent = p; m->title = t; m->item = false; m->drawStyle = 1; m->y = 20; m->x = xpos; m->w = Graphics.font->textlen(t)+10; p->items.push_back(m); xpos += Graphics.font->textlen(t)+10;


MENUCOMMAND(new);
MENUCOMMAND(open);
MENUCOMMAND(close);
MENUCOMMAND(save);
MENUCOMMAND(quicksave);
MENUCOMMAND(opengrf);
MENUCOMMAND(saveAs);
MENUCOMMAND(exit);
MENUCOMMAND(random1);
MENUCOMMAND(random2);
MENUCOMMAND(random3);
MENUCOMMAND(random4);
MENUCOMMAND(random5);
MENUCOMMAND(shading);
MENUCOMMAND(exportheight);
MENUCOMMAND(mode);
MENUCOMMAND(mode_detail);
MENUCOMMAND(speed);
MENUCOMMAND(fillarea);
MENUCOMMAND(model);
MENUCOMMAND(picktexture);
MENUCOMMAND(quadtree);
MENUCOMMAND(gatheight);
MENUCOMMAND(gatcollision2);
MENUCOMMAND(dolightmaps);
MENUCOMMAND(dolightmapsall);
MENUCOMMAND(dolightmapslights);
MENUCOMMAND(dolightmapsnoshadow);
MENUCOMMAND(dolightmaps2);
MENUCOMMAND(smoothlightmaps);
MENUCOMMAND(fixcolors);
MENUCOMMAND(savelightmaps);
MENUCOMMAND(loadlightmaps);
MENUCOMMAND(addwalls);
MENUCOMMAND(gatcollision);
MENUCOMMAND(cleanuplightmaps);
MENUCOMMAND(tempfunc);
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
MENUCOMMAND(npcscreenies);
MENUCOMMAND(favlights);
MENUCOMMAND(plugin);
MENUCOMMAND(mapdatabase);
MENUCOMMAND(saveOnline);
MENUCOMMAND(makeMinimaps);
MENUCOMMAND(switchMap);

MENUCOMMAND(rebuildtexturefile);
MENUCOMMAND(rebuildmodelfile);
MENUCOMMAND(rebuildsoundsfile);



// popup menu

MENUCOMMAND(addfavorite);
MENUCOMMAND(deselectlight);
MENUCOMMAND(light_disableshadow);
MENUCOMMAND(light_snaptofloor);
MENUCOMMAND(light_setheight);


// fav lights popup menu
MENUCOMMAND(removefavlight);
MENUCOMMAND(addfavlight);
MENUCOMMAND(addfavlightcat);

#endif
