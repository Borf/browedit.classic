#include "keybindwindow.h"
#include <fstream>
#include <common.h>

std::string keytostring(SDLKey key)
{
	switch(key)
	{
		/* The keyboard syms have been cleverly chosen to map to ASCII */
	case SDLK_UNKNOWN:		return "Unknown";
	case SDLK_BACKSPACE:	return "Backspace";
	case SDLK_TAB:			return "Tab";
	case SDLK_CLEAR:		return "Clear";
	case SDLK_RETURN:		return "Return";
	case SDLK_PAUSE:		return "Pause";
	case SDLK_ESCAPE:		return "Escape";
	case SDLK_SPACE:		return "Space";
	case SDLK_EXCLAIM:		return "Exclaim";
	case SDLK_QUOTEDBL:		return "QuoteDouble";
	case SDLK_HASH:			return "Hash";
	case SDLK_DOLLAR:		return "Dollar";
	case SDLK_AMPERSAND:	return "Ampersand";
	case SDLK_QUOTE:		return "Quote";
	case SDLK_LEFTPAREN:	return "Leftparen";
	case SDLK_RIGHTPAREN:	return "Rightparen";
	case SDLK_ASTERISK:		return "Asterisk";
	case SDLK_PLUS:			return "Plus";
	case SDLK_COMMA:		return "Comma";
	case SDLK_MINUS:		return "Minus";
	case SDLK_PERIOD:		return "PERIOD";
	case SDLK_SLASH:		return "SLASH";
	case SDLK_0:			return "0";
	case SDLK_1:			return "1";
	case SDLK_2:			return "2";
	case SDLK_3:			return "3";
	case SDLK_4:			return "4";
	case SDLK_5:			return "5";
	case SDLK_6:			return "6";
	case SDLK_7:			return "7";
	case SDLK_8:			return "8";
	case SDLK_9:			return "9";
	case SDLK_COLON:		return "Colon";
	case SDLK_SEMICOLON:	return "Semicolon";
	case SDLK_LESS:			return "Less";
	case SDLK_EQUALS:		return "Equals";
	case SDLK_GREATER:		return "Greater";
	case SDLK_QUESTION:		return "Question";
	case SDLK_AT:			return "At";
	/* 
	   Skip uppercase letters
	 */
	case SDLK_LEFTBRACKET:	return "Leftbracket";
	case SDLK_BACKSLASH:	return "Backslash";
	case SDLK_RIGHTBRACKET:	return "Rightbracket";
	case SDLK_CARET:		return "Caret";
	case SDLK_UNDERSCORE:	return "Underscore";
	case SDLK_BACKQUOTE:	return "Backquote";
	case SDLK_a:			return "a";
	case SDLK_b:			return "b";
	case SDLK_c:			return "c";
	case SDLK_d:			return "d";
	case SDLK_e:			return "e";
	case SDLK_f:			return "f";
	case SDLK_g:			return "g";
	case SDLK_h:			return "h";
	case SDLK_i:			return "i";
	case SDLK_j:			return "j";
	case SDLK_k:			return "k";
	case SDLK_l:			return "l";
	case SDLK_m:			return "m";
	case SDLK_n:			return "n";
	case SDLK_o:			return "o";
	case SDLK_p:			return "p";
	case SDLK_q:			return "q";
	case SDLK_r:			return "r";
	case SDLK_s:			return "s";
	case SDLK_t:			return "t";
	case SDLK_u:			return "u";
	case SDLK_v:			return "v";
	case SDLK_w:			return "w";
	case SDLK_x:			return "x";
	case SDLK_y:			return "y";
	case SDLK_z:			return "z";
	case SDLK_DELETE:		return "Delete";
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	case SDLK_WORLD_0:		return "World 0";
	case SDLK_WORLD_1:		return "World 1";
	case SDLK_WORLD_2:		return "World 2";
	case SDLK_WORLD_3:		return "World 3";
	case SDLK_WORLD_4:		return "World 4";
	case SDLK_WORLD_5:		return "World 5";
	case SDLK_WORLD_6:		return "World 6";
	case SDLK_WORLD_7:		return "World 7";
	case SDLK_WORLD_8:		return "World 8";
	case SDLK_WORLD_9:		return "World 9";
	case SDLK_WORLD_10:		return "World 10";
	case SDLK_WORLD_11:		return "World 11";
	case SDLK_WORLD_12:		return "World 12";
	case SDLK_WORLD_13:		return "World 13";
	case SDLK_WORLD_14:		return "World 14";
	case SDLK_WORLD_15:		return "World 15";
	case SDLK_WORLD_16:		return "World 16";
	case SDLK_WORLD_17:		return "World 17";
	case SDLK_WORLD_18:		return "World 18";
	case SDLK_WORLD_19:		return "World 19";
	case SDLK_WORLD_20:		return "World 20";
	case SDLK_WORLD_21:		return "World 21";
	case SDLK_WORLD_22:		return "World 22";
	case SDLK_WORLD_23:		return "World 23";
	case SDLK_WORLD_24:		return "World 24";
	case SDLK_WORLD_25:		return "World 25";
	case SDLK_WORLD_26:		return "World 26";
	case SDLK_WORLD_27:		return "World 27";
	case SDLK_WORLD_28:		return "World 28";
	case SDLK_WORLD_29:		return "World 29";
	case SDLK_WORLD_30:		return "World 30";
	case SDLK_WORLD_31:		return "World 31";
	case SDLK_WORLD_32:		return "World 32";
	case SDLK_WORLD_33:		return "World 33";
	case SDLK_WORLD_34:		return "World 34";
	case SDLK_WORLD_35:		return "World 35";
	case SDLK_WORLD_36:		return "World 36";
	case SDLK_WORLD_37:		return "World 37";
	case SDLK_WORLD_38:		return "World 38";
	case SDLK_WORLD_39:		return "World 39";
	case SDLK_WORLD_40:		return "World 40";
	case SDLK_WORLD_41:		return "World 41";
	case SDLK_WORLD_42:		return "World 42";
	case SDLK_WORLD_43:		return "World 43";
	case SDLK_WORLD_44:		return "World 44";
	case SDLK_WORLD_45:		return "World 45";
	case SDLK_WORLD_46:		return "World 46";
	case SDLK_WORLD_47:		return "World 47";
	case SDLK_WORLD_48:		return "World 48";
	case SDLK_WORLD_49:		return "World 49";
	case SDLK_WORLD_50:		return "World 50";
	case SDLK_WORLD_51:		return "World 51";
	case SDLK_WORLD_52:		return "World 52";
	case SDLK_WORLD_53:		return "World 53";
	case SDLK_WORLD_54:		return "World 54";
	case SDLK_WORLD_55:		return "World 55";
	case SDLK_WORLD_56:		return "World 56";
	case SDLK_WORLD_57:		return "World 57";
	case SDLK_WORLD_58:		return "World 58";
	case SDLK_WORLD_59:		return "World 59";
	case SDLK_WORLD_60:		return "World 60";
	case SDLK_WORLD_61:		return "World 61";
	case SDLK_WORLD_62:		return "World 62";
	case SDLK_WORLD_63:		return "World 63";
	case SDLK_WORLD_64:		return "World 64";
	case SDLK_WORLD_65:		return "World 65";
	case SDLK_WORLD_66:		return "World 66";
	case SDLK_WORLD_67:		return "World 67";
	case SDLK_WORLD_68:		return "World 68";
	case SDLK_WORLD_69:		return "World 69";
	case SDLK_WORLD_70:		return "World 70";
	case SDLK_WORLD_71:		return "World 71";
	case SDLK_WORLD_72:		return "World 72";
	case SDLK_WORLD_73:		return "World 73";
	case SDLK_WORLD_74:		return "World 74";
	case SDLK_WORLD_75:		return "World 75";
	case SDLK_WORLD_76:		return "World 76";
	case SDLK_WORLD_77:		return "World 77";
	case SDLK_WORLD_78:		return "World 78";
	case SDLK_WORLD_79:		return "World 79";
	case SDLK_WORLD_80:		return "World 80";
	case SDLK_WORLD_81:		return "World 81";
	case SDLK_WORLD_82:		return "World 82";
	case SDLK_WORLD_83:		return "World 83";
	case SDLK_WORLD_84:		return "World 84";
	case SDLK_WORLD_85:		return "World 85";
	case SDLK_WORLD_86:		return "World 86";
	case SDLK_WORLD_87:		return "World 87";
	case SDLK_WORLD_88:		return "World 88";
	case SDLK_WORLD_89:		return "World 89";
	case SDLK_WORLD_90:		return "World 90";
	case SDLK_WORLD_91:		return "World 91";
	case SDLK_WORLD_92:		return "World 92";
	case SDLK_WORLD_93:		return "World 93";
	case SDLK_WORLD_94:		return "World 94";
	case SDLK_WORLD_95:		return "World 95";

	/* Numeric keypad */
	case SDLK_KP0:			return "KeyPad 0";
	case SDLK_KP1:			return "KeyPad 1";
	case SDLK_KP2:			return "KeyPad 2";
	case SDLK_KP3:			return "KeyPad 3";
	case SDLK_KP4:			return "KeyPad 4";
	case SDLK_KP5:			return "KeyPad 5";
	case SDLK_KP6:			return "KeyPad 6";
	case SDLK_KP7:			return "KeyPad 7";
	case SDLK_KP8:			return "KeyPad 8";
	case SDLK_KP9:			return "KeyPad 9";
	case SDLK_KP_PERIOD:	return "KeyPad Period";
	case SDLK_KP_DIVIDE:	return "KeyPad Divide";
	case SDLK_KP_MULTIPLY:	return "KeyPad Multiply";
	case SDLK_KP_MINUS:		return "KeyPad Minus";
	case SDLK_KP_PLUS:		return "KeyPad Plus";
	case SDLK_KP_ENTER:		return "KeyPad Enter";
	case SDLK_KP_EQUALS:	return "KeyPad Equals";

	/* Arrows + Home/End pad */
	case SDLK_UP:			return "Up";
	case SDLK_DOWN:			return "Down";
	case SDLK_RIGHT:		return "Right";
	case SDLK_LEFT:			return "Left";
	case SDLK_INSERT:		return "Insert";
	case SDLK_HOME:			return "Home";
	case SDLK_END:			return "End";
	case SDLK_PAGEUP:		return "PageUp";
	case SDLK_PAGEDOWN:		return "PageDown";

	/* Function keys */
	case SDLK_F1:			return "F1";
	case SDLK_F2:			return "F2";
	case SDLK_F3:			return "F3";
	case SDLK_F4:			return "F4";
	case SDLK_F5:			return "F5";
	case SDLK_F6:			return "F6";
	case SDLK_F7:			return "F7";
	case SDLK_F8:			return "F8";
	case SDLK_F9:			return "F9";
	case SDLK_F10:			return "F10";
	case SDLK_F11:			return "F11";
	case SDLK_F12:			return "F12";
	case SDLK_F13:			return "F13";
	case SDLK_F14:			return "F14";
	case SDLK_F15:			return "F15";

	/* Key state modifier keys */
	case SDLK_NUMLOCK:		return "Numlock";
	case SDLK_CAPSLOCK:		return "CapsLock";
	case SDLK_SCROLLOCK:	return "ScrolLock";
	case SDLK_RSHIFT:		return "Right Shift";
	case SDLK_LSHIFT:		return "Left Shift";
	case SDLK_RCTRL:		return "Right Control";
	case SDLK_LCTRL:		return "Left Control";
	case SDLK_RALT:			return "Right Alt";
	case SDLK_LALT:			return "Left Alt";
	case SDLK_RMETA:		return "Right Meta";
	case SDLK_LMETA:		return "Left Meta";
	case SDLK_LSUPER:		return "Left Super";
	case SDLK_RSUPER:		return "Right Super";
	case SDLK_MODE:			return "Mode";
	case SDLK_COMPOSE:		return "Compose";

	/* Miscellaneous function keys */
	case SDLK_HELP:			return "Help";
	case SDLK_PRINT:		return "Print";
	case SDLK_SYSREQ:		return "SysReq";
	case SDLK_BREAK:		return "Break";
	case SDLK_MENU:			return "Menu";
	case SDLK_POWER:		return "Power";
	case SDLK_EURO:			return "Euro";
	case SDLK_UNDO:			return "Undo";

	/* Add any other keys here */

	case SDLK_LAST:			return "Last";
	default:
		Log(1,0,"Error, unknown keycode: %i", key);
	}
	return "";
}


