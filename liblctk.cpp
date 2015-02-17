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

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <random>
#include <functional>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include "liblctk.hpp"
using namespace std;

/*
 * Common
 */

void selectLanguage(string lc) {
	letterfreq = letterfreqs.at(lc);
	}

string cleanString(string *text) {
	string clean = "";
	for (char c : *text) {
		if(isalpha(c)) {
			clean += toupper(c);
			}
		}
	return clean;
	}

map<char, float> calcLetterFreq(string *text) {
	map<char, float> lfreq;
	for (char c : *text) {
		if (isalpha(c)) {
			lfreq[toupper(c)] += 1;
			}
		}
	short int lcount = text->length();
	for (auto pair : lfreq) {
		lfreq[pair.first] = pair.second/lcount;
		}
	return lfreq;
	}

float calcOffset(string *text) {
	map<char, float> lfreq = calcLetterFreq(text);
	int lcount = text->length();
	float offset = 0;
	for (auto pair : lfreq) {
		offset += fabs(letterfreq->at(pair.first) - pair.second);
		}
	offset /= lcount;
	return offset;
	}

string restorePunctuation(string *wopunc, string *wpunc, bool encrypting) {
	string text = "";
	int i = 0;
	for (char c : *wpunc) {
		if (isalpha(c)) {
			text += (islower(c)) ? tolower(wopunc->at(i)) : wopunc->at(i);
			i++;
			}
		else {
			text += c;
			}
		}
	// add encrypted X for uneven plaintexts
	if (i < wopunc->length()) {
		text += wopunc->at(i);
		}
	// remove trailing capital X, if decrypting
	if (!encrypting and text.at(text.length()-1) == 'X') {
		text.pop_back();
		}
	return text;
	}


/*
 * Caesar Cipher En-/Decryption
 * Version: 0.2
 */

string encipherCaesarString(string *plain, char key) {
	string cipher = "";
	char a = 'a';
	for (char c : *plain) {
		if (not isalpha(c)) {
			cipher += c;
			continue;
			}
		a = (isupper(c)) ? 'A' : 'a';
		c = (c - a + key) % 26 + a;
		cipher += c;
		}
	return cipher;
	}

string decipherCaesarString(string *cipher, char key) {
	string plain = "";
	char a = 'a';
	for (char c : *cipher) {
		if (not isalpha(c)) {
			plain += c;
			continue;
			}
		a = (isupper(c)) ? 'A' : 'a';
		c = (c - a + (26 - key)) % 26 + a;
		plain += c;
		}
	return plain;
	}


/*
 * Caesar Cipher Cracking
 * Version: 0.2
 */

string crackCaesarString(string *cipher) {
	string plain = "";
	char bestkey = 0;
	float offset = 0;
	float bestoffset = calcOffset(cipher);
	for (char key = 1; key < 26; key++) {
		plain = decipherCaesarString(cipher,key);
		offset = calcOffset(&plain);
		bestkey = (offset > bestoffset) ? key : bestkey;
		bestoffset = (offset > bestoffset) ? offset : bestoffset;
		}
	plain = decipherCaesarString(cipher,bestkey);
	return plain;
	}


/*
 * Vigenere Cipher En-/Decryption
 * Version: 0.2
 */

string encipherVigenereString(string *plain, string *key) {
	string cipher = "";
	*key = cleanString(key);
	char a = 'a';
	short int i = 0;
	short int keylen = key->length();
	for (char c : *plain) {
		if (not isalpha(c)) {
			cipher += c;
			continue;
			}
		a = (isupper(c)) ? 'A' : 'a';
		short int incr = isupper(key->at(i)) ? key->at(i) - 'A' : key->at(i) - 'a';
		c = (c - a + incr) % 26 + a;
		i = (i + 1) % keylen;
		cipher += c;
		}
	return cipher;
	}

string decipherVigenereString(string *cipher, string *key) {
	string plain = "";
	*key = cleanString(key);
	char a = 'a';
	short int i = 0;
	short int keylen = key->length();
	for (char c : *cipher) {
		if (not isalpha(c)) {
			plain += c;
			continue;
			}
		a = (isupper(c)) ? 'A' : 'a';
		short int incr = isupper(key->at(i)) ? key->at(i) - 'A' : key->at(i) - 'a';
		c = (c - a + (26 - incr)) % 26 + a;
		i = (i + 1) % keylen;
		plain += c;
		}
	return plain;
	}


