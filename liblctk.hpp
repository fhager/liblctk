/*
 * Author: Florian Hager
 * Version: 0.1
 * License: BSD 2-Clause
 * 
 * Copyright (c) 2014, Florian Hager
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <string>
#include <utility>
#include <map>
using namespace std;


map<char, float> letterfreq_en = {
	{'E',12.702},{'T',9.056},
	{'A',8.167}, {'O',7.507},
	{'I',6.996}, {'N',6.749},
	{'S',6.327}, {'R',5.987},
	{'H',6.094}, {'D',4.253},
	{'L',4.025}, {'U',2.758},
	{'C',2.782}, {'M',2.406},
	{'F',2.228}, {'Y',1.974},
	{'W',2.360}, {'G',2.015},
	{'P',1.929}, {'B',1.492},
	{'V',0.978}, {'K',0.772},
	{'X',0.150}, {'Q',0.095},
	{'J',0.153}, {'Z',0.074}};

map<char, float> letterfreq_de = {
	{'E',16.693},{'N',9.905},
	{'I',7.812}, {'S',6.765},
	{'R',6.539}, {'A',6.506},
	{'T',6.742}, {'D',5.414},
	{'H',4.064}, {'U',3.703},
	{'L',2.825}, {'C',2.837},
	{'G',3.647}, {'M',3.005},
	{'O',2.285}, {'B',2.566},
	{'W',1.396}, {'F',2.044},
	{'K',1.879}, {'Z',1.002},
	{'P',0.944}, {'V',1.069},
	{'J',0.191}, {'Y',0.032},
	{'X',0.022}, {'Q',0.055}};

map<string, map<char, float>*> letterfreqs = {
	{"en", &letterfreq_en},
	{"de", &letterfreq_de}};
map<char, float> *letterfreq = &letterfreq_en;

char alphasq[25] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

struct CaesarResult {
	int key;
	string text;
	};

struct VigenereResult {
	string key;
	string text;
	};

struct coords {
	short row;
	short column;
	short index;
	};

string cleanString(string *text);
map<char, float> calcLetterFreq(string *text);
float calcOffset(string *text);
string restorePunctuation(string *wopunc, string *wpunc, bool encrypting);

string encipherCaesarString(string *plain, char key);
string decipherCaesarString(string *cipher, char key);

CaesarResult crackCaesarString(string *cipher);

string encipherVigenereString(string *plain, string *key);
string decipherVigenereString(string *cipher, string *key);

float calcIC(string *cipher);
VigenereResult crackVigenereString(string *cipher);

string encipherOTPString(string *plain, int seed);
string decipherOTPString(string *cipher, int seed);

char* genKeySquare(string *key);
string encipherPlayfairString(string *plain, char* keysq);
string decipherPlayfairString(string *cipher, char* keysq);
