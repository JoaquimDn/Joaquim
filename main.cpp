#include <cstdint>
#include <cstring>
#include <iostream>
using namespace std;

void printSizes() {
cout << "bool: " << sizeof(bool) << endl;
cout << "char: " << sizeof(char) << endl;
cout << "short int: " << sizeof(short int) << endl;
cout << "int: " << sizeof(int) << endl;
cout << "long int: " << sizeof(long int) << endl;
cout << "long long: " << sizeof(long long) << endl;
cout << "float: " << sizeof(float) << endl;
cout << "double: " << sizeof(double) << endl;
cout << "long double: " << sizeof(long double) << endl;
}

void printIntBits(uint32_t u, int width) {
for (int i = width - 1; i >= 0; --i) {
if (i == width - 1)
cout << "[sign]";
cout << ((u >> i) & 1);
if ((width - 1 - i) % 8 == 7)
cout << ' ';
}
cout << endl;
}

void printThreeCodes(int32_t value, int width) {
uint32_t mag = (value < 0) ? (uint32_t)(-value) : (uint32_t)value;
uint32_t mask = (width == 32) ? 0xFFFFFFFFu : ((1u << width) - 1u);
uint32_t magMask = mask >> 1;
mag &= magMask;

uint32_t sign = (value < 0) ? (1u << (width - 1)) : 0;
uint32_t straight = sign | mag;
uint32_t ones = value < 0 ? (sign | ((~mag) & magMask)) : straight;
uint32_t two = (uint32_t)value & mask;

cout << "число: " << value << endl;
cout << "прямой: ";
printIntBits(straight, width);
cout << "обратный: ";
printIntBits(ones, width);
cout << "дополнительный: ";
printIntBits(two, width);
}

void printFloatBits(float f) {
uint32_t bits;
memcpy(&bits, &f, sizeof(bits));

int sign = (bits >> 31) & 1;
int exponent = (bits >> 23) & 0xFF;
int mantissa = bits & 0x7FFFFF;

cout << f << endl;
cout << "[sign]" << sign << " [exp]";
for (int i = 30; i >= 23; --i)
cout << ((bits >> i) & 1);
cout << " [mant]";
for (int i = 22; i >= 0; --i)
cout << ((bits >> i) & 1);
cout << endl;
cout << "sign=" << sign << " exponent=" << exponent
<< " (bias 127, E=" << (exponent - 127) << ") mantissa=" << mantissa;
if (exponent == 0 && mantissa != 0)
cout << " денорм.";
cout << endl;
}

void printDoubleBits(double d) {
uint64_t bits;
memcpy(&bits, &d, sizeof(bits));

int sign = (int)((bits >> 63) & 1);
int exponent = (int)((bits >> 52) & 0x7FF);
unsigned long long mantissa = bits & 0xFFFFFFFFFFFFFULL;

cout << d << endl;
cout << "[sign]" << sign << " [exp]";
for (int i = 62; i >= 52; --i)
cout << ((bits >> i) & 1);
cout << " [mant]";
for (int i = 51; i >= 0; --i)
cout << ((bits >> i) & 1);
cout << endl;
cout << "sign=" << sign << " exponent=" << exponent
<< " (bias 1023, E=" << (exponent - 1023) << ") mantissa=" << mantissa;
if (exponent == 0 && mantissa != 0)
cout << " денорм.";
cout << endl;
}

int main() {
cout << "=== sizeof ===" << endl;
printSizes();
cout << endl;

cout << "=== три кода (16 бит, как в шпаргалке для -3) ===" << endl;
printThreeCodes(3, 16);
cout << endl;
printThreeCodes(-3, 16);
cout << endl;

cout << "=== int в доп. коде (как хранится) ===" << endl;
int32_t pos = 127;
int32_t neg = -127;
cout << pos << " ";
printIntBits((uint32_t)pos, 32);
cout << neg << " ";
printIntBits((uint32_t)neg, 32);
cout << endl;

cout << "=== float IEEE 754 ===" << endl;
printFloatBits(1.0f);
cout << endl;
printFloatBits(-3.14f);
cout << endl;
uint32_t denF = 1; // порядок 0, мантисса 1 — денорм.
float fden;
memcpy(&fden, &denF, sizeof(fden));
printFloatBits(fden);
cout << endl;

cout << "=== double IEEE 754 ===" << endl;
printDoubleBits(1.0);
cout << endl;
printDoubleBits(-3.14);
cout << endl;
uint64_t denD = 1;
double dden;
memcpy(&dden, &denD, sizeof(dden));
printDoubleBits(dden);
cout << endl;

cout << "=== смена бита k ===" << endl;
uint32_t x = 0;
cout << "до: ";
printIntBits(x, 32);
x |= (1u << 0); // установить бит 0
cout << "|= 1<<0: ";
printIntBits(x, 32);
x |= (1u << 7); // установить бит 7
cout << "|= 1<<7: ";
printIntBits(x, 32);
x &= ~(1u << 0); // сбросить бит 0
cout << "&= ~1: ";
printIntBits(x, 32);
x ^= (1u << 5); // инвертировать бит 5
cout << "^= 1<<5: ";
printIntBits(x, 32);

uint32_t fb;
float f = 1.0f;
memcpy(&fb, &f, sizeof(fb));
fb ^= (1u << 31); // инверсия знака float
memcpy(&f, &fb, sizeof(f));
cout << endl << "1.0f, инверсия бита 31:" << endl;
printFloatBits(f);

uint64_t db;
double d = 1.0;
memcpy(&db, &d, sizeof(db));
db ^= (1ull << 63);
memcpy(&d, &db, sizeof(d));
cout << endl << "1.0, инверсия бита 63:" << endl;
printDoubleBits(d);

return 0;
}