#include "GCrypt.h"
#include "RandY.h"

#define N 50

const unsigned char baseKey [] = {
	192	,
	100	,
	14	,
	173	,
	151	,
	169	,
	181	,
	102	,
	38	,
	40	,
	242	,
	240	,
	125	,
	153	,
	3	,
	233	,
	161	,
	231	,
	250	,
	184	,
	143	,
	198	,
	158	,
	163	,
	93	,
	126	,
	22	,
	144	,
	219	,
	148	,
	129	,
	116	,
	172	,
	51	,
	233	,
	33	,
	89	,
	159	,
	18	,
	44	,
	55	,
	38	,
	182	,
	14	,
	26	,
	81	,
	250	,
	242	,
	236	,
	221
};

unsigned char PS[50];
unsigned char T[256],IT[256];
		
unsigned char GCrypt_Key(const char* keyValue,unsigned int n)
{
	unsigned int x;
	int i;
	x=0;
	for (i=0;i<N;i++)
	{
		x += PS[i] * keyValue[i % n];
	}
	return (x % 256);
}

void GCrypt_Initialize(const char* keyValue,unsigned int length)
{
	int i,j,k,n;
	unsigned char tmp;
	// Initialize PS "Key Vector"
	for (i=0;i<N;i++)
	{
		PS[i]=baseKey[i];
	}
	for (i=0;i<N;i++)
	{
		PS[i]=GCrypt_Key(keyValue,length);
	}
	
	// Initialize T and IT "Scramble Matrix and Inverse Scramble Matrix"
	for (i=0;i<256;i++)
		T[i]=i;
	for (k=0;k<125;k++)
	{
		for (n=0;n<256;n++)
		{
			tmp=T[PS[n % N]];
			T[PS[n % N]]=T[n];
			T[n]=tmp;
		}
	}
	for (i=0;i<256;i++)
	{
		IT[i]=0;
		for (j=0;j<256;j++)
		{
			if (T[j]==i)
				IT[i]=j;
		}
	}
}

void GCrypt_encipher(unsigned char* PlainText,unsigned int n)
{
	unsigned char InitVal,x;
	unsigned int i;
	InitVal=PS[N-1];
	for(i=0;i<n;i++)
	{
		x = (PlainText[i] ^ InitVal);
		x = T[x];
		PlainText[i]=x;
		InitVal = x;
	}
}

void GCrypt_decipher(unsigned char* CriptText,unsigned int n)
{
	unsigned char InitVal,x;
	unsigned int i;
	InitVal=PS[N-1];
	for(i=0;i<n;i++)
	{
		x = IT[CriptText[i]] ^ InitVal;
		InitVal=CriptText[i];
		CriptText[i] = x;
	}
}

void GCrypt(const char* command,unsigned char* testo, unsigned int n)
{
	if (command[0]=='c')
		GCrypt_encipher (testo,n);
	if (command[0]=='d')
		GCrypt_decipher (testo,n);
}

/////////////////////////////////////////////////////////
// Procedura di Hashing della password
/////////////////////////////////////////////////////////

#include "RandY.h"

// Ritorna il Byte iesimo 3 2 1 0 del int
unsigned char GetByte(unsigned int iVal,unsigned int iPos)
{
	return (unsigned char)(iVal>>(iPos * 8));
}

// Scrive il byte iesimo 3 2 1 0 del int
void SetByte(unsigned int* iVal,unsigned int iPos, unsigned char cValue)
{
	unsigned int iTemp,iMask;
	iTemp=0;
	iTemp = cValue;
	iMask = 0xFF;
	iTemp = iTemp << (iPos * 8);
	iMask = iMask << (iPos * 8);
	*iVal = (*iVal) & (~iMask);
	*iVal = (*iVal) | (iTemp);
}

void GHashPass(const unsigned char* pass,unsigned int n,unsigned int* pH, unsigned int* pL)
{
	unsigned int i,j;
	*pH = 0;
	for (i=0;i<n;i++)
	{
		*pH += pass[i];
	}
	*pL = 0;
	for (i=0;i<n;i++)
	{
		*pL += RandY[pass[i]];
	}
	for (i=0;i<n;i++)
	{
		for (j=0;j<4;j++)
		{
			SetByte(pL,3-j,GetByte(*pH,j) ^ pass[i]);
			SetByte(pH,3-j,GetByte(*pL,j) ^ RandY[pass[i]]);
		}
	}
}