cKeyBindWindow::cWindowOkButton::cWindowOkButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent)
{
	text = "Ok";
	alignment = ALIGN_BOTTOMRIGHT;
	moveTo(0,0);
	resizeTo(100,20);
}

void cKeyBindWindow::cWindowOkButton::click()
{
	keymap[SDLK_UP] = parent->objects["Up"]->getInt(0);
	keymap[SDLK_DOWN] = parent->objects["Down"]->getInt(0);
	keymap[SDLK_LEFT] = parent->objects["Left"]->getInt(0);
	keymap[SDLK_RIGHT] = parent->objects["Right"]->getInt(0);
	keymap[SDLK_MINUS] = parent->objects["Minus"]->getInt(0);
	keymap[SDLK_EQUALS] = parent->objects["Equals"]->getInt(0);
	keymap[SDLK_LEFTBRACKET] = parent->objects["lbracket"]->getInt(0);
	keymap[SDLK_RIGHTBRACKET] = parent->objects["rbracket"]->getInt(0);
	keymap[SDLK_SPACE] = parent->objects["Space"]->getInt(0);
	keymap[SDLK_h] = parent->objects["H"]->getInt(0);
	keymap[SDLK_v] = parent->objects["V"]->getInt(0);
	keymap[SDLK_g] = parent->objects["G"]->getInt(0);
	keymap[SDLK_l] = parent->objects["L"]->getInt(0);
	keymap[SDLK_d] = parent->objects["D"]->getInt(0);
	keymap[SDLK_COMMA] = parent->objects["Comma"]->getInt(0);
	keymap[SDLK_PERIOD] = parent->objects["Period"]->getInt(0);
	keymap[SDLK_PAGEUP] = parent->objects["Pageup"]->getInt(0);
	keymap[SDLK_PAGEDOWN] = parent->objects["Pagedown"]->getInt(0);
	keymap[SDLK_HOME] = parent->objects["Home"]->getInt(0);
	keymap[SDLK_END] = parent->objects["End"]->getInt(0);
	keymap[SDLK_w] = parent->objects["W"]->getInt(0);
	keymap[SDLK_c] = parent->objects["C"]->getInt(0);
	keymap[SDLK_p] = parent->objects["P"]->getInt(0);
	keymap[SDLK_o] = parent->objects["O"]->getInt(0);
	keymap[SDLK_BACKSPACE] = parent->objects["Backspace"]->getInt(0);
	keymap[SDLK_f] = parent->objects["F"]->getInt(0);
	keymap[SDLK_INSERT] = parent->objects["Ins"]->getInt(0);
	keymap[SDLK_DELETE] = parent->objects["Del"]->getInt(0);
	keymap[SDLK_r] = parent->objects["R"]->getInt(0);
	keymap[SDLK_F1] = parent->objects["F1"]->getInt(0);
	keymap[SDLK_F2] = parent->objects["F2"]->getInt(0);
	keymap[SDLK_F3] = parent->objects["F3"]->getInt(0);
	keymap[SDLK_F4] = parent->objects["F4"]->getInt(0);
	
	keymap[SDLK_F5] = parent->objects["F5"]->getInt(0);
	keymap[SDLK_F6] = parent->objects["F6"]->getInt(0);
	keymap[SDLK_F7] = parent->objects["F7"]->getInt(0);
	keymap[SDLK_F8] = parent->objects["F8"]->getInt(0);
	keymap[SDLK_F9] = parent->objects["F9"]->getInt(0);
	keymap[SDLK_F10] = parent->objects["F10"]->getInt(0);
	keymap[SDLK_F11] = parent->objects["F11"]->getInt(0);
	keymap[SDLK_u] = parent->objects["U"]->getInt(0);
	keymap[SDLK_t] = parent->objects["T"]->getInt(0);
	keymap[SDLK_m] = parent->objects["M"]->getInt(0);
	keymap[SDLK_RETURN] = parent->objects["Return"]->getInt(0);
	keymap[SDLK_s] = parent->objects["S"]->getInt(0);
	keymap[SDLK_1] = SDLK_1;
	keymap[SDLK_2] = SDLK_2;
	keymap[SDLK_3] = SDLK_3;
	keymap[SDLK_4] = SDLK_4;
	keymap[SDLK_5] = SDLK_5;
	keymap[SDLK_6] = SDLK_6;
	keymap[SDLK_7] = SDLK_7;
	keymap[SDLK_8] = SDLK_8;
	keymap[SDLK_9] = SDLK_9;
	keymap[SDLK_0] = SDLK_0;
	keymap[SDLK_ESCAPE] = parent->objects["Esc"]->getInt(0);;
	
	
	std::ofstream pFile("data/keymap.txt");
	for(int i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
	{
		char buf[100];
		sprintf(buf, "%i\n", keymap[i]);
		pFile.write(buf, strlen(buf));
	}
	pFile.close();
	parent->close();
}

cKeyBindWindow::cCancelButton::cCancelButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent)
{
	alignment = ALIGN_BOTTOMRIGHT;
	moveTo(110,0);
	resizeTo(100,20);
	text = "Cancel";
}

