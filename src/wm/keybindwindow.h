#ifndef __KEYBINDWINDOW_H__
#define __KEYBINDWINDOW_H__

#include "window.h"
#include <fstream>


string keytostring(SDLKey key)
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


class cKeyBindWindow :	public cWindow
{
public:
	class cWindowOkButton : public cWindowButton
	{
	public:
		cWindowOkButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			text = "Ok";
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(0,0);
			resizeto(100,20);
		}
		void click()
		{
			keymap[SDLK_UP] = parent->objects["Up"]->GetInt(0);
			keymap[SDLK_DOWN] = parent->objects["Down"]->GetInt(0);
			keymap[SDLK_LEFT] = parent->objects["Left"]->GetInt(0);
			keymap[SDLK_RIGHT] = parent->objects["Right"]->GetInt(0);
			keymap[SDLK_MINUS] = parent->objects["Minus"]->GetInt(0);
			keymap[SDLK_EQUALS] = parent->objects["Equals"]->GetInt(0);
			keymap[SDLK_LEFTBRACKET] = parent->objects["lbracket"]->GetInt(0);
			keymap[SDLK_RIGHTBRACKET] = parent->objects["rbracket"]->GetInt(0);
			keymap[SDLK_SPACE] = parent->objects["Space"]->GetInt(0);
			keymap[SDLK_h] = parent->objects["H"]->GetInt(0);
			keymap[SDLK_v] = parent->objects["V"]->GetInt(0);
			keymap[SDLK_g] = parent->objects["G"]->GetInt(0);
			keymap[SDLK_l] = parent->objects["L"]->GetInt(0);
			keymap[SDLK_d] = parent->objects["D"]->GetInt(0);
			keymap[SDLK_COMMA] = parent->objects["Comma"]->GetInt(0);
			keymap[SDLK_PERIOD] = parent->objects["Period"]->GetInt(0);
			keymap[SDLK_PAGEUP] = parent->objects["Pageup"]->GetInt(0);
			keymap[SDLK_PAGEDOWN] = parent->objects["Pagedown"]->GetInt(0);
			keymap[SDLK_HOME] = parent->objects["Home"]->GetInt(0);
			keymap[SDLK_END] = parent->objects["End"]->GetInt(0);
			keymap[SDLK_w] = parent->objects["W"]->GetInt(0);
			keymap[SDLK_c] = parent->objects["C"]->GetInt(0);
			keymap[SDLK_p] = parent->objects["P"]->GetInt(0);
			keymap[SDLK_o] = parent->objects["O"]->GetInt(0);
			keymap[SDLK_BACKSPACE] = parent->objects["Backspace"]->GetInt(0);
			keymap[SDLK_f] = parent->objects["F"]->GetInt(0);
			keymap[SDLK_INSERT] = parent->objects["Ins"]->GetInt(0);
			keymap[SDLK_DELETE] = parent->objects["Del"]->GetInt(0);
			keymap[SDLK_r] = parent->objects["R"]->GetInt(0);
			keymap[SDLK_F1] = parent->objects["F1"]->GetInt(0);
			keymap[SDLK_F2] = parent->objects["F2"]->GetInt(0);
			keymap[SDLK_F3] = parent->objects["F3"]->GetInt(0);
			keymap[SDLK_F4] = parent->objects["F4"]->GetInt(0);

			keymap[SDLK_F5] = parent->objects["F5"]->GetInt(0);
			keymap[SDLK_F6] = parent->objects["F6"]->GetInt(0);
			keymap[SDLK_F7] = parent->objects["F7"]->GetInt(0);
			keymap[SDLK_F8] = parent->objects["F8"]->GetInt(0);
			keymap[SDLK_F9] = parent->objects["F9"]->GetInt(0);
			keymap[SDLK_F10] = parent->objects["F10"]->GetInt(0);
			keymap[SDLK_F11] = parent->objects["F11"]->GetInt(0);
			keymap[SDLK_u] = parent->objects["U"]->GetInt(0);
			keymap[SDLK_t] = parent->objects["T"]->GetInt(0);
			keymap[SDLK_m] = parent->objects["M"]->GetInt(0);
			keymap[SDLK_RETURN] = parent->objects["Return"]->GetInt(0);
			keymap[SDLK_s] = parent->objects["S"]->GetInt(0);
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
			keymap[SDLK_ESCAPE] = parent->objects["Esc"]->GetInt(0);;


			ofstream pFile("keymap.txt");
			for(int i = 0; i < SDLK_LAST-SDLK_FIRST; i++)
			{
				char buf[100];
				sprintf(buf, "%i\n", keymap[i]);
				pFile.write(buf, strlen(buf));
			}
			pFile.close();
			parent->close();
		}
	};

	class cCancelButton : public cWindowButton
	{
	public:
		cCancelButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(110,0);
			resizeto(100,20);
			text = "Cancel";
		}
		void click()
		{
			parent->close();
		}
	};


	class cKeyBindBox :	public cWindowInputBox
	{
		SDLKey key;
	public:
		cKeyBindBox(cWindow* parent, TiXmlDocument &skin) : cWindowInputBox(parent,skin)
		{
			key = SDLK_UNKNOWN;
		}

		bool onkeydown(int keyid)
		{
			key = (SDLKey)keyid;
			return true;
		}

		bool onkeyup(int keyid)
		{
			return true;
		}

		bool onchar(char ch)
		{
			return true;
		}

		void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
		{
			text = keytostring(key);
			cWindowInputBox::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
		}


		void SetInt(int id, intptr_t val)
		{
			key = (SDLKey)val;
		}
		intptr_t GetInt(int id)
		{
			return key;
		}





	};




	class cClearButton : public cWindowButton
	{
		cKeyBindBox* clearbox;
	public:
		cClearButton(cWindow* parent, cKeyBindBox* box, TiXmlDocument &skin) : cWindowButton(parent,skin)
		{
			text = "Clear";
			resizeto(50,20);
			alignment = box->alignment;
			clearbox = box;
		}
		void click()
		{
			clearbox->SetInt(0,0);
		}
	};

	void addclearbutton(cKeyBindBox* box, TiXmlDocument &skin)
	{
		cWindowObject* o = new cClearButton(this, box, skin);
		if(box->alignment == ALIGN_TOPLEFT)
			o->moveto(box->px()+100, box->py());
		else
			o->moveto(box->px()-50, box->py());
		static int i = 0;
		char buf[10];
		sprintf(buf, "%i", i++);
		objects[buf] = o;
	}

	cKeyBindBox* addbox(string name, int x, int y, int defval, TiXmlDocument &skin)
	{
		cKeyBindBox* o = new cKeyBindBox(this,skin);
		o->moveto(x,y);
		o->alignment = x == 50 ? ALIGN_TOPRIGHT : ALIGN_TOPLEFT;
		o->resizeto(100,20);
		o->SetInt(1, defval);
		objects[name] = o;
		addclearbutton(o,skin);
		return o;
		
	}
	cKeyBindWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
	{
		wtype = WT_KEYBIND;
		resizable = false;
		visible = true;
		modal = false;

		resizeto(900,700);
		center();
		title = GetMsg("wm/preferences/TITLE");


/*		o = new cKeyBindBox(this);
		o->moveto(30,30);
		o->resizeto(200,20);
		objects["bla"] = o;*/

		addlabel("lblUp",		0,   0, "Move up");
		addlabel("lblDown",		0,  20, "Move down");
		addlabel("lblLeft",		0,  40, "Move left");
		addlabel("lblRight",	0,  60, "Move right");		
		addlabel("lblMinus",	0,  80, "Decrease Brush Size");
		addlabel("lblEquals",	0, 100, "Increase brush Size");
		addlabel("lbllbracket", 0, 120, "Move right menu up");
		addlabel("lblrbracket", 0, 140, "Move right menu down");
		addlabel("lblSpace",	0, 160, "Rotate brush, map gat local");
		addlabel("lblH",		0, 180, "Horizontal Flip");
		addlabel("lblV",		0, 200, "Vertical Flip");
		addlabel("lblG",		0, 220, "Grid");
		addlabel("lblL",		0, 240, "Toggle lightmaps");
		addlabel("lblD",		0, 260, "Duplicate");
		addlabel("lblComma",	0, 280, "Add horizontal wall");
		addlabel("lblPeriod",	0, 300, "Add vertical wall");
		addlabel("lblPageup",	0, 320, "Raise things");
		addlabel("lblPagedown",	0, 340, "Lower things");
		addlabel("lblHome",		0, 360, "Raise wall things");
		addlabel("lblEnd",		0, 380, "Lower wall things");
		addlabel("lblW",		0, 400, "Wall texture align");
		addlabel("lblC",		0, 420, "Copy");
		addlabel("lblP",		0, 440,"Paste");
		addlabel("lblO",		0, 460,"Show Objects");
		addlabel("lblBackspace",0, 480,"Remove");
		addlabel("lblF",		0, 500,"Flatten");
		addlabel("lblIns",		0, 520,"Mode Quadtree detail");
		addlabel("lblDel",		0, 540,"Less Quadtree detail");
		addlabel("lblR",		0, 560,"Reset Rotation");
		addlabel("lblF1",		0, 580,"Texture Edit Mode");
		addlabel("lblF2",		0, 600,"Global Height Edit Mode");
		addlabel("lblF3",		0, 620,"Local Height Edit Mode");
		addlabel("lblF4",		0, 640,"Wall Edit Mode");

		addlabel("lblF5",		200,  0,"Object Edit Mode")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblF6",		200, 20,"GAT Edit Mode")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblF7",		200, 40,"Water Edit Mode")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblF8",		200, 60,"Effect Edit Mode")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblF9",		200, 80,"Sound Edit Mode")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblF10",		200,100,"Light Edit Mode")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblF11",		200,120,"Object Group Edit Mode")->alignment = ALIGN_TOPRIGHT;

		addlabel("lblU",		200,140,"Undo")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblT",		200,160,"Open Texture Window")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblM",		200,180,"Open Model select Window")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblReturn",	200,200,"Open object/light/effect properties")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblS",		200,220,"Smooth")->alignment = ALIGN_TOPRIGHT;
		addlabel("lblEsc",		200,240,"Exit")->alignment = ALIGN_TOPRIGHT;


		addbox("Up",			250,  0,keymap[SDLK_UP],skin);
		addbox("Down",			250, 20,keymap[SDLK_DOWN],skin);
		addbox("Left",			250, 40,keymap[SDLK_LEFT],skin);
		addbox("Right",			250, 60,keymap[SDLK_RIGHT],skin);
		addbox("Minus",			250, 80,keymap[SDLK_MINUS],skin);
		addbox("Equals",		250,100,keymap[SDLK_EQUALS],skin);
		addbox("lbracket",		250,120,keymap[SDLK_LEFTBRACKET],skin);
		addbox("rbracket",		250,140,keymap[SDLK_RIGHTBRACKET],skin);
		addbox("Space",			250,160,keymap[SDLK_SPACE],skin);
		addbox("H",				250,180,keymap[SDLK_h],skin);
		addbox("V",				250,200,keymap[SDLK_v],skin);
		addbox("G",				250,220,keymap[SDLK_g],skin);
		addbox("L",				250,240,keymap[SDLK_l],skin);
		addbox("D",				250,260,keymap[SDLK_d],skin);
		addbox("Comma",			250,280,keymap[SDLK_COMMA],skin);
		addbox("Period",		250,300,keymap[SDLK_PERIOD],skin);
		addbox("Pageup",		250,320,keymap[SDLK_PAGEUP],skin);
		addbox("Pagedown",		250,340,keymap[SDLK_PAGEDOWN],skin);
		addbox("Home",			250,360,keymap[SDLK_HOME],skin);
		addbox("End",			250,380,keymap[SDLK_END],skin);
		addbox("W",				250,400,keymap[SDLK_w],skin);
		addbox("C",				250,420,keymap[SDLK_c],skin);
		addbox("P",				250,440,keymap[SDLK_p],skin);
		addbox("O",				250,460,keymap[SDLK_o],skin);
		addbox("Backspace",		250,480,keymap[SDLK_BACKSPACE],skin);
		addbox("F",				250,500,keymap[SDLK_f],skin);
		addbox("Ins",			250,520,keymap[SDLK_INSERT],skin);
		addbox("Del",			250,540,keymap[SDLK_DELETE],skin);
		addbox("R",				250,560,keymap[SDLK_r],skin);
		addbox("F1",			250,580,keymap[SDLK_F1],skin);
		addbox("F2",			250,600,keymap[SDLK_F2],skin);
		addbox("F3",			250,620,keymap[SDLK_F3],skin);
		addbox("F4",			250,640,keymap[SDLK_F4],skin);

		addbox("F5",			50,  0,keymap[SDLK_F5],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("F6",			50, 20,keymap[SDLK_F6],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("F7",			50, 40,keymap[SDLK_F7],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("F8",			50, 60,keymap[SDLK_F8],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("F9",			50, 80,keymap[SDLK_F9],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("F10",			50,100,keymap[SDLK_F10],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("F11",			50,120,keymap[SDLK_F11],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("U",				50,140,keymap[SDLK_u],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("T",				50,160,keymap[SDLK_t],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("M",				50,180,keymap[SDLK_m],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("Return",		50,200,keymap[SDLK_RETURN],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("S",				50,220,keymap[SDLK_s],skin)->alignment = ALIGN_TOPRIGHT;
		addbox("Esc",			50,240,keymap[SDLK_ESCAPE],skin)->alignment = ALIGN_TOPRIGHT;



		objects["closebutton"] = new cWindowCloseButton(this,skin);
		objects["rollupbutton"] = new cWindowRollupButton(this,skin);

		objects["okbutton"] = new cWindowOkButton(this,skin);
		objects["cancelbutton"] = new cCancelButton(this,skin);
		
	}







};








#endif

