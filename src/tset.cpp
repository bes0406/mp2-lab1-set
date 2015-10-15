// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp) // конструктор
{
	MaxPower=mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField() //преобразование типа к TBitField
{
	TBitField temp(this->BitField);
	return temp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
	MaxPower=s.GetMaxPower();
	BitField=s.BitField;
	return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (BitField==s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
	return (BitField!=s.BitField);
}

TSet TSet::operator+(const TSet &s) // объединение
{
	int Power;
	if (MaxPower>s.MaxPower) Power=MaxPower;
	else Power=s.MaxPower;
	TSet C(Power);
	C.BitField=BitField|s.BitField;
	return (C);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	TSet C(MaxPower);
	C.BitField=BitField;
	C.BitField.SetBit(Elem); 
	return (C);//ИЗМЕНИТЬ!!!!!!!!!!!!!!
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	TSet C(MaxPower);
	C.BitField=BitField;
	C.BitField.ClrBit(Elem); 
	return (C); //ИЗМЕНИТЬ!!!!!!!!!!!!!!
}

TSet TSet::operator*(const TSet &s) // пересечение
{
	TSet C(MaxPower);
	C.BitField=BitField&s.BitField;
	return (C);
}

TSet TSet::operator~(void) // дополнение
{
	TSet C(MaxPower);
	C.BitField=~BitField;
	return(C);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
	// формат данных - {i1,i2,...,in}
	int temp;
	char ch;
	//поиск
	do {istr>>ch;} while (ch!='{');
	do
	{
		//ввод элементов и включение в множество
		istr >> temp;
		s.InsElem(temp);
		do {istr >> ch;} while ((ch!=',') && (ch!='}'));
	}
	while (ch!='}');
	return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
	//формат данных - {i1,i2,...,in}
	int i,n;
	char ch=' ';
	ostr << "{";
	n=s.GetMaxPower();
	for (i=0;i<n;i++)
	{
		//вывод элементов
		if (s.IsMember(i))
		{
			ostr << ch << ' '<< i; ch=',';
		}
	}
	ostr << "}";
	return ostr;
}