void cKeyBindWindow::cCancelButton::click()
{
	parent->close();
}

cKeyBindWindow::cKeyBindBox::cKeyBindBox( cWindow* parent, TiXmlDocument* skin ) : cWindowInputBox(parent)
{
	key = SDLK_UNKNOWN;
}

bool cKeyBindWindow::cKeyBindBox::onKeyDown( int keyid )
{
	key = (SDLKey)keyid;
	return true;
}

bool cKeyBindWindow::cKeyBindBox::onKeyUp( int keyid )
{
	return true;
}

bool cKeyBindWindow::cKeyBindBox::onChar( char ch )
{
	return true;
}

void cKeyBindWindow::cKeyBindBox::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	text = keytostring(key);
	cWindowInputBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
}

void cKeyBindWindow::cKeyBindBox::setInt( int id, int val )
{
	key = (SDLKey)val;
}

int cKeyBindWindow::cKeyBindBox::getInt( int id )
{
	return key;
}

cKeyBindWindow::cClearButton::cClearButton( cWindow* parent, cKeyBindBox* box, TiXmlDocument* skin ) : cWindowButton(parent)
{
	text = "Clear";
	resizeTo(50,20);
	alignment = box->alignment;
	clearbox = box;
}

void cKeyBindWindow::cClearButton::click()
{
	clearbox->setInt(0,0);
}

