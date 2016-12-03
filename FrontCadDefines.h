#ifndef FRONTCADDEFINES__H
#define FRONTCADDEFINES__H

#define RED(x)		((x & 0xff0000) >> 16)
#define GREEN(x)	((x & 0xff00) >> 8)
#define BLUE(x)		(x & 0x0ff)
///---------------------------------------
/// CFrontCadView
///---------------------------------------
#define DRAWMODE_NONE		0
#define DRAWMODE_LINE		1
#define DRAWMODE_RECT		2
#define DRAWMODE_ELIPSE		3
#define DRAWMODE_SELECT		4
#define DRAWMODE_RNDRECT	5
#define DRAWMODE_ARC		6
#define DRAWMODE_POLYGON	7
#define DRAWMODE_POLYFILL	8
#define DRAWMODE_ARC_CENTER	9
#define DRAWMODE_HOLE_ROUND	10
#define DRAWMODE_HOLE_RECT	11
#define DRAWMODE_HOLE_RND1F	12
#define DRAWMODE_HOLE_RND2F	13
#define DRAWMODE_TEXT		14
#define DRAWMODE_LIBPART	15
#define DRAWMODE_GETREF		16
#define DRAWMODE_MOVE		17
#define DRAWMODE_COPY		18
#define DRAWMODE_CUT		19
#define DRAWMODE_PASTE		20
#define DRAWMODE_SELECTREGION	21
#define DRAWMODE_BITMAP		22
#define DRAWMODE_ARROW		23
#define DRAWMODE_ORIGIN		24
#define DRAWMODE_DIMENSION	25
#define DRAWMODE_ALIGN_DIM_TO_HOLE	26
#define DRAWMODE_PRINTRECT	27


/*
///------------------------------
/// States for moving selections
///------------------------------

#define MOVESTATE_GETREFERENCE		0
#define MOVESTATE_DRAGSELECTION		1
*/
///---------------------------
/// Draw States
///--------------------------

#define DRAWSTATE_WAITMOUSE_DOWN	0
#define DRAWSTATE_MOVE				1
#define DRAWSTATE_PLACE				2
#define DRAWSTATE_ARCSTART			3
#define DRAWSTATE_ARCEND			4
#define DRAWSTATE_GETREFEREMCE		5
#define DRAWSTATE_DRAG				6

#define MOUSESTATE_DOWN	1
#define MOUSESTATE_UP	0

//-----------------------------------
// CCadObject
//------------------------------------

#define OBJECT_TYPE_NONE			0
#define OBJECT_TYPE_LINE			1
#define OBJECT_TYPE_RECT			2
#define OBJECT_TYPE_ELLIPSE			3
#define OBJECT_TYPE_RNDRECT			4
#define OBJECT_TYPE_POLY			5
#define OBJECT_TYPE_POLYFILL		6
#define OBJECT_TYPE_LIBCOMP			7
#define OBJECT_TYPE_HOLEROUND		8
#define OBJECT_TYPE_HOLERECT		9
#define OBJECT_TYPE_HOLERND1FLAT	10
#define OBJECT_TYPE_HOLERND2FLAT	11
#define OBJECT_TYPE_TEXT			12
#define OBJECT_TYPE_ARC				13
#define OBJECT_TYPE_ARCCENTERED		14
#define OBJECT_TYPE_DRAWING			15
#define OBJECT_TYPE_BITMAP			16
#define OBJECT_TYPE_ARROW			17
#define OBJECT_TYPE_ORIGIN			18
#define OBJECT_TYPE_DIMENSION		19
#define OBJECT_TYPE_PRINTRECT		20

#define OBJECT_MODE_FINAL		0
#define OBJECT_MODE_SELECTED	1
#define OBJECT_MODE_SKETCH		2
#define OBJECT_MODE_ERASE		3
#define OBJECT_MODE_ARCSTART	4
#define OBJECT_MODE_ARCEND		5

//------------------------------------
// Windows Messages
//------------------------------------

	///message from Utility Pane to Main Pane
#define WM_UTILMESSAGE		WM_USER+1

	///submessages for WM_UTILMESSAGE

#define UTIL_DEFAULT		0
#define UTIL_X1				1
#define UTIL_Y1				2
#define UTIL_X2				3
#define UTIL_Y2				4
#define UTIL_X3				5
#define UTIL_Y3				6
#define UTIL_LINECOLOR		7
#define UTIL_LINEWIDTH		8
#define UTIL_FILLCOLOR		9
#define UTIL_TEXT			10
#define UTIL_FLATDIST		11
#define UTIL_HOLERADIUS		12
#define UTIL_FONTHEIGHT		13
#define UTIL_FONTWIDTH		14
#define UTIL_TEXTANGLE		15
#define UTIL_TEXTTRANSPARENT		16
#define UTIL_TEXTCOLOR		17
#define UTIL_FONT			18
#define UTIL_TEXTBKCOLOR	19
#define UTIL_FONTWEIGHT		20
#define UTIL_BACKGROUND		21
#define UTIL_PRINTRECT		22

#define WM_LIBVIEW_GETREF	WM_USER+6
#define WM_MAINVIEW_GOTREF	WM_USER+7

	///message from Tab1 to 
	///Pot Scale Wizard Dlg
#define WM_TAB1_MESSAGE		WM_USER+2

#define TAB1_ARCSTART		0
#define TAB1_ARCEND			1
#define TAB1_DIVISIONS		2
#define TAB1_ARCLINEWIDTH	3
#define TAB1_ARCRADIUS		4
#define TAB1_DIVLINEWIDTH	5
#define TAB1_DIVLINELEN		6
#define TAB1_HOLERADIUS		7
#define TAB1_FLATDIST		8
#define TAB1_HOLETYPE		9
#define TAB1_SUBDIVISIONS	10
#define TAB1_SUBDIVLINELEN	11
#define TAB1_SUBDIVLINWIDTH	12
#define TAB1_DIVCOLOR		13
#define TAB1_SUBCOLOR		14
#define TAB1_ARCCOLOR		15

#define WM_TAB2_MESSAGE		WM_USER+3

#define TAB2_FONTSIZE		0
#define TAB2_FONTWEIGHT		1
#define TAB2_LABELTEXT		2
#define TAB2_DISTTOTICK		3
#define TAB2_FONT			4
#define TAB2_TEXTCOLOR		5
#define TAB2_TEXTBKCOLOR	6
#define TAB2_ROTATE_TEXT	7

#define WM_TAB2_LABELSEL	WM_USER+4

#define WM_TAB3_MESSAGE		WM_USER+5

#define WM_UPDATEDIMENSIONS		WM_USER+8

///------------------------------
/// Zoom
///-----------------------------

#define MAX_ZOOM		6

#endif
