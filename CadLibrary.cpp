// CadLibrary.cpp: implementation of the CCadLibrary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "frontcad.h"
#include "CadLibrary.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadLibrary::CCadLibrary()
{
	m_pHead = 0;
	m_pTail = 0;
	m_pNext = 0;
	m_pPrev = 0;
	m_pName = 0;
}

CCadLibrary::~CCadLibrary()
{
	if(m_pName) delete[] m_pName;
}

void CCadLibrary::AddPart(CCadLibObject *pLO)
{
	if(m_pHead == 0)	//nothing in library
	{
		m_pHead = pLO;
		m_pTail = pLO;
	}
	else				//add part to library
	{
		m_pTail->m_pNext = pLO;
		pLO->m_pPrev = m_pTail;
		m_pTail = pLO;
	}
}

void CCadLibrary::DeletePart(CCadLibObject *pLO)
{
	if(pLO == m_pHead)
	{
		m_pHead = (CCadLibObject *)m_pHead->m_pNext;
		m_pHead->m_pPrev = 0;
	}
	else if (pLO == m_pTail)
	{
		m_pTail = (CCadLibObject*)m_pTail->m_pPrev;
		m_pTail->m_pNext = 0;
	}
	else
	{
		pLO->m_pPrev->m_pNext = pLO->m_pNext;
		pLO->m_pNext->m_pPrev = pLO->m_pPrev;
	}
	delete pLO;
}

void CCadLibrary::SetName(char *pN)
{
	if(m_pName) delete[] m_pName;
	m_pName = new char[strlen(pN)+1];
	strcpy_s(m_pName, strlen(pN) + 1,pN);
}

void CCadLibrary::Save(FILE *pO)
{
	fprintf(pO,"LIBRARY(\"%s\"){\n",this->m_pName);
	CCadObject *pOb = m_pHead;
	while(pOb)
	{
		pOb->Save(pO);
		pOb = pOb->m_pNext;
	}
	fprintf(pO,"}\n");
}
