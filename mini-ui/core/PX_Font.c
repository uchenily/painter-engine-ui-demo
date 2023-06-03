
#include "PX_Font.h"

static px_uchar const AsciiLib[95][16] = {
    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*" ",0*/

    {0x00,
     0x00,
     0x00,
     0x18,
     0x3C,
     0x3C,
     0x3C,
     0x18,
     0x18,
     0x00,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00}, /*"!",1*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*""",2*/

    {0x00,
     0x00,
     0x00,
     0x36,
     0x36,
     0x7F,
     0x36,
     0x36,
     0x36,
     0x7F,
     0x36,
     0x36,
     0x00,
     0x00,
     0x00,
     0x00}, /*"#",3*/

    {0x00,
     0x18,
     0x18,
     0x3C,
     0x66,
     0x60,
     0x30,
     0x18,
     0x0C,
     0x06,
     0x66,
     0x3C,
     0x18,
     0x18,
     0x00,
     0x00}, /*"$",4*/

    {0x00,
     0x00,
     0x70,
     0xD8,
     0xDA,
     0x76,
     0x0C,
     0x18,
     0x30,
     0x6E,
     0x5B,
     0x1B,
     0x0E,
     0x00,
     0x00,
     0x00}, /*"%",5*/

    {0x00,
     0x00,
     0x00,
     0x38,
     0x6C,
     0x6C,
     0x38,
     0x60,
     0x6F,
     0x66,
     0x66,
     0x3B,
     0x00,
     0x00,
     0x00,
     0x00}, /*"&",6*/

    {0x00,
     0x00,
     0x00,
     0x18,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"'",7*/

    {0x00,
     0x00,
     0x00,
     0x0C,
     0x18,
     0x18,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x18,
     0x18,
     0x0C,
     0x00,
     0x00}, /*"(",8*/

    {0x00,
     0x00,
     0x00,
     0x30,
     0x18,
     0x18,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x18,
     0x18,
     0x30,
     0x00,
     0x00}, /*")",9*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x36,
     0x1C,
     0x7F,
     0x1C,
     0x36,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"*",10*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x18,
     0x18,
     0x7E,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"+",11*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x1C,
     0x1C,
     0x0C,
     0x18,
     0x00,
     0x00}, /*",",12*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"-",13*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x1C,
     0x1C,
     0x00,
     0x00,
     0x00,
     0x00}, /*".",14*/

    {0x00,
     0x00,
     0x00,
     0x06,
     0x06,
     0x0C,
     0x0C,
     0x18,
     0x18,
     0x30,
     0x30,
     0x60,
     0x60,
     0x00,
     0x00,
     0x00}, /*"/",15*/

    {0x00,
     0x00,
     0x00,
     0x1E,
     0x33,
     0x37,
     0x37,
     0x33,
     0x3B,
     0x3B,
     0x33,
     0x1E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"0",16*/

    {0x00,
     0x00,
     0x00,
     0x0C,
     0x1C,
     0x7C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"1",17*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x06,
     0x0C,
     0x18,
     0x30,
     0x60,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"2",18*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x06,
     0x1C,
     0x06,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"3",19*/

    {0x00,
     0x00,
     0x00,
     0x30,
     0x30,
     0x36,
     0x36,
     0x36,
     0x66,
     0x7F,
     0x06,
     0x06,
     0x00,
     0x00,
     0x00,
     0x00}, /*"4",20*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0x60,
     0x60,
     0x60,
     0x7C,
     0x06,
     0x06,
     0x0C,
     0x78,
     0x00,
     0x00,
     0x00,
     0x00}, /*"5",21*/

    {0x00,
     0x00,
     0x00,
     0x1C,
     0x18,
     0x30,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"6",22*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0x06,
     0x0C,
     0x0C,
     0x18,
     0x18,
     0x30,
     0x30,
     0x30,
     0x00,
     0x00,
     0x00,
     0x00}, /*"7",23*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x76,
     0x3C,
     0x6E,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"8",24*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3E,
     0x0C,
     0x18,
     0x38,
     0x00,
     0x00,
     0x00,
     0x00}, /*"9",25*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x1C,
     0x1C,
     0x00,
     0x00,
     0x00,
     0x1C,
     0x1C,
     0x00,
     0x00,
     0x00,
     0x00}, /*":",26*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x1C,
     0x1C,
     0x00,
     0x00,
     0x00,
     0x1C,
     0x1C,
     0x0C,
     0x18,
     0x00,
     0x00}, /*";",27*/

    {0x00,
     0x00,
     0x00,
     0x06,
     0x0C,
     0x18,
     0x30,
     0x60,
     0x30,
     0x18,
     0x0C,
     0x06,
     0x00,
     0x00,
     0x00,
     0x00}, /*"<",28*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x7E,
     0x00,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"=",29*/

    {0x00,
     0x00,
     0x00,
     0x60,
     0x30,
     0x18,
     0x0C,
     0x06,
     0x0C,
     0x18,
     0x30,
     0x60,
     0x00,
     0x00,
     0x00,
     0x00}, /*">",30*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x0C,
     0x18,
     0x18,
     0x00,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00}, /*"?",31*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0xC3,
     0xC3,
     0xCF,
     0xDB,
     0xDB,
     0xCF,
     0xC0,
     0x7F,
     0x00,
     0x00,
     0x00,
     0x00}, /*"@",32*/

    {0x00,
     0x00,
     0x00,
     0x18,
     0x3C,
     0x66,
     0x66,
     0x66,
     0x7E,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"A",33*/

    {0x00,
     0x00,
     0x00,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x7C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"B",34*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x60,
     0x60,
     0x60,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"C",35*/

    {0x00,
     0x00,
     0x00,
     0x78,
     0x6C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x6C,
     0x78,
     0x00,
     0x00,
     0x00,
     0x00}, /*"D",36*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0x60,
     0x60,
     0x60,
     0x7C,
     0x60,
     0x60,
     0x60,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"E",37*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0x60,
     0x60,
     0x60,
     0x7C,
     0x60,
     0x60,
     0x60,
     0x60,
     0x00,
     0x00,
     0x00,
     0x00}, /*"F",38*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x60,
     0x60,
     0x6E,
     0x66,
     0x66,
     0x3E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"G",39*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x7E,
     0x66,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"H",40*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"I",41*/

    {0x00,
     0x00,
     0x00,
     0x06,
     0x06,
     0x06,
     0x06,
     0x06,
     0x06,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"J",42*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x6C,
     0x6C,
     0x78,
     0x6C,
     0x6C,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"K",43*/

    {0x00,
     0x00,
     0x00,
     0x60,
     0x60,
     0x60,
     0x60,
     0x60,
     0x60,
     0x60,
     0x60,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"L",44*/

    {0x00,
     0x00,
     0x00,
     0x63,
     0x63,
     0x77,
     0x6B,
     0x6B,
     0x6B,
     0x63,
     0x63,
     0x63,
     0x00,
     0x00,
     0x00,
     0x00}, /*"M",45*/

    {0x00,
     0x00,
     0x00,
     0x63,
     0x63,
     0x73,
     0x7B,
     0x6F,
     0x67,
     0x63,
     0x63,
     0x63,
     0x00,
     0x00,
     0x00,
     0x00}, /*"N",46*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"O",47*/

    {0x00,
     0x00,
     0x00,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x7C,
     0x60,
     0x60,
     0x60,
     0x60,
     0x00,
     0x00,
     0x00,
     0x00}, /*"P",48*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x0C,
     0x06,
     0x00,
     0x00}, /*"Q",49*/

    {0x00,
     0x00,
     0x00,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x7C,
     0x6C,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"R",50*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x60,
     0x30,
     0x18,
     0x0C,
     0x06,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"S",51*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00}, /*"T",52*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"U",53*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00}, /*"V",54*/

    {0x00,
     0x00,
     0x00,
     0x63,
     0x63,
     0x63,
     0x6B,
     0x6B,
     0x6B,
     0x36,
     0x36,
     0x36,
     0x00,
     0x00,
     0x00,
     0x00}, /*"W",55*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x34,
     0x18,
     0x18,
     0x2C,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"X",56*/

    {0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x18,
     0x18,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00}, /*"Y",57*/

    {0x00,
     0x00,
     0x00,
     0x7E,
     0x06,
     0x06,
     0x0C,
     0x18,
     0x30,
     0x60,
     0x60,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"Z",58*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x3C,
     0x00}, /*"[",59*/

    {0x00,
     0x00,
     0x00,
     0x60,
     0x60,
     0x30,
     0x30,
     0x18,
     0x18,
     0x0C,
     0x0C,
     0x06,
     0x06,
     0x00,
     0x00,
     0x00}, /*"\",60*/

    {0x00,
     0x00,
     0x00,
     0x3C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x3C,
     0x00}, /*"]",61*/

    {0x00,
     0x18,
     0x3C,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"^",62*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0xFF,
     0x00}, /*"_",63*/

    {0x00,
     0x00,
     0x00,
     0x18,
     0x18,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"'",64*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3C,
     0x06,
     0x06,
     0x3E,
     0x66,
     0x66,
     0x3E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"a",65*/

    {0x00,
     0x00,
     0x00,
     0x60,
     0x60,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x7C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"b",66*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x60,
     0x60,
     0x60,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"c",67*/

    {0x00,
     0x00,
     0x00,
     0x06,
     0x06,
     0x3E,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"d",68*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x7E,
     0x60,
     0x60,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"e",69*/

    {0x00,
     0x00,
     0x00,
     0x1E,
     0x30,
     0x30,
     0x30,
     0x7E,
     0x30,
     0x30,
     0x30,
     0x30,
     0x00,
     0x00,
     0x00,
     0x00}, /*"f",70*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3E,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3E,
     0x06,
     0x06,
     0x7C,
     0x00}, /*"g",71*/

    {0x00,
     0x00,
     0x00,
     0x60,
     0x60,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"h",72*/

    {0x00,
     0x00,
     0x18,
     0x18,
     0x00,
     0x78,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"i",73*/

    {0x00,
     0x00,
     0x0C,
     0x0C,
     0x00,
     0x3C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x0C,
     0x78,
     0x00}, /*"j",74*/

    {0x00,
     0x00,
     0x00,
     0x60,
     0x60,
     0x66,
     0x66,
     0x6C,
     0x78,
     0x6C,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"k",75*/

    {0x00,
     0x00,
     0x00,
     0x78,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"l",76*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x7E,
     0x6B,
     0x6B,
     0x6B,
     0x6B,
     0x6B,
     0x63,
     0x00,
     0x00,
     0x00,
     0x00}, /*"m",77*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"n",78*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"o",79*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x7C,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x7C,
     0x60,
     0x60,
     0x60,
     0x00}, /*"p",80*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3E,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3E,
     0x06,
     0x06,
     0x06,
     0x00}, /*"q",81*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x66,
     0x6E,
     0x70,
     0x60,
     0x60,
     0x60,
     0x60,
     0x00,
     0x00,
     0x00,
     0x00}, /*"r",82*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x3E,
     0x60,
     0x60,
     0x3C,
     0x06,
     0x06,
     0x7C,
     0x00,
     0x00,
     0x00,
     0x00}, /*"s",83*/

    {0x00,
     0x00,
     0x00,
     0x30,
     0x30,
     0x7E,
     0x30,
     0x30,
     0x30,
     0x30,
     0x30,
     0x1E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"t",84*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"u",85*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x18,
     0x00,
     0x00,
     0x00,
     0x00}, /*"v",86*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x63,
     0x6B,
     0x6B,
     0x6B,
     0x6B,
     0x36,
     0x36,
     0x00,
     0x00,
     0x00,
     0x00}, /*"w",87*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x3C,
     0x18,
     0x3C,
     0x66,
     0x66,
     0x00,
     0x00,
     0x00,
     0x00}, /*"x",88*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x66,
     0x3C,
     0x0C,
     0x18,
     0xF0,
     0x00}, /*"y",89*/

    {0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x7E,
     0x06,
     0x0C,
     0x18,
     0x30,
     0x60,
     0x7E,
     0x00,
     0x00,
     0x00,
     0x00}, /*"z",90*/

    {0x00,
     0x00,
     0x00,
     0x0C,
     0x18,
     0x18,
     0x18,
     0x30,
     0x60,
     0x30,
     0x18,
     0x18,
     0x18,
     0x0C,
     0x00,
     0x00}, /*"{",91*/

    {0x00,
     0x00,
     0x00,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x18,
     0x00}, /*"|",92*/

    {0x00,
     0x00,
     0x00,
     0x30,
     0x18,
     0x18,
     0x18,
     0x0C,
     0x06,
     0x0C,
     0x18,
     0x18,
     0x18,
     0x30,
     0x00,
     0x00}, /*"}",93*/

    {0x00,
     0x00,
     0x00,
     0x71,
     0xDB,
     0x8E,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00,
     0x00}, /*"~",94*/
};

px_void PX_GetASCIICode(px_uchar *pBuffer, px_uchar ASCII) {
    if (ASCII - 32 < 95)
        PX_memcpy(pBuffer, (px_void *) AsciiLib[(ASCII - 32)], 16);
    else
        PX_memset(pBuffer, 0xff, 16);
}

px_int PX_FontDrawChar(
    px_surface *psurface, px_int x, px_int y, px_uchar ASCI, px_color Color) {
    unsigned short i, j;
    px_uchar buffer[16], tmp_char;

    if (ASCI == ' ') {
        return 0;
    }

    PX_GetASCIICode(buffer, ASCI);

    for (i = 0; i < 16; i++) {
        tmp_char = buffer[i];
        for (j = 0; j < 8; j++) {
            if ((tmp_char >> (7 - j)) & 0x01) {
                PX_SurfaceDrawPixel(psurface, x + j, y + i, Color);
            }
        }
    }
    return 16;
}

px_int PX_FontDrawText(px_surface *psurface,
                       px_int x,
                       px_int y,
                       PX_ALIGN align,
                       const px_char *Text,
                       px_color Color) {
    px_uchar TempChar;
    px_int resX = x;
    px_int xlen = 0;
    px_int frWidth, frHeight;
    const px_char *CalcText = Text;

    PX_FontTextGetRenderWidthHeight(Text, &frWidth, &frHeight);

    switch (align) {
        case PX_ALIGN_LEFTTOP:
            break;
        case PX_ALIGN_MIDTOP:
            x -= frWidth / 2;
            break;
        case PX_ALIGN_RIGHTTOP:
            x -= frWidth;
            break;
        case PX_ALIGN_LEFTMID:
            y -= frHeight / 2;
            break;
        case PX_ALIGN_CENTER:
            y -= frHeight / 2;
            x -= frWidth / 2;
            break;
        case PX_ALIGN_RIGHTMID:
            y -= frHeight / 2;
            x -= frWidth;
            break;
        case PX_ALIGN_LEFTBOTTOM:
            y -= frHeight;
            break;
        case PX_ALIGN_MIDBOTTOM:
            y -= frHeight;
            x -= frWidth / 2;
            break;
        case PX_ALIGN_RIGHTBOTTOM:
            y -= frHeight;
            x -= frWidth;
            break;
    }

    while (*Text != 0) {
        if (*Text == '\r' || *Text == '\n') {
            y += __PX_FONT_HEIGHT + 4;
            x = resX;
            Text++;
            continue;
        }

        TempChar = *Text++;
        PX_FontDrawChar(psurface, x, y, TempChar, Color);
        x += __PX_FONT_ASCSIZE;
        continue;
    }
    return x - resX + 1;
}

px_void PX_FontTextGetRenderWidthHeight(const px_char *Text,
                                        px_int *width,
                                        px_int *height) {
    px_int x = 0, y = 0;
    px_int max_width = 0, max_height = 0;

    while (*Text != 0) {
        if (*Text == '\r' || *Text == '\n') {
            y += __PX_FONT_HEIGHT + 4;
            if (y > max_height) {
                max_height = y;
            }
            Text++;
            continue;
        }

        if ((*Text) & 0x80) {
            // not support
            Text++;
            continue;
        }

        if (*Text == ' ') {
            Text++;
            x += __PX_FONT_ASCSIZE;
            if (x > max_width) {
                max_width = x;
            }
            continue;
        }

        Text++;

        x += __PX_FONT_ASCSIZE;
        if (x > max_width) {
            max_width = x;
        }
        if (y + __PX_FONT_HEIGHT > max_height) {
            max_height = y + __PX_FONT_HEIGHT;
        }
    }
    *width = max_width;
    *height = max_height;
}

px_int PX_FontModuleGetCharacterCode(PX_FONTMODULE_CODEPAGE codePage,
                                     const px_char *Text,
                                     px_dword *code) {
    const px_uchar *uText = (const px_uchar *) Text;

    switch (codePage) {
        case PX_FONTMODULE_CODEPAGE_GBK:
            if (*uText == 0) {
                *code = 0;
                return 0;
            }
            if ((*uText) & 0x80) {
                *code = (px_dword) ((*uText) + *(uText + 1) * 256);
                return 2;
            } else {
                *code = (px_dword) (*uText);
                return 1;
            }
            break;
        case PX_FONTMODULE_CODEPAGE_UTF8: {
            px_dword bc = 1;
            px_dword rcode = 0;
            px_uchar pageHead = (px_uchar) (*uText);
            px_uchar oftbit = 0x80;
            if (pageHead == 0) {
                return 0;
            }

            rcode = (*uText);
            while (1) {
                if ((pageHead & oftbit) == 0) {
                    break;
                }

                oftbit >>= 1;

                if ((pageHead & oftbit) == 0) {
                    break;
                }
                uText++;
                rcode += ((*uText) << (bc * 8));
                bc++;
            };
            *code = rcode;
            return bc;
        } break;
        case PX_FONTMODULE_CODEPAGE_UTF16: {
            *code = (px_dword) ((*uText) + *(uText + 1) * 256);
            if (*code == 0) {
                return 0;
            }
            return 2;
        } break;
        default:
            return 0;
            break;
    }
    *code = 0;
    return 0;
}

px_bool PX_FontModuleInitialize(px_memorypool *mp, PX_FontModule *module) {
    module->mp = mp;
    module->codePage = PX_FONTMODULE_CODEPAGE_UNDEFINED;
    module->max_BearingY = 0;
    module->max_Height = 0;
    module->max_Width = 0;
    return PX_MapInitialize(mp, &module->characters_map);
}

px_bool PX_FontModuleLoad(PX_FontModule *module, px_byte *buffer, px_int size) {
    px_int offset = 0;
    PX_FontModule_Charactor *cpy;

    if (module->mp == PX_NULL) {
        PX_ASSERT();
        return PX_FALSE;
    }

    while (offset < size) {
        px_char hex[16] = {0};
        px_byte *pData;

        PX_FontModule_Charactor_Header *pcHeader
            = (PX_FontModule_Charactor_Header *) (buffer + offset);
        offset += sizeof(PX_FontModule_Charactor_Header);
        pData = (buffer + offset);
        if (pcHeader->c_magic[0] != 'P')
            goto _ERROR;
        if (pcHeader->c_magic[1] != 'X')
            goto _ERROR;
        if (pcHeader->c_magic[2] != 'F')
            goto _ERROR;
        if (pcHeader->c_magic[3] != 'M')
            goto _ERROR;

        if (module->codePage == PX_FONTMODULE_CODEPAGE_UNDEFINED) {
            module->codePage = (PX_FONTMODULE_CODEPAGE) pcHeader->codePage;
        } else {
            if (pcHeader->codePage != (px_dword) module->codePage) {
                offset += pcHeader->Font_Width * pcHeader->Font_Height;
                continue;
            }
        }

        PX_Base64Encode((px_byte *) &pcHeader->charactor_code,
                        sizeof(pcHeader->charactor_code),
                        hex);

        // PX_itoa(pcHeader->charactor_code,hex,sizeof(hex),16);

        if (PX_MapGet(&module->characters_map, hex)) {
            offset += pcHeader->Font_Width * pcHeader->Font_Height;
            continue;
        }
        if ((px_int) pcHeader->BearingY > module->max_BearingY) {
            module->max_BearingY = pcHeader->BearingY;
        }
        if ((px_int) pcHeader->Font_Height > module->max_Height) {
            module->max_Height = pcHeader->Font_Height;
        }

        if ((px_int) pcHeader->Font_Width > module->max_Width) {
            module->max_Width = pcHeader->Font_Width;
        }
        cpy = (PX_FontModule_Charactor *) MP_Malloc(
            module->mp, sizeof(PX_FontModule_Charactor));

        if (!cpy) {
            goto _ERROR;
        }

        PX_memcpy(cpy, pcHeader, sizeof(PX_FontModule_Charactor_Header));

        if (!PX_ShapeCreate(module->mp,
                            &cpy->shape,
                            cpy->header.Font_Width,
                            cpy->header.Font_Height)) {
            goto _ERROR;
        }
        PX_memcpy(cpy->shape.alpha,
                  pData,
                  cpy->header.Font_Width * cpy->header.Font_Height);
        offset += cpy->header.Font_Width * cpy->header.Font_Height;
        PX_MapPut(&module->characters_map, hex, cpy);
    }
    return PX_TRUE;
_ERROR:
    // PX_FontModuleFree(module);
    return PX_FALSE;
}

px_void PX_FontModuleFree(PX_FontModule *module) {
    px_map_element *pme;
    pme = PX_MapFirst(&module->characters_map);
    while (pme) {
        PX_FontModule_Charactor *pmc = (PX_FontModule_Charactor *) pme->Ptr;
        PX_ShapeFree(&pmc->shape);
        MP_Free(module->mp, pmc);
        pme = PX_MapNext(&module->characters_map, pme);
    }
    PX_MapFree(&module->characters_map);
}

px_int PX_FontModuleGetCharacterDesc(PX_FontModule *module,
                                     const px_char *Text,
                                     px_dword *code,
                                     px_int *advance,
                                     px_int *height) {
    px_char hex[16];
    px_int f_size;
    PX_FontModule_Charactor *pChar = PX_NULL;

    if (!(f_size
          = PX_FontModuleGetCharacterCode(module->codePage, Text, code))) {
        return 0;
    }

    if (*code == ' ') {
        *advance = module->max_Width / 2;
        *height = module->max_Height;
        return f_size;
    }

    PX_Base64Encode((px_byte *) code, sizeof(px_dword), hex);

    pChar = (PX_FontModule_Charactor *) PX_MapGet(&module->characters_map, hex);
    if (pChar) {
        *advance = (px_int) pChar->header.Advance;
        *height = (px_int) pChar->header.Font_Height;
    } else {
        *advance = 0;
        *height = 0;
    }

    return f_size;
}

px_void PX_FontModuleTextGetRenderWidthHeight(PX_FontModule *module,
                                              const px_char *Text,
                                              px_int *width,
                                              px_int *height) {
    px_int dx = 0;
    px_int dy = 0;
    px_int max_width = 0, max_height = 0;
    const px_char *pTextPointer = Text;

    if (module == PX_NULL) {
        PX_FontTextGetRenderWidthHeight(pTextPointer, width, height);
        return;
    }

    while (PX_TRUE) {
        px_dword code = 0;
        px_char hex[16];
        px_int f_size;
        PX_FontModule_Charactor *pChar = PX_NULL;

        if (!(f_size = PX_FontModuleGetCharacterCode(
                  module->codePage, pTextPointer, &code))) {
            break;
        }
        pTextPointer += f_size;

        if (code == ' ') {
            dx += module->max_Width / 2;
        } else if (code == '\n') {
            dx = 0;
            dy += module->max_Height;
        } else {
            PX_Base64Encode((px_byte *) &code, sizeof(code), hex);

            pChar = (PX_FontModule_Charactor *) PX_MapGet(
                &module->characters_map, hex);
            if (pChar) {
                //
                dx += pChar->header.Advance;
            }
        }

        if (dx > max_width) {
            max_width = dx;
        }

        if (pChar) {
            if ((px_int) (dy + module->max_BearingY - pChar->header.BearingY
                          + pChar->shape.height)
                > max_height) {
                max_height = dy + module->max_BearingY - pChar->header.BearingY
                             + pChar->shape.height;
            }
        }
    }
    *width = max_width;
    *height = max_height;
}

px_int PX_FontModuleDrawCharacter(px_surface *psurface,
                                  PX_FontModule *mod,
                                  px_int x,
                                  px_int y,
                                  const px_dword code,
                                  px_color Color) {
    px_char hex[16];
    PX_FontModule_Charactor *pChar;
    PX_Base64Encode((px_byte *) &code, sizeof(code), hex);
    pChar = (PX_FontModule_Charactor *) PX_MapGet(&mod->characters_map, hex);
    if (pChar) {
        PX_ShapeRender(psurface,
                       &pChar->shape,
                       x + pChar->header.BearingX,
                       y + mod->max_BearingY - pChar->header.BearingY,
                       PX_ALIGN_LEFTTOP,
                       Color);
        return pChar->header.Advance;
    }

    return 0;
}

px_int PX_FontModuleDrawText(px_surface *psurface,
                             PX_FontModule *mod,
                             px_int x,
                             px_int y,
                             PX_ALIGN align,
                             const px_char *Text,
                             px_color Color) {
    px_int dx, dy, content_width = 0;
    px_int frWidth, frHeight;
    const px_char *pTextPointer = Text;

    if (!Text) {
        PX_ASSERT();
        return 0;
    }

    if (mod == PX_NULL) {
        return PX_FontDrawText(psurface, x, y, align, Text, Color);
    }

    PX_FontModuleTextGetRenderWidthHeight(mod, Text, &frWidth, &frHeight);

    switch (align) {
        case PX_ALIGN_LEFTTOP:
            break;
        case PX_ALIGN_MIDTOP:
            x -= frWidth / 2;
            break;
        case PX_ALIGN_RIGHTTOP:
            x -= frWidth;
            break;
        case PX_ALIGN_LEFTMID:
            y -= frHeight / 2;
            break;
        case PX_ALIGN_CENTER:
            y -= frHeight / 2;
            x -= frWidth / 2;
            break;
        case PX_ALIGN_RIGHTMID:
            y -= frHeight / 2;
            x -= frWidth;
            break;
        case PX_ALIGN_LEFTBOTTOM:
            y -= frHeight;
            break;
        case PX_ALIGN_MIDBOTTOM:
            y -= frHeight;
            x -= frWidth / 2;
            break;
        case PX_ALIGN_RIGHTBOTTOM:
            y -= frHeight;
            x -= frWidth;
            break;
    }

    dx = x;
    dy = y;
    pTextPointer = Text;

    while (PX_TRUE) {
        px_dword code = 0;
        px_char hex[16];
        px_int f_size;
        PX_FontModule_Charactor *pChar;

        if (!(f_size = PX_FontModuleGetCharacterCode(
                  mod->codePage, pTextPointer, &code))) {
            break;
        }
        pTextPointer += f_size;

        if (code == ' ') {
            dx += mod->max_Width / 2;
        } else if (code == '\n') {
            dy += mod->max_Height;
            dx = x;
        } else {
            PX_Base64Encode((px_byte *) &code, sizeof(code), hex);
            // PX_itoa(code,hex,sizeof(hex),16);
            pChar = (PX_FontModule_Charactor *) PX_MapGet(&mod->characters_map,
                                                          hex);
            if (pChar) {
                PX_ShapeRender(psurface,
                               &pChar->shape,
                               dx + pChar->header.BearingX,
                               dy + mod->max_BearingY - pChar->header.BearingY,
                               PX_ALIGN_LEFTTOP,
                               Color);
                dx += pChar->header.Advance;
            }
        }
    }
    return dx - x + 1;
}
