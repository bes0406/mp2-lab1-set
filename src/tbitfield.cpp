// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) // конструктор
{
	if (len>0)
	{
	BitLen=len;
    //MemLen=(len+sizeof(TELEM)-1)/sizeof(TELEM);
	MemLen=BitLen/(sizeof(TELEM)*8)+1;
	pMem=new TELEM[MemLen];
	for(int i=0; i<MemLen; i++)
		pMem[i]=0;
	}
	else { throw invalid_argument("Len<=0..."); }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM[MemLen];
	for (int i=0; i<MemLen; i++)
		pMem[i]=bf.pMem[i];
}

TBitField::~TBitField() //деструктор
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/(sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int k=n%(sizeof(TELEM)*8);
	TELEM M=1;
	M=M<<k;
	return M;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	int len=BitLen;
	return len;
    // return 0; (было)
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n>=0)&&(n<BitLen))
	{int index;
	index=GetMemIndex(n);
	TELEM Mask;
	Mask=GetMemMask(n);
	pMem[index]|=Mask;}
	else { throw invalid_argument("Len<=0..."); }
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n>=0)&&(n<BitLen))
	{int index;
	index=GetMemIndex(n);
	TELEM Mask;
	Mask=GetMemMask(n);
	pMem[index]&=~Mask;}
	else { throw invalid_argument("Len<=0..."); }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n>=0)&&(n<BitLen))
		return pMem[GetMemIndex(n)]&GetMemMask(n);
	else { throw invalid_argument("Len<=0..."); }
	//return 0;
	/*int fl=1;                                                            !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int index=GetMemIndex(n);
	TELEM Mask=GetMemMask(n);
	Mask=Mask&pMem[index];
	if (Mask=0) fl=0;
	return fl;*/
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this!= &bf)
	{
		delete []pMem;
		BitLen=bf.BitLen;
	    MemLen=bf.MemLen;
	    pMem=new TELEM[MemLen];
	    for (int i=0; i<MemLen; i++)
			pMem[i]=bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res=1;
	if (BitLen!=bf.BitLen) res=0;
	else
		for (int i=0; i<MemLen; i++)
			if (pMem[i]!=bf.pMem[i]) 
			{
				res=0;
				break;
			}
	return res;
  }

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int res=0;
	if (BitLen!=bf.BitLen) res=1;
	else
		for (int i=0; i<MemLen; i++)
			if (pMem[i]!=bf.pMem[i]) 
			{
				res=1;
				break;
			}
	return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i, len=BitLen;
	if (bf.BitLen>len) len=bf.BitLen;
	TBitField temp(len);
	for (i=0; i<MemLen; i++)
		temp.pMem[i]=pMem[i];
	for (i=0; i<bf.MemLen; i++)
		temp.pMem[i]|=bf.pMem[i];
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i, len=BitLen;
	if (bf.BitLen>len) len=bf.BitLen;
	TBitField temp(len);
	for (i=0; i<MemLen; i++)
		temp.pMem[i]=pMem[i];
	for (i=0; i<bf.MemLen; i++)
		temp.pMem[i]&=bf.pMem[i];
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	/*int len=BitLen;  
	TBitField temp(len);
	for (int i=0; i<MemLen; i++)
		temp.pMem[i]=~pMem[i];
	return temp;*/
	TBitField temp(BitLen);
	for (int i=0;i<BitLen; i++)
		if ( GetBit(i)==0) temp.SetBit(i);
		else temp.ClrBit(i);
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	// формат данных - последовательность из 0 и 1 без пробелов
	// начальные пробелы игнорируются
	// при получении не 0 и 1 - завершение ввода
	int i=0;
	char ch;
	// поиск
	do {istr >> ch;} while (ch!=' ');
	//ввод элементов и включение в множество
	while (1)
		{
			istr >> ch;
			if (ch=='0') bf.ClrBit(i++);
			else if (ch=='1') bf.SetBit(i++);
			     else break;
		}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	// формат данных - последовательность 0 и 1
	//вывод элементов
	int len=bf.GetLength();
	for (int i=0; i<len; i++)
		if (bf.GetBit(i)) ostr << '1';
		else ostr << '0';
	return ostr;
}