/*
 * Vigenere Cipher Cracking
 * Version: 0.1
 */

float calcIC(string *cipher) {
	short int lcount = cipher->length();
	map<char, short int> counts;
	transform(cipher->begin(), cipher->end(), cipher->begin(), ::toupper);
	for (char c : *cipher) {
		counts[c] = counts[c] + 1;
		}
	float ic = 0;
	for (short int i = 'A'; i <= 'Z'; i++) {
		ic += counts[i] * (counts[i] - 1);
		}
	ic /= lcount * (lcount - 1);
	return ic;
	}

string crackVigenereString(string *cipher) {
	string plain = "";
	string key = "";
	string bestkey = "";
	string tidyciph = "";
	float offset = 0;
	float icoffset = 0;
	short keylen = 0;
	
	// tidy string for calculations
	for (char c : *cipher) {
		if (isalpha(c)) {
			tidyciph += c;
			}
		}
	
	float bestoffset = calcOffset(&tidyciph);
	
	// generate map of potential key-lengths
	map<short int, float> potkeys;
	for (short int i = 1, len = tidyciph.length(); i < len; i++) { // iterate key-lengths
		vector<float> icvect;
		for (short int j = 0; j < i; j++) { // iterate sub-strings
			string substr = "";
			for (short int k = 0, max = len/i; k < max; k++) { // create sub-string
				substr += tidyciph.at(j+i*k);
				}
			float ic = calcIC(&substr);
			icvect.push_back(ic);
			}
		// calculate average I.C. for key-lengths
		float icav = 0;
		for (float ic : icvect) {
			icav += ic;
			}
		icav /= i;
		potkeys[i] = icav;
		}
	
	// delete NaNs, calculate I.C. limit
	float limit = 0;
	for (auto itr = potkeys.begin(); itr != potkeys.end(); ++itr) {
			if (isnan(itr->second)) {
				potkeys.erase(itr->first);
				}
			else {
				limit += itr->second;
				}
			}
	limit /= potkeys.size();
	limit *= 1.3;
	
	// shrink map of potetial key lenghts
	for (auto itr = potkeys.begin(); itr != potkeys.end(); ++itr) {
		if (itr->second < limit) {
			potkeys.erase(itr->first);
			}
		}
	
	// find key length
	for (auto itr = potkeys.begin(); itr != potkeys.end(); ++itr) {
		// look ahead the next keys, if one is divisible by the current key
		// the current key is assumed to be the key length.
		if ((next(itr,1)->first%itr->first == 0) or (next(itr,2)->first%itr->first == 0) or (next(itr,3)->first%itr->first == 0)) {
			keylen = itr->first;
			break;
			}
		}
	
	// calculate key
	for (short int i = 0; i < keylen; i++) {
		string substr = "";
		// generate substring of every keylenth letter
		for (short int j = i; j < tidyciph.length(); j += keylen) {
			substr += tidyciph.at(j);
			}
		// crack substring using caesar *cipher cracking technique
		string plain = "";
		char bestkey = 0;
		float offset = 0;
		float bestoffset = calcOffset(cipher);
		for (char key = 1; key < 26; key++) {
			plain = decipherCaesarString(&substr,key);
			offset = calcOffset(&plain);
			bestkey = (offset > bestoffset) ? key : bestkey;
			bestoffset = (offset > bestoffset) ? offset : bestoffset;
			}
		key += 'A' + bestkey;
		}
	
	plain = decipherVigenereString(cipher, &key);
	return plain;
	}


/*
 * One-Time-Pad En-/Decryption
 * Version: 0.1
 */

string encipherOTPString(string *plain, int seed) {
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(1,25);
	auto random = ::bind(distribution, generator);
	string cipher = "";
	char a = 'a';
	for (char c : *plain) {
		if (not isalpha(c)) {
			cipher += c;
			continue;
			}
		a = (isupper(c)) ? 'A' : 'a';
		c = (c - a + random()) % 26 + a;
		cipher += c;
		}
	return cipher;
	}

