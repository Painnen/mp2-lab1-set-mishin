

#include "tset.h"


// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetBitLen()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return TBitField(BitField);
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
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(BitField == s.BitField);;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField res = BitField | s.BitField;
    TSet result(res);
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem <= MaxPower)
    {
        TSet res(*this);
        res.BitField.SetBit(Elem);
        return res;
    }
    else
    {
        throw std::out_of_range("element doesn't exist in universal set");
    }
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem <= MaxPower)
    {
        TSet res(*this);
        BitField.ClrBit(Elem);
        return res;
    }
    else
    {
        throw std::out_of_range("element doesn't exist in universal set");
    }
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField res = BitField & s.BitField;
    TSet result(res);
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TBitField res = ~BitField;
    TSet result(res);
    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
