#include "tbitfield.h"

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}

 int TBitField::GetBitLen() const
{
     return BitLen;
}

TBitField::TBitField() : MemLen(0), BitLen(0), pMem(nullptr)
{}

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("negative length");
    else
    {
        BitLen = len;
        MemLen = GetMemIndex(len) + 1;
        pMem = new int[MemLen];
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = 0;
        }
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new int[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::TBitField(TBitField&& bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = bf.pMem;
    bf.pMem = nullptr;
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n/(sizeof(int)* 8);
}
int TBitField::GetBitIndex(const int n) const
{
    return n % (sizeof(int) * 8);
}

int TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << GetBitIndex(n); 
}



int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen; //или return this->BitLen? (нужно только для различия с именованием параметра)
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen - 1)
    {
        throw std::invalid_argument("index out of range");
    }
    else
    {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen - 1)
    {
        throw std::invalid_argument("index out of range");
    }
    else
    {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  if (n < 0 || n > BitLen - 1)
  {
    throw std::invalid_argument("index out of range");
  }
  else
        return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this != bf)
    {
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        delete[] pMem;
        pMem = new int[MemLen];
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    else
    {
        return *this;
    }
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen || MemLen != bf.MemLen)
        return 0;
    int range = MemLen - 1;
    for (int i = 0; i < range; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    int start = range * 32;
    for (int j = start; j < BitLen; j++)
        if (GetBit(j) != bf.GetBit(j))
            return 0;
    return 1;//сначала по интам, затем по битам
    /* по битам
    for(int i = 0; i < Bitlen; i++)
    {
        if (GetBit(i) != bf.GetBit(i))
            return 0;
    }
    return 1;
    */
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int res_len = max(BitLen, bf.BitLen);
    int range = min(BitLen, bf.BitLen);
    TBitField result(res_len);
    for (int i = 0; i < range; i++)
    {
        if (GetBit(i) == 0 && bf.GetBit(i) == 0)
            result.ClrBit(i);
        else
            result.SetBit(i);
    }
    if (BitLen > bf.BitLen)
    {
        for (int i = range; i < BitLen; i++)
        {
            if (GetBit(i) == 0)
                result.ClrBit(i);
            else
                result.SetBit(i);
        }
    }
    else
    {
        for (int i = range; i < bf.BitLen; i++)
        {
            if (bf.GetBit(i) == 0)
                result.ClrBit(i);
            else
                result.SetBit(i);
        }
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    /*if (BitLen != bf.BitLen)
        throw std::invalid_argument("Bit fields must be equal size for and operation");
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i) == 0 || bf.GetBit(i) == 0)
            result.ClrBit(i);
        else
            result.SetBit(i);
    }
    return result;//в общем смысле для битовых полей

    //c точки зрения теории множеств
    int res_len = min(BitLen, bf.BitLen);
    TBitField result(res_len);
    for (int i = 0; i < result.BitLen; i++)
    {
        if (GetBit(i) == 0 || bf.GetBit(i) == 0)
            result.ClrBit(i);
        else
            result.SetBit(i);
    }
    return result;
    //с точки зрения теории множеств и сохранении длина битового поля*/

    int res_len = max(BitLen, bf.BitLen);
    int range = min(BitLen, bf.BitLen);
    TBitField result(res_len);
    for (int i = 0; i < range; i++)
    {
        if (GetBit(i) == 0 || bf.GetBit(i) == 0)
            result.ClrBit(i);
        else
            result.SetBit(i);
    }
    for (int i = range; i < result.BitLen; i++)
    {
        result.ClrBit(i);
    }
    return result;
}


TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++)
        if (GetBit(i) != 0)
            result.ClrBit(i);
        else
            result.SetBit(i);
    return result;
}

// ввод/вывод

std::istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int element;
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (istr.peek() == '\n' || istr.peek() == EOF)
            break;
        istr >> element;
        if (element == 0)
            bf.ClrBit(i);
        else if (element == 1)
            bf.SetBit(i);
        else
            throw std::invalid_argument("only 0 or 1");
    }
    
    return istr;
}

std::ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
