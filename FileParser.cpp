// FileParser.cpp: implementation of the CFileParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "frontcad.h"
#include "FrontCadView.h"
#include "FileParser.h"
#include "CadLine.h"
#include "CadRect.h"
#include "CadArc.h"
#include "CadArcCentered.h"
#include "CadElipse.h"
#include "CadHoleRnd1Flat.h"
#include "CadHolernd2Flat.h"
#include "CadHoleRound.h"
#include "CadLibObject.h"
#include "CadLine.h"
#include "CadPolygon.h"
#include "CadPolygonFill.h"
#include "CadRect.h"
#include "CadRectHole.h"
#include "CadroundRect.h"
#include "CadText.h"
#include "CadArrow.h"
#include "CadDrawing.h"
#include "CadOrigin.h"
#include "CadDimension.h"
#include "CadPrintRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static KEYWORD KeyWords[] = {
	{"ARC",TOKEN_ARC},
	{"ARCCENT",TOKEN_ARC_CENTER},
	{"CIRCLE",TOKEN_CIRCLE},
	{"RECT",TOKEN_RECT},
	{"RNDRECT",TOKEN_RNDRECT},
	{"POLY",TOKEN_POLY},
	{"POLYFILL",TOKEN_POLYFILL},
	{"LINE",TOKEN_LINE},
	{"HOLERND",TOKEN_HOLERND},
	{"HOLERECT",TOKEN_HOLERECT},
	{"HOLE1FLAT",TOKEN_HOLE_1FLAT},
	{"HOLE2FLAT",TOKEN_HOLE_2FLAT},
	{"LIBPART",TOKEN_LIBPART},
	{"TEXT",TOKEN_TEXT},
	{"POINT",TOKEN_POINT},
	{"COLOR",TOKEN_COLOR},
	{"FONT",TOKEN_FONT},
	{"WEIGHT",TOKEN_WEIGHT},
	{"HEIGHT",TOKEN_HEIGHT},
	{"WIDTH",TOKEN_WIDTH},
	{"RADIUS",TOKEN_RADIUS},
	{"FLATDIST",TOKEN_FLATDIST},
	{"DRAWFILE",TOKEN_DRAWFILE},
	{"LIBRARY",TOKEN_LIBFILE},
	{"VERTEX",TOKEN_VERTEX},
	{"RECTW",TOKEN_RECTW},
	{"RECTH",TOKEN_RECTH},
	{"ANGLE",TOKEN_ANGLE},
	{"TRANSPARENT",TOKEN_TRANSPARENT},
	{"REF",TOKEN_REFERENCE},
	{"BITMAP",TOKEN_BITMAP},
	{"FILE",TOKEN_FILE},
	{"ARROW",TOKEN_ARROW},
	{"LENGTH",TOKEN_LENGTH},
	{"ORIGIN",TOKEN_ORG},
	{"DIMENSION",TOKEN_DIMENSION},
	{"PRINTRECT",TOKEN_PRINTRECT },
	{"SIZE",TOKEN_SIZE },
	{NULL,0}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileParser::CFileParser(CFrontCadDoc *pDoc)
{
	m_pDoc = pDoc;
	m_pLib = 0;
	m_pLob = 0;
	m_pDLob = 0;
	UnGetBuff = 0;
}

CFileParser::CFileParser()
{
	m_pDoc = 0;
	m_pLib = 0;
	m_pLob = 0;
	UnGetBuff = 0;
}

CFileParser::~CFileParser()
{

}

int CFileParser::GetChar(FILE *in)
{
	int rV;

	if(UnGetBuff)
	{
		rV = UnGetBuff;
		UnGetBuff = 0;
	}
	else
	{
		rV = fgetc(in);
	}
	return rV;
}

int CFileParser::UnGetChar(int c)
{
	UnGetBuff = c;
	return 0;
}

int CFileParser::Open(char *pFileName, CFrontCadDoc *pDoc)
{
	return 0;
}

int CFileParser::LookUp(char *pKW)
{
	int rV=-1;
	int i,loop;
	for(i=0,loop=1;KeyWords[i].KeyWord && loop;++i)
	{
		if(strcmp(KeyWords[i].KeyWord,pKW) == 0)
		{
			loop = 0;
			rV = KeyWords[i].Token;
		}
	}
	return rV;
}

