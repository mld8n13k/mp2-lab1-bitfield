// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "Invalid length";
	BitLen = len;
	MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& bf)
// конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField() // деструктор

{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) throw "Index out of range";
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen) throw "Index out of range";
	return 1 << (n % (sizeof(TELEM) * 8));

}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) throw "Index out of range";
	int idx = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[idx] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) throw "Index out of range";
	int idx = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[idx] &= ~mask;

}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) throw "Index out of range";
	int idx = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[idx] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf)
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField result(maxLen);

	int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

	// Копируем общую часть
	for (int i = 0; i < minMemLen; i++)
		result.pMem[i] = pMem[i] | bf.pMem[i];

	// Копируем хвост из большего поля ПРАВИЛЬНО
	if (BitLen > bf.BitLen) {
		for (int i = minMemLen; i < MemLen; i++)
			result.pMem[i] = pMem[i];  // Копируем хвост из текущего объекта
	}
	else if (bf.BitLen > BitLen) {
		for (int i = minMemLen; i < bf.MemLen; i++)
			result.pMem[i] = bf.pMem[i];  // Копируем хвост из второго объекта
	}

	return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
	TBitField result(maxLen);

	int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;

	for (int i = 0; i < minMemLen; i++)
		result.pMem[i] = pMem[i] & bf.pMem[i];

	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (size_t i = 0; i < MemLen; i++) {
		result.pMem[i] = ~pMem[i];
	}
	TELEM rem = BitLen % (sizeof(TELEM) * 8);
	if (rem > 0) {
		TELEM m = (1 << rem) - 1;
		result.pMem[MemLen - 1] &= m;
	}

	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	string input;
	istr >> input;

	if (input.length() > bf.BitLen) {
		throw "Input string too long";
	}

	for (int i = 0; i < bf.MemLen; i++) {
		bf.pMem[i] = 0;
	}

	for (size_t i = 0; i < input.length(); i++) {
		if (input[i] == '1') {
			bf.SetBit(i);
		}
		else if (input[i] != '0') {
			throw "Invalid input character";
		}
	}

	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << (bf.GetBit(i) ? '1' : '0');
	}
	return ostr;
}