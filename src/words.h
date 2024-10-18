#include <Arduino.h>

#ifndef words_h
#define words_h

const int es[1] = {0};
const String esString = "S'";
const int ist[4] = {2,3,4,5};
const String istString = "ISCH";
const int fuenf[4] = {7,8,9,10};
const String fuenfString = "FÜNF";
const int zehn[4] = {18,19,20,21};
const String zehnString = "ZEAH";
const int zwanzig[7] = {11,12,13,14,15,16,17};
const String zwanzigString = "ZWANZIG";
const int vor[3] = {22,23,24};
const String vorString = "VOR";
const int nach[4] = {29,30,31,32};
const String nachString = "NOCH";
const int dreiviertel[11] = {33,34,35,36,37,38,39,40,41,42,43};
const String dreiviertelString = "DREIVIERTEL";
const int viertel[7] = {33,34,35,36,37,38,39};
const String viertelString = "VIERTEL";
const int halb[4] = {44,45,46,47};
const String halbString = "HALB";

const String zeitNameStrings[12] = {"ZWÖLFE","OINSE","ZWOIE","DREIE","VIERE","FÜNF","SECHSE","SIEBENE","ACHDE","NEUNE","ZEHNE","ELF"};
const int zeitNames[12][7] = {
    {93, 94, 95, 96, 97, 98, -1}, // 12
    {61, 62, 63, 64, 65, -1, -1}, // 01
    {88, 89, 90, 91, 92, -1, -1}, // 02
    {66, 67, 68, 69, 70, -1, -1}, // 03
    {72, 73, 74, 75, 76, -1, -1}, // 04
    {51, 52, 53, 54, -1, -1, -1}, // 05
    {99, 100,101,102,103,104,-1}, // 06
    {81, 82, 83, 84, 85, 86, 87}, // 07
    {55, 56, 57, 58, 59, -1, -1}, // 08
    {78, 79, 80, 81, 82, -1, -1}, // 09
    {105,106,107,108,109,-1, -1}, // 10
    {49, 50, 51, -1, -1, -1, -1}  // 11
};

#endif