string decipherOTPString(string *cipher, int seed) {
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(1,25);
	auto random = ::bind(distribution, generator);
	string plain = "";
	char a = 'a';
	for (char c : *cipher) {
		if (not isalpha(c)) {
			plain += c;
			continue;
			}
		a = (isupper(c)) ? 'A' : 'a';
		c = (c - a + (26 - random())) % 26 + a;
		plain += c;
		}
	return plain;
	}


/*
 * Playfair Cipher En-/Decryption
 * Version: 0.1
 */

short getIndexInKeySq(char c, char* keysq) {
	for (int i = 0; i < 25; i++) {
		if (keysq[i] == c) {
			return i;
			}
		}
	}

void calcIndex(coords *co) {
	co->index = co->row*5 + co->column;
	}

void calcCoords(coords *co) {
	co->row = (short) co->index/5;
	co->column = co->index % 5;
	}

char* genKeySquare(string *key) {
	transform(key->begin(), key->end(),key->begin(), ::toupper);
	replace(key->begin(), key->end(), 'J', 'I');
	char* keysq = (char*) malloc(sizeof(char)*26);
	string keystr;
	for (char c : *key) {
		if (isalpha(c)) {
			if (keystr.find(c) == string::npos) {
				keystr += c;
				}
			}
		}
	for (char c : alphasq) {
		if (keystr.find(c) == string::npos) {
			keystr += c;
			}
		}
	strcpy(keysq, keystr.c_str());
	keysq[25] = '\0';
	return keysq;
	}

string encipherPlayfairString(string *plain, char* keysq) {
	string cipher = "";
	string clean = "";
	
	// clean string
	clean = cleanString(plain);
	replace(clean.begin(), clean.end(), 'J', 'I');
	for (int i = 0, n = clean.length()-1; i < n; i+=2) {
		if (clean.at(i) == clean.at(i+1)) {
			clean.at(i+1) = 'X';
			}
		}
	if (clean.length()%2 != 0) {
		clean += 'X';
		}
	
	// encrypt string
	for (int i = 0, n = clean.length()-1; i < n; i+=2) {
		// calc index
		coords first;
		coords second;
		first.index = getIndexInKeySq(clean.at(i), keysq);
		second.index = getIndexInKeySq(clean.at(i+1), keysq);
		calcCoords(&first);
		calcCoords(&second);
		
		// switch row/column
		if ((first.row != second.row) and (first.column != second.column)) {
			short tmp = first.column;
			first.column = second.column;
			second.column = tmp;
			}
		else if (first.row == second.row) {
			first.column = (first.column+1)%5;
			second.column = (second.column+1)%5;
			}
		else {
			first.row = (first.row+1)%5;
			second.row = (second.row+1)%5;
			}
		
		// recalc index
		calcIndex(&first);
		calcIndex(&second);
		cipher += keysq[first.index];
		cipher += keysq[second.index];
		}
	cipher = restorePunctuation(&cipher, plain, true);
	return cipher;
	}

string decipherPlayfairString(string *cipher, char* keysq) {
	string plain = "";
	string clean = "";
	
	// clean string
	clean = cleanString(cipher);
	replace(clean.begin(), clean.end(), 'J', 'I');
	
	// decrypt string
	for (int i = 0, n = clean.length()-1; i < n; i+=2) {
		// calc index
		coords first;
		coords second;
		first.index = getIndexInKeySq(clean.at(i), keysq);
		second.index = getIndexInKeySq(clean.at(i+1), keysq);
		calcCoords(&first);
		calcCoords(&second);
		
		// switch row/column
		if ((first.row != second.row) and (first.column != second.column)) {
			short tmp = first.column;
			first.column = second.column;
			second.column = tmp;
			}
		else if (first.row == second.row) {
			first.column = (first.column+4)%5;
			second.column = (second.column+4)%5;
			}
		else {
			first.row = (first.row+4)%5;
			second.row = (second.row+4)%5;
			}
		
		// recalc index
		calcIndex(&first);
		calcIndex(&second);
		plain += keysq[first.index];
		plain += keysq[second.index];
		}
	plain = restorePunctuation(&plain, cipher, false);
	return plain;
	}