void cKeyBindWindow::addclearbutton( cKeyBindBox* box, TiXmlDocument* skin )
{
	cWindowObject* o = new cClearButton(this, box, skin);
	if(box->alignment == ALIGN_TOPLEFT)
		o->moveTo(box->getX()+100, box->getY());
	else
		o->moveTo(box->getX()-50, box->getY());
	static int i = 0;
	char buf[10];
	sprintf(buf, "%i", i++);
	objects[buf] = o;
}

cKeyBindWindow::cKeyBindBox* cKeyBindWindow::addbox( std::string name, int x, int y, int defval, TiXmlDocument* skin )
{
	cKeyBindBox* o = new cKeyBindBox(this);
	o->moveTo(x,y);
	o->alignment = x == 50 ? ALIGN_TOPRIGHT : ALIGN_TOPLEFT;
	o->resizeTo(100,20);
	o->setInt(1, defval);
	objects[name] = o;
	addclearbutton(o, skin);
	return o;
}

cKeyBindWindow::cKeyBindWindow( ) : cWindow()
{
	windowType = WT_KEYBIND;
	resizable = false;
	visible = true;
	modal = false;
	
	resizeTo(700,520);
	center();
	title = GetMsg("wm/preferences/TITLE");
	
	
	/*		o = new cKeyBindBox(this);
	o->moveto(30,30);
	o->resizeTo(200,20);
	objects["bla"] = o;*/
	
	addLabel("lblUp",		0,   0, "Move up");
	addLabel("lblDown",		0,  20, "Move down");
	addLabel("lblLeft",		0,  40, "Move left");
	addLabel("lblRight",	0,  60, "Move right");		
	addLabel("lblMinus",	0,  80, "Decrease Brush Size");
	addLabel("lblEquals",	0, 100, "Increase brush Size");
	addLabel("lbllbracket", 0, 120, "Move right menu up");
	addLabel("lblrbracket", 0, 140, "Move right menu down");
	addLabel("lblSpace",	0, 160, "Rotate brush, map gat local");
	addLabel("lblH",		0, 180, "Horizontal Flip");
	addLabel("lblV",		0, 200, "Vertical Flip");
	addLabel("lblG",		0, 220, "Grid");
	addLabel("lblL",		0, 240, "Toggle lightmaps");
	addLabel("lblD",		0, 260, "Duplicate");
	addLabel("lblComma",	0, 280, "Add horizontal wall");
	addLabel("lblPeriod",	0, 300, "Add vertical wall");
	addLabel("lblPageup",	0, 320, "Raise things");
	addLabel("lblPagedown",	0, 340, "Lower things");
	addLabel("lblHome",		0, 360, "Raise wall things");
	addLabel("lblEnd",		0, 380, "Lower wall things");
	addLabel("lblW",		0, 400, "Wall texture align");
	addLabel("lblC",		0, 420, "Copy");
	addLabel("lblP",		0, 440,"Paste");
	
	addLabel("lblO",		160,  0,"Show Objects")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblBackspace",160, 20,"Remove")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF",		160, 40,"Flatten")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblIns",		160, 60,"Mode Quadtree detail")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblDel",		160, 80,"Less Quadtree detail")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblR",		160,100,"Reset Rotation")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF1",		160,120,"Texture Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF2",		160,140,"Global Height Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF3",		160,160,"Local Height Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF4",		160,180,"Wall Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF5",		160,200,"Object Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF6",		160,220,"GAT Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF7",		160,240,"Water Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF8",		160,260,"Effect Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF9",		160,280,"Sound Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF10",		160,300,"Light Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblF11",		160,320,"Object Group Edit Mode")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblU",		160,340,"Undo")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblT",		160,360,"Open Texture Window")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblM",		160,380,"Open Model select Window")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblReturn",	160,400,"Open object/light/effect properties")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblS",		160,420,"Smooth")->alignment = ALIGN_TOPRIGHT;
	addLabel("lblEsc",		160,440,"Exit")->alignment = ALIGN_TOPRIGHT;
	
	
	addbox("Up",			160,  0,keymap[SDLK_UP]);
	addbox("Down",			160, 20,keymap[SDLK_DOWN]);
	addbox("Left",			160, 40,keymap[SDLK_LEFT]);
	addbox("Right",			160, 60,keymap[SDLK_RIGHT]);
	addbox("Minus",			160, 80,keymap[SDLK_MINUS]);
	addbox("Equals",		160,100,keymap[SDLK_EQUALS]);
	addbox("lbracket",		160,120,keymap[SDLK_LEFTBRACKET]);
	addbox("rbracket",		160,140,keymap[SDLK_RIGHTBRACKET]);
	addbox("Space",			160,160,keymap[SDLK_SPACE]);
	addbox("H",				160,180,keymap[SDLK_h]);
	addbox("V",				160,200,keymap[SDLK_v]);
	addbox("G",				160,220,keymap[SDLK_g]);
	addbox("L",				160,240,keymap[SDLK_l]);
	addbox("D",				160,260,keymap[SDLK_d]);
	addbox("Comma",			160,280,keymap[SDLK_COMMA]);
	addbox("Period",		160,300,keymap[SDLK_PERIOD]);
	addbox("Pageup",		160,320,keymap[SDLK_PAGEUP]);
	addbox("Pagedown",		160,340,keymap[SDLK_PAGEDOWN]);
	addbox("Home",			160,360,keymap[SDLK_HOME]);
	addbox("End",			160,380,keymap[SDLK_END]);
	addbox("W",				160,400,keymap[SDLK_w]);
	addbox("C",				160,420,keymap[SDLK_c]);
	addbox("P",				160,440,keymap[SDLK_p]);
	
	
	addbox("O",				50,  0,keymap[SDLK_o])->alignment = ALIGN_TOPRIGHT;
	addbox("Backspace",		50, 20,keymap[SDLK_BACKSPACE])->alignment = ALIGN_TOPRIGHT;
	addbox("F",				50, 40,keymap[SDLK_f])->alignment = ALIGN_TOPRIGHT;
	addbox("Ins",			50, 60,keymap[SDLK_INSERT])->alignment = ALIGN_TOPRIGHT;
	addbox("Del",			50, 80,keymap[SDLK_DELETE])->alignment = ALIGN_TOPRIGHT;
	addbox("R",				50,100,keymap[SDLK_r])->alignment = ALIGN_TOPRIGHT;
	addbox("F1",			50,120,keymap[SDLK_F1])->alignment = ALIGN_TOPRIGHT;
	addbox("F2",			50,140,keymap[SDLK_F2])->alignment = ALIGN_TOPRIGHT;
	addbox("F3",			50,160,keymap[SDLK_F3])->alignment = ALIGN_TOPRIGHT;
	addbox("F4",			50,180,keymap[SDLK_F4])->alignment = ALIGN_TOPRIGHT;
	addbox("F5",			50,200,keymap[SDLK_F5])->alignment = ALIGN_TOPRIGHT;
	addbox("F6",			50,220,keymap[SDLK_F6])->alignment = ALIGN_TOPRIGHT;
	addbox("F7",			50,240,keymap[SDLK_F7])->alignment = ALIGN_TOPRIGHT;
	addbox("F8",			50,260,keymap[SDLK_F8])->alignment = ALIGN_TOPRIGHT;
	addbox("F9",			50,280,keymap[SDLK_F9])->alignment = ALIGN_TOPRIGHT;
	addbox("F10",			50,300,keymap[SDLK_F10])->alignment = ALIGN_TOPRIGHT;
	addbox("F11",			50,320,keymap[SDLK_F11])->alignment = ALIGN_TOPRIGHT;
	addbox("U",				50,340,keymap[SDLK_u])->alignment = ALIGN_TOPRIGHT;
	addbox("T",				50,360,keymap[SDLK_t])->alignment = ALIGN_TOPRIGHT;
	addbox("M",				50,380,keymap[SDLK_m])->alignment = ALIGN_TOPRIGHT;
	addbox("Return",		50,400,keymap[SDLK_RETURN])->alignment = ALIGN_TOPRIGHT;
	addbox("S",				50,420,keymap[SDLK_s])->alignment = ALIGN_TOPRIGHT;
	addbox("Esc",			50,440,keymap[SDLK_ESCAPE])->alignment = ALIGN_TOPRIGHT;
	
	
	
	objects["closebutton"] = new cWindowCloseButton(this);
	objects["rollupbutton"] = new cWindowRollupButton(this);
	
	objects["okbutton"] = new cWindowOkButton(this);
	objects["cancelbutton"] = new cCancelButton(this);
}