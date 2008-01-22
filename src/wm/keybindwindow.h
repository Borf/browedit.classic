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
		cWindowOkButton(cWindow* parent) : cWindowButton(parent)
		{
			text = "Ok";
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(10,10);
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
		cCancelButton(cWindow* parent) : cWindowButton(parent)
		{
			alignment = ALIGN_BOTTOMRIGHT;
			moveto(110,10);
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
		cKeyBindBox(cWindow* parent) : cWindowInputBox(parent)
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
		cClearButton(cWindow* parent, cKeyBindBox* box) : cWindowButton(parent)
		{
			text = "Clear";
			resizeto(50,20);
			alignment = ALIGN_TOPLEFT;
			clearbox = box;
		}
		void click()
		{
			clearbox->SetInt(0,0);
		}
	};

	void addclearbutton(cKeyBindBox* box)
	{
		cWindowObject* o = new cClearButton(this, box);
		o->moveto(box->px()+100, box->py());
		static int i = 0;
		char buf[10];
		sprintf(buf, "%i", i++);
		objects[buf] = o;
	}

	cKeyBindBox* addbox(string name, int x, int y, int defval)
	{
		cKeyBindBox* o = new cKeyBindBox(this);
		o->moveto(x,y);
		o->alignment = ALIGN_TOPLEFT;
		o->resizeto(100,20);
		o->SetInt(1, defval);
		objects[name] = o;
		addclearbutton(o);
		return o;
		
	}
	cKeyBindWindow(cTexture* t, cFont* f) : cWindow(t,f)
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

		addlabel("lblUp",		20, 30, "Move up");
		addlabel("lblDown",		20, 50, "Move down");
		addlabel("lblLeft",		20, 70, "Move left");
		addlabel("lblRight",	20, 90, "Move right");		
		addlabel("lblMinus",	20, 110, "Decrease Brush Size");
		addlabel("lblEquals",	20,	130, "Increase brush Size");
		addlabel("lbllbracket", 20, 150, "Move right menu up");
		addlabel("lblrbracket", 20, 170, "Move right menu down");
		addlabel("lblSpace",	20, 190, "Rotate brush, map gat local");
		addlabel("lblH",		20, 210, "Horizontal Flip");
		addlabel("lblV",		20, 230, "Vertical Flip");
		addlabel("lblG",		20, 250, "Grid");
		addlabel("lblL",		20, 270, "Toggle lightmaps");
		addlabel("lblD",		20, 290, "Duplicate");
		addlabel("lblComma",	20, 310, "Add horizontal wall");
		addlabel("lblPeriod",	20, 330, "Add vertical wall");
		addlabel("lblPageup",	20, 350, "Raise things");
		addlabel("lblPagedown",	20, 370, "Lower things");
		addlabel("lblHome",		20, 390, "Raise wall things");
		addlabel("lblEnd",		20, 410, "Lower wall things");
		addlabel("lblW",		20, 430, "Wall texture align");
		addlabel("lblC",		20, 450, "Copy");
		addlabel("lblP",		20, 470,"Paste");
		addlabel("lblO",		20, 490,"Show Objects");
		addlabel("lblBackspace",20, 510,"Remove");
		addlabel("lblF",		20, 530,"Flatten");
		addlabel("lblIns",		20, 550,"Mode Quadtree detail");
		addlabel("lblDel",		20, 570,"Less Quadtree detail");
		addlabel("lblR",		20, 590,"Reset Rotation");
		addlabel("lblF1",		20, 610,"Texture Edit Mode");
		addlabel("lblF2",		20, 630,"Global Height Edit Mode");
		addlabel("lblF3",		20, 650,"Local Height Edit Mode");
		addlabel("lblF4",		20, 670,"Wall Edit Mode");

		addlabel("lblF5",		500, 30,"Object Edit Mode");
		addlabel("lblF6",		500, 50,"GAT Edit Mode");
		addlabel("lblF7",		500, 70,"Water Edit Mode");
		addlabel("lblF8",		500, 90,"Effect Edit Mode");
		addlabel("lblF9",		500,110,"Sound Edit Mode");
		addlabel("lblF10",		500,130,"Light Edit Mode");
		addlabel("lblF11",		500,150,"Object Group Edit Mode");

		addlabel("lblU",		500,170,"Undo");
		addlabel("lblT",		500,190,"Open Texture Window");
		addlabel("lblM",		500,210,"Open Model select Window");
		addlabel("lblReturn",	500,230,"Open object/light/effect properties");
		addlabel("lblS",		500,250,"Smooth");
		addlabel("lblEsc",		500,270,"Exit");


		addbox("Up",250,30,keymap[SDLK_UP]);
		addbox("Down",250,50,keymap[SDLK_DOWN]);
		addbox("Left",250,70,keymap[SDLK_LEFT]);
		addbox("Right",250,90,keymap[SDLK_RIGHT]);
		addbox("Minus",250,110,keymap[SDLK_MINUS]);
		addbox("Equals",250,130,keymap[SDLK_EQUALS]);
		addbox("lbracket",250,150,keymap[SDLK_LEFTBRACKET]);
		addbox("rbracket",250,170,keymap[SDLK_RIGHTBRACKET]);
		addbox("Space",250,190,keymap[SDLK_SPACE]);
		addbox("H",250,210,keymap[SDLK_h]);
		addbox("V",250,230,keymap[SDLK_v]);
		addbox("G",250,250,keymap[SDLK_g]);
		addbox("L",250,270,keymap[SDLK_l]);
		addbox("D",250,290,keymap[SDLK_d]);
		addbox("Comma",250,310,keymap[SDLK_COMMA]);
		addbox("Period",250,330,keymap[SDLK_PERIOD]);
		addbox("Pageup",250,350,keymap[SDLK_PAGEUP]);
		addbox("Pagedown",250,370,keymap[SDLK_PAGEDOWN]);
		addbox("Home",250,390,keymap[SDLK_HOME]);
		addbox("End",250,410,keymap[SDLK_END]);
		addbox("W",250,430,keymap[SDLK_w]);
		addbox("C",250,450,keymap[SDLK_c]);
		addbox("P",250,470,keymap[SDLK_p]);
		addbox("O",250,490,keymap[SDLK_o]);
		addbox("Backspace",250,510,keymap[SDLK_BACKSPACE]);
		addbox("F",250,530,keymap[SDLK_f]);
		addbox("Ins",250,550,keymap[SDLK_INSERT]);
		addbox("Del",250,570,keymap[SDLK_DELETE]);
		addbox("R",250,590,keymap[SDLK_r]);
		addbox("F1",250,610,keymap[SDLK_F1]);
		addbox("F2",250,630,keymap[SDLK_F2]);
		addbox("F3",250,650,keymap[SDLK_F3]);
		addbox("F4",250,670,keymap[SDLK_F4]);

		addbox("F5",740,30,keymap[SDLK_F5]);
		addbox("F6",740,50,keymap[SDLK_F6]);
		addbox("F7",740,70,keymap[SDLK_F7]);
		addbox("F8",740,90,keymap[SDLK_F8]);
		addbox("F9",740,110,keymap[SDLK_F9]);
		addbox("F10",740,130,keymap[SDLK_F10]);
		addbox("F11",740,150,keymap[SDLK_F11]);
		addbox("U",740,170,keymap[SDLK_u]);
		addbox("T",740,190,keymap[SDLK_t]);
		addbox("M",740,210,keymap[SDLK_m]);
		addbox("Return",740,230,keymap[SDLK_RETURN]);
		addbox("S",740,250,keymap[SDLK_s]);
		addbox("Esc",740,270,keymap[SDLK_ESCAPE]);



		objects["closebutton"] = new cWindowCloseButton(this);
		objects["rollupbutton"] = new cWindowRollupButton(this);

		objects["okbutton"] = new cWindowOkButton(this);
		objects["cancelbutton"] = new cCancelButton(this);
		
	}







};








#endif