int CFileParser::Accept(int token, int LookAHead, FILE *pIN)
{
	/********************************************
	** Accept
	**
	** this function is used to match
	** the expected token with the current
	** token.  If they match, get the
	** next token.
	**
	** parameter:
	**	pI.....input file stream
	**	token..expected token
	**
	** Returns:Next token, or -1 on error
	********************************************/
	int rV;

	if(Expect(token,LookAHead,pIN))
		rV = Lex(pIN);
	else
		rV = -1;
	return rV;
}

int CFileParser::Expect(int token, int LookAHead, FILE *pIN)
{
	int rV=0;

	if(token == LookAHead)
		rV = 1;
	else
	{
//		char *s = new char[256];
//		sprintf(s,"Expected %d  Got %d",Tv,LookAHead);
//		Error(s);
//		delete [] s;
	}
	return rV;
}

int CFileParser::Lex(FILE *pIN)
{
	int loop,rV;
	int c;
	int token;
	loop = 1;
	while(loop)
	{
		c = GetChar(pIN);
		switch(c)
		{
			case EOF:
				loop = 0;
				rV = c;
				break;
			case ' ': case '\t':	//whitespace
				break;
			case '\n':	//end of line
				break;
			case '\"':	//string
				m_LexIndex = 0;
				while((c = GetChar(pIN)) != '\"')
				{
					m_LexBuff[m_LexIndex++] = c;
				}
				m_LexBuff[m_LexIndex] = 0;
				rV = TOKEN_STRING;
				loop = 0;
				break;
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
			case '8': case '9':	case '-': //deccimal number
				m_LexIndex = 0;
				do
				{
					m_LexBuff[m_LexIndex++] = c;
				}while(isdigit(c = GetChar(pIN)));
				UnGetChar(c);
				m_LexBuff[m_LexIndex] = 0;
				loop = 0;
				rV = TOKEN_NUM;
				break;
			case ',': case '(': case ')': case '{': case '}':
				rV = c;
				loop = 0;
				break;
			default:	//keywords
				m_LexIndex = 0;
				m_LexBuff[m_LexIndex++] = c;
				while(isalnum(c = GetChar(pIN)))
				{
					m_LexBuff[m_LexIndex++] = c;
				}
				UnGetChar(c);
				m_LexBuff[m_LexIndex] = 0;
				token = LookUp(m_LexBuff);
				if(token >= 0)
					rV = token;
				else
					rV = -1;
				loop = 0;
				break;
		}
	}
	return rV;
}

int CFileParser::Parse(FILE *pIN)
{
	int loop = 1;
	int Token;
	CCadLibrary *pLib=0;
	CCadDrawing *pDraw=0;
	while(loop)
	{
		Token = Lex(pIN);
		switch(Token)
		{
			case TOKEN_DRAWFILE:
				Token = Accept(TOKEN_DRAWFILE,Token,pIN);
				pDraw = new CCadDrawing;
				if(Token == '(')
				{
					Token = Accept('(',Token,pIN);
					Token = Color(pIN,pDraw->m_BkColor,Token);
					Token = Accept(')',Token,pIN);
				}
				else
					pDraw->SetBkColor(RGB(255,255,255));
				Token = Accept('{',Token,pIN);
				if((Token = DrawFile(pIN,Token,pDraw)) < 0)
					loop = 0;
				Token = Accept('}',Token,pIN);
				break;
			case TOKEN_LIBFILE:
				pLib = new CCadLibrary;
				Token = Accept(TOKEN_LIBFILE,Token,pIN);
				Token = Accept('(',Token,pIN);
				pLib->SetName(m_LexBuff);
				Token = Accept(TOKEN_STRING,Token,pIN);
				Token = Accept(')',Token,pIN);
				Token = Accept('{',Token,pIN);
				Token = LibFile(pIN,Token,pLib);
				Token = Accept('}',Token,pIN);
				break;
			case EOF:
				loop = 0;
				if(pLib)
					this->m_pLib = pLib;
				else if(pDraw)
				{
					if(this->m_pDoc)
					{
						m_pDoc->m_pDrawing = pDraw;
					}
				}
				break;
		}
	}
	return 0;
}

