// FileParser.h: interface for the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEPARSER_H__6A884CBF_F824_4989_ABE1_0126A175C1F8__INCLUDED_)
#define AFX_FILEPARSER_H__6A884CBF_F824_4989_ABE1_0126A175C1F8__INCLUDED_

#include <stdio.h>
#include "FrontCadDoc.h"
#include "CadDrawing.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {
	TOKEN_NUM=256,
	TOKEN_STRING,
	TOKEN_ARC,
	TOKEN_ARC_CENTER,
	TOKEN_CIRCLE,
	TOKEN_RECT,
	TOKEN_RNDRECT,
	TOKEN_POLY,
	TOKEN_POLYFILL,
	TOKEN_LINE,
	TOKEN_HOLERND,
	TOKEN_HOLERECT,
	TOKEN_HOLE_1FLAT,
	TOKEN_HOLE_2FLAT,
	TOKEN_LIBPART,
	TOKEN_TEXT,
	TOKEN_POINT,
	TOKEN_COLOR,
	TOKEN_FONT,
	TOKEN_WEIGHT,
	TOKEN_HEIGHT,
	TOKEN_WIDTH,
	TOKEN_RADIUS,
	TOKEN_FLATDIST,
	TOKEN_DRAWFILE,
	TOKEN_LIBFILE,
	TOKEN_VERTEX,
	TOKEN_RECTW,
	TOKEN_RECTH,
	TOKEN_ANGLE,
	TOKEN_TRANSPARENT,
	TOKEN_REFERENCE,
	TOKEN_BITMAP,
	TOKEN_FILE,
	TOKEN_ARROW,
	TOKEN_LENGTH,
	TOKEN_ORG,	//token for origin object
	TOKEN_DIMENSION,
	TOKEN_PRINTRECT,
	TOKEN_SIZE
};

typedef struct {
	const char *KeyWord;
	int Token;
}KEYWORD;

class CFileParser  
{
	CFrontCadDoc *m_pDoc;
	CCadLibrary *m_pLib;
	CCadLibObject *m_pLob;
	CCadLibObject *m_pDLob;
public:
	CFileParser(CFrontCadDoc *pD);
	CFileParser();
	virtual ~CFileParser();
	int LibPart(FILE *pIN, int Token,CCadLibObject *pLibobj);
	int ReadPointValue(int TokenType,int Lookahead,CPoint &p,FILE *pIN);
	int ReadDecimalValue(int typetoken,int lookahead,int &v,FILE *pIN);
	int FontWidth(FILE *pIN, int &Width,int Token);
	int RectH(FILE *pIN,int &H, int Token);
	int RectW(FILE *pIN,int &W,int Token);
	int Vertex(FILE *pIN,int &p,int Token);
	int Height(FILE *pIN,int &H,int Token);
	int Weight(FILE *pIN,int &W,int Token);
	int Font(FILE *pIN, char **F,int token);
	int LineWidth(FILE *pIN,int &Width,int token);
	int FlatDist(FILE *pIN,int &FlatDist,int token);
	int Radius(FILE *pO,int &Radius,int token);
	int Color(FILE *pIN, COLORREF &Color,int token);
	int Point(FILE *pIN, CPoint &point,int token);
	int LibFile(FILE *pIN, int Token,CCadLibrary *pLib);
	int DrawFile(FILE *pIN,int token,CCadObject *pD);
	int m_LexIndex;
	char m_LexBuff[256];
	int UnGetBuff;
	int Expect(int token, int lookahead, FILE *pIN);
	int Accept(int token, int lookahead, FILE *pIN);
	int LookUp(char *pKW);
	int Open(char *pFileName,CFrontCadDoc *pDoc);
	int UnGetChar(int c);
	int GetChar(FILE *in);
	int Parse(FILE *pIN);
	int Lex(FILE *pIN);
	CCadLibrary *GetLib(void){return m_pLib;}
	int Size(FILE * pIN, CSize & Sz, int Token);
};

#endif // !defined(AFX_FILEPARSER_H__6A884CBF_F824_4989_ABE1_0126A175C1F8__INCLUDED_)