int CFileParser::LibFile(FILE *pIN, int Token,CCadLibrary *pLib)
{
	CCadLibObject *pLibObj;
	int loop=1;
	while(loop)
	{
		switch(Token)
		{
			case TOKEN_LIBPART:
				pLibObj = new CCadLibObject;
				Token = LibPart(pIN,Token,pLibObj);
				pLib->AddPart(pLibObj);
				break;
			case '}':
				loop = 0;
				break;
		}
	}
	return Token;
}

int CFileParser::DrawFile(FILE *pIN,int Token,CCadObject *pO)
{
	int loop = 1;
	int rV=0;

	while(loop)
	{
		switch(Token)
		{
			case TOKEN_PRINTRECT:
				{
					CCadPrintRect *pPR;
					pPR = new CCadPrintRect;
					Token = Accept(TOKEN_PRINTRECT, Token, pIN);
					Token = Accept('(', Token, pIN);
					while (Token != ')')
					{
						switch (Token)
						{
							case TOKEN_POINT:
								Token = Point(pIN, pPR->m_P1, Token);
								break;
							case TOKEN_COLOR:
								Token = Color(pIN, pPR->m_Attrib.m_LineColor, Token);
								break;
							case TOKEN_WIDTH:
								Token = LineWidth(pIN, pPR->m_Attrib.m_Width,Token);
								break;
							case TOKEN_SIZE:
								Token = Size(pIN, pPR->m_Size, Token);
								break;
							case ',':
								Token = Accept(',', Token, pIN);
								break;
						}
					}
					Token = Accept(')', Token, pIN);
					pO->AddObject(pPR);
				}
				break;
			case TOKEN_DIMENSION:
				{
					CCadDimension *pDim;
					pDim = new CCadDimension;
					Token = Accept(TOKEN_DIMENSION, Token, pIN);
					Token = Accept('(', Token, pIN);
					while (Token != '}')
					{
						switch (Token)
						{
							case TOKEN_POINT:
								Token = Point(pIN, pDim->m_P1, Token);
								Token = Accept(',', Token, pIN);
								Token = Point(pIN, pDim->m_P2, Token);
								break;
							case TOKEN_WIDTH:
								Token = LineWidth(pIN, pDim->GetAttrib().m_Width,Token);
								break;
							case TOKEN_COLOR:
								Token = Color(pIN, pDim->GetAttrib().m_Color, Token);
								break;
							case',':
								Token = Accept(',', Token, pIN);
								break;
							case')':
								Token = Accept(')', Token, pIN);
								break;
							case '{':
								Token = Accept('{', Token, pIN);
								Token = DrawFile(pIN, Token, pDim);
								break;
						}

					}
					Token = Accept('}', Token, pIN);
					pO->AddObject(pDim);
				}
				break;
			case TOKEN_ORG:
				{
					CCadOrigin *pOrg;
					pOrg = new CCadOrigin;
					Token = Accept(TOKEN_ORG, Token, pIN);
					Token = Accept('(', Token, pIN);
					Token = Point(pIN, pOrg->m_P1, Token);
					Token = Accept(')', Token, pIN);
					pO->AddObject(pOrg);
			}
				break;
			case TOKEN_TEXT:
				{
					char *s = new char[256];
					CCadText *pCT = new CCadText;
					Token = Accept(TOKEN_TEXT,Token,pIN);
					Token = Accept('(',Token,pIN);
					strcpy_s(s,256,m_LexBuff);
					Token = Accept(TOKEN_STRING,Token,pIN);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pCT->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pCT->m_atrb.m_Color,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pCT->m_atrb.m_BkColor,Token);
					Token = Accept(',',Token,pIN);
					Token = Font(pIN,&pCT->m_atrb.m_pFontName,Token);
					Token = Accept(',',Token,pIN);
					Token = Weight(pIN,pCT->m_atrb.m_Weight,Token);
					Token = Accept(',',Token,pIN);
					Token = Height(pIN,pCT->m_atrb.m_FontHeight,Token);
					Token = Accept(',',Token,pIN);
					Token = FontWidth(pIN,pCT->m_atrb.m_FontWidth,Token);
					Token = Accept(',',Token,pIN);
					Token = ReadDecimalValue(TOKEN_ANGLE,Token,pCT->m_atrb.m_Angle,pIN);
					Token = Accept(',',Token,pIN);
					Token = ReadDecimalValue(TOKEN_TRANSPARENT,Token,pCT->m_atrb.m_Transparent,pIN);
					Token = Accept(')',Token,pIN);
					pCT->SetText(s);
					pO->AddObject(pCT);
				}
				break;
			case TOKEN_ARC:
				{
					CCadArc *pArc = new CCadArc;
					Token = Accept(TOKEN_ARC,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pArc->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pArc->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pArc->m_atrb.m_Start,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pArc->m_atrb.m_End,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pArc->m_atrb.m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pArc->m_atrb.m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pArc);
				}
				break;
			case TOKEN_ARC_CENTER:
				{
					CCadArcCentered *pArc = new CCadArcCentered;
					Token = Accept(TOKEN_ARC_CENTER,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pArc->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pArc->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pArc->m_atrb.m_Start,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pArc->m_atrb.m_End,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pArc->m_atrb.m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pArc->m_atrb.m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pArc);
				}
				break;
			case TOKEN_CIRCLE:
				{
					CCadElipse *pE = new CCadElipse;
					Token = Accept(TOKEN_CIRCLE,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pE->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pE->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pE->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pE->m_FillColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pE->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pE);
				}
				break;
			case TOKEN_RECT:
				{
					CCadRect *pRect = new CCadRect;
					Token = Accept(TOKEN_RECT,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pRect->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pRect->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pRect->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pRect->m_FillColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pRect->m_OutLineWidth,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pRect);
				}
				break;
			case TOKEN_RNDRECT:
				{
					CCadRoundRect *pRR = new CCadRoundRect;
					Token = Accept(TOKEN_RNDRECT,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pRR->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pRR->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pRR->m_P3,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pRR->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pRR->m_FillColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pRR->m_OutLineWidth,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pRR);
				}
				break;
			case TOKEN_POLY:
				{
					int i;
					CCadPolygon *pP = new CCadPolygon;
					Token = Accept(TOKEN_POLY,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Vertex(pIN,pP->m_Count,Token);
					Token = Accept(',',Token,pIN);
					for(i=0;i<pP->m_Count;++i)
					{
						Token = Point(pIN,pP->m_pVertex[i],Token);
						Token = Accept(',',Token,pIN);
					}
					Token = Color(pIN,pP->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pP->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pP);
				}
				break;
			case TOKEN_POLYFILL:
				{
					int i;
					CCadPolygonFill *pP = new CCadPolygonFill;
					Token = Accept(TOKEN_POLYFILL,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Vertex(pIN,pP->m_Count,Token);
					Token = Accept(',',Token,pIN);
					for(i=0;i<pP->m_Count;++i)
					{
						Token = Point(pIN,pP->m_pVertex[i],Token);
						Token = Accept(',',Token,pIN);
					}
					Token = Color(pIN,pP->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pP->m_FillColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pP->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pP);
				}
				break;
			case TOKEN_LINE:
				{
					CCadLine *pLine = new CCadLine;
					Token = Accept(TOKEN_LINE,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pLine->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pLine->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pLine->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pLine->m_OutLineWidth,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pLine);
				}
				break;
			case TOKEN_HOLERND:
				{
					CCadHoleRound *pHR = new CCadHoleRound;
					Token = Accept(TOKEN_HOLERND,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pHR->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Radius(pIN,pHR->m_Radius,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pHR->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pHR->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pHR);
				}
				break;
			case TOKEN_HOLERECT:
				{
					CCadRectHole *pRH = new CCadRectHole;
					Token = Accept(TOKEN_HOLERECT,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pRH->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = RectW(pIN,pRH->m_W,Token);
					Token = Accept(',',Token,pIN);
					Token = RectH(pIN,pRH->m_H,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pRH->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pRH->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pRH);
				}
				break;
			case TOKEN_HOLE_1FLAT:
				{
					CCadHoleRnd1Flat *pHR = new CCadHoleRnd1Flat;
					Token = Accept(TOKEN_HOLE_1FLAT,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pHR->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Radius(pIN,pHR->m_Radius,Token);
					Token = Accept(',',Token,pIN);
					Token = FlatDist(pIN,pHR->m_FlatDist,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pHR->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pHR->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pHR);
				}
				break;
			case TOKEN_HOLE_2FLAT:
				{
					CCadHoleRnd2Flat *pHR = new CCadHoleRnd2Flat;
					Token = Accept(TOKEN_HOLE_2FLAT,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pHR->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Radius(pIN,pHR->m_Radius,Token);
					Token = Accept(',',Token,pIN);
					Token = FlatDist(pIN,pHR->m_FlatDist,Token);
					Token = Accept(',',Token,pIN);
					Token = Color(pIN,pHR->m_LineColor,Token);
					Token = Accept(',',Token,pIN);
					Token = LineWidth(pIN,pHR->m_Width,Token);
					Token = Accept(')',Token,pIN);
					pO->AddObject(pHR);
				}
				break;
			case TOKEN_LIBPART:
				{
					CCadLibObject *pLO;
					pLO = new CCadLibObject;
					Token = LibPart(pIN,Token,pLO);
					if(pO)pO->AddObject(pLO);
				}
				break;
			case TOKEN_BITMAP:
				{
					CCadBitmap *pBM = new CCadBitmap;
					Token = Accept(TOKEN_BITMAP,Token,pIN);
					Token = Accept('(',Token,pIN);
					Token = Point(pIN,pBM->m_P1,Token);
					Token = Accept(',',Token,pIN);
					Token = Point(pIN,pBM->m_P2,Token);
					Token = Accept(',',Token,pIN);
					Token = Accept(TOKEN_FILE,Token,pIN);
					Token = Accept('(',Token,pIN);
					pBM->LoadImage(this->m_LexBuff);
					Token = Accept(TOKEN_STRING,Token,pIN);
					Token = Accept(')',Token,pIN);
					Token = Accept(')',Token,pIN);
					if(pO)pO->AddObject(pBM);
				}
				break;
			case TOKEN_ARROW:
				{
					CCadArrow *pCA = new CCadArrow();
					Token = Accept(TOKEN_ARROW, Token, pIN);
					Token = Accept('(', Token, pIN);
					Token = Point(pIN, pCA->m_P1, Token);
					Token = Accept(',', Token, pIN);
					Token = Point(pIN, pCA->m_P2, Token);
					Token = Accept(',', Token, pIN);
					Token = ReadDecimalValue(TOKEN_LENGTH, Token, pCA->m_Attrib.m_Len, pIN);
					Token = Accept(',', Token, pIN);
					Token = ReadDecimalValue(TOKEN_WIDTH, Token, pCA->m_Attrib.m_LineWidth, pIN);
					Token = Accept(',', Token, pIN);
					Token = this->Color(pIN, pCA->m_Attrib.m_Color, Token);
					Token = Accept(')', Token, pIN);
					if (pO) pO->AddObject(pCA);
				}
				break;
			case '}':
				loop = 0;
				break;
		}//end of switch(Token)
	}	//end of while(loop)
	return Token;
}

int CFileParser::Point(FILE *pIN, CPoint &Point,int Token)
{
	int x,y;

	Token = Accept(TOKEN_POINT,Token,pIN);
	if(Token >= 0)Token = Accept('(',Token,pIN);
	if(Token >= 0)x = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(',',Token,pIN);
	if(Token >= 0)y = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(')',Token,pIN);
	if(Token >= 0)Point = CPoint(x,y);
	return Token;
}

int CFileParser::LibPart(FILE *pIN, int Token,CCadLibObject *pLO)
{
	Token = Accept(TOKEN_LIBPART,Token,pIN);
	Token = Accept('(',Token,pIN);
	pLO->SetName(m_LexBuff);
	Token = Accept(TOKEN_STRING,Token,pIN);
	Token = Accept(',',Token,pIN);
	Token = ReadPointValue(TOKEN_REFERENCE,Token,pLO->m_Ref,pIN);
	Token = Accept(',',Token,pIN);
	Token = ReadPointValue(TOKEN_POINT,Token,pLO->m_P1,pIN);
	Token = Accept(')',Token,pIN);
	Token = Accept('{',Token,pIN);
	Token = DrawFile(pIN,Token,pLO);
	Token = Accept('}',Token,pIN);
	return Token;
}

int CFileParser::Color(FILE *pIN, COLORREF &C,int Token)
{
	int Red,Blue,Green;

	Token = Accept(TOKEN_COLOR,Token,pIN);
	if(Token >= 0)Token = Accept('(',Token,pIN);
	if(Token >= 0)Blue = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(',',Token,pIN);
	if(Token >= 0)Green = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(',',Token,pIN);
	if(Token >= 0)Red = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(')',Token,pIN);
	if(Token >= 0)C = RGB(Red,Green,Blue);
	return Token;
}

int CFileParser::Radius(FILE *pIN, int &Radius,int Token)
{
	Token = Accept(TOKEN_RADIUS,Token,pIN);
	if(Token >= 0)Token = Accept('(',Token,pIN);
	if(Token >= 0)Radius = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM, Token,pIN);
	if(Token >= 0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::FlatDist(FILE *pIN, int &FlatDist,int Token)
{
	Token = Accept(TOKEN_FLATDIST,Token,pIN);
	if(Token >= 0)Token = Accept('(',Token,pIN);
	if(Token >= 0)FlatDist = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM, Token,pIN);
	if(Token >= 0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::LineWidth(FILE *pIN, int &Width,int Token)
{
	Token = Accept(TOKEN_WIDTH,Token,pIN);
	if(Token >=0) Token = Accept('(',Token,pIN);
	if(Token >=0)Width = atoi(this->m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::FontWidth(FILE *pIN, int &Width,int Token)
{
	Token = Accept(TOKEN_WIDTH,Token,pIN);
	if(Token >=0) Token = Accept('(',Token,pIN);
	if(Token >=0)Width = atoi(this->m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::Font(FILE *pIN, char **F,int Token)
{
	Token = Accept(TOKEN_FONT,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)
	{
		int len = strlen(m_LexBuff) + 1;
		*F = new char[len];
		strcpy_s(*F,len,m_LexBuff);
	}
	if(Token >=0)Token = Accept(TOKEN_STRING,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::Weight(FILE *pIN, int &W, int Token)
{
	Token = Accept(TOKEN_WEIGHT,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)W = atoi(m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::Height(FILE *pIN, int &H, int Token)
{
	Token = Accept(TOKEN_HEIGHT,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)H = atoi(m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::Vertex(FILE *pIN, int &N, int Token)
{
	Token = Accept(TOKEN_VERTEX,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)N = atoi(m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::RectW(FILE *pIN, int &W, int Token)
{
	Token = Accept(TOKEN_RECTW,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)W = atoi(m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::RectH(FILE *pIN, int &H, int Token)
{
	Token = Accept(TOKEN_RECTH,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)H = atoi(m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::ReadDecimalValue(int typetoken, int Token, int &v, FILE *pIN)
{
	Token = Accept(typetoken,Token,pIN);
	if(Token >=0)Token = Accept('(',Token,pIN);
	if(Token >=0)v = atoi(m_LexBuff);
	if(Token >=0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >=0)Token = Accept(')',Token,pIN);
	return Token;
}

int CFileParser::ReadPointValue(int TokenType, int Token, CPoint &Point, FILE *pIN)
{
	int x,y;

	Token = Accept(TokenType,Token,pIN);
	if(Token >= 0)Token = Accept('(',Token,pIN);
	if(Token >= 0)x = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(',',Token,pIN);
	if(Token >= 0)y = atoi(m_LexBuff);
	if(Token >= 0)Token = Accept(TOKEN_NUM,Token,pIN);
	if(Token >= 0)Token = Accept(')',Token,pIN);
	if(Token >= 0)Point = CPoint(x,y);
	return Token;
}


int CFileParser::Size(FILE * pIN, CSize & Sz, int Token)
{
	int x, y;

	Token = Accept(TOKEN_SIZE, Token, pIN);
	if (Token >= 0)Token = Accept('(', Token, pIN);
	if (Token >= 0)x = atoi(m_LexBuff);
	if (Token >= 0)Token = Accept(TOKEN_NUM, Token, pIN);
	if (Token >= 0)Token = Accept(',', Token, pIN);
	if (Token >= 0)y = atoi(m_LexBuff);
	if (Token >= 0)Token = Accept(TOKEN_NUM, Token, pIN);
	if (Token >= 0)Token = Accept(')', Token, pIN);
	if (Token >= 0)Sz = CSize(x, y);
	return Token;
}
