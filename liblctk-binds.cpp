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
#include "liblctk.cpp"


string cleanStringX(string text) {
	return cleanString(&text);
	}
string restorePunctuationX(string wopunc, string wpunc, bool encrypting) {
	return restorePunctuation(&wopunc, &wpunc, encrypting);
	}
string encipherCaesarStringX(string plain, char key) {
	return encipherCaesarString(&plain, key);
	}
string decipherCaesarStringX(string cipher, char key) {
	return decipherCaesarString(&cipher, key);
	}
CaesarResult crackCaesarStringX(string cipher) {
	return crackCaesarString(&cipher);
	}
string encipherVigenereStringX(string plain, string key) {
	return encipherVigenereString(&plain, &key);
	}
string decipherVigenereStringX(string cipher, string key) {
	return decipherVigenereString(&cipher, &key);
	}
float calcICX(string cipher) {
	return calcIC(&cipher);
	}
VigenereResult crackVigenereStringX(string cipher) {
	return crackVigenereString(&cipher);
	}
string encipherOTPStringX(string plain, int seed) {
	return encipherOTPString(&plain, seed);
	}
string decipherOTPStringX(string cipher, int seed) {
	return decipherOTPString(&cipher, seed);
	}
string genKeySquareString(string key) {
	return string(genKeySquare(&key));
	}
string encipherPlayfairStringX(string plain, string keysq) {
	return encipherPlayfairString(&plain, (char*)keysq.c_str());
	}
string decipherPlayfairStringX(string cipher, string keysq) {
	return decipherPlayfairString(&cipher, (char*)keysq.c_str());
	}


#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
EMSCRIPTEN_BINDINGS(liblctk) {
	emscripten::class_<CaesarResult>("CaesarResult")
		.property("key", &CaesarResult::key)
		.property("text", &CaesarResult::text);
	emscripten::class_<VigenereResult>("VigenereResult")
		.property("key", &VigenereResult::key)
		.property("text", &VigenereResult::text);	
	emscripten::function("selectLanguage", &selectLanguage);
	emscripten::function("cleanString", &cleanStringX);
	emscripten::function("restorePunctuation", &restorePunctuationX);
	emscripten::function("encipherCaesarString", &encipherCaesarStringX);
	emscripten::function("decipherCaesarString", &decipherCaesarStringX);
	emscripten::function("crackCaesarString", &crackCaesarStringX);
	emscripten::function("encipherVigenereString", &encipherVigenereStringX);
	emscripten::function("decipherVigenereString", &decipherVigenereStringX);
	emscripten::function("calcIC", &calcICX);
	emscripten::function("crackVigenereString", &crackVigenereStringX);
	emscripten::function("encipherOTPString", &encipherOTPStringX);
	emscripten::function("decipherOTPString", &decipherOTPStringX);
	emscripten::function("genKeySquare", &genKeySquareString);
	emscripten::function("encipherPlayfairString", &encipherPlayfairStringX);
	emscripten::function("decipherPlayfairString", &decipherPlayfairStringX);
}
#endif


#ifdef __CHAISCRIPT__
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/dispatchkit/bootstrap.hpp>
using namespace chaiscript;
CHAISCRIPT_MODULE_EXPORT ModulePtr create_chaiscript_module_lctk() {
	ModulePtr mptr(new Module());
	mptr->add(user_type<CaesarResult>(), "CaesarResult");
	mptr->add(bootstrap::basic_constructors<CaesarResult>("CaesarResult"));
	mptr->add(fun(&CaesarResult::key), "key");
	mptr->add(fun(&CaesarResult::text), "text");
	mptr->add(user_type<VigenereResult>(), "VigenereResult");
	mptr->add(bootstrap::basic_constructors<VigenereResult>("VigenereResult"));
	mptr->add(fun(&VigenereResult::key), "key");
	mptr->add(fun(&VigenereResult::text), "text");
	mptr->add(fun(&selectLanguage), "selectLanguage");
	mptr->add(fun(&cleanStringX), "cleanString");
	mptr->add(fun(&restorePunctuationX), "restorePunctuation");
	mptr->add(fun(&encipherCaesarStringX), "encipherCaesarString");
	mptr->add(fun(&decipherCaesarStringX), "decipherCaesarString");
	mptr->add(fun(&crackCaesarStringX), "crackCaesarString");
	mptr->add(fun(&encipherVigenereStringX), "encipherVigenereString");
	mptr->add(fun(&decipherVigenereStringX), "decipherVigenereString");
	mptr->add(fun(&calcICX), "calcIC");
	mptr->add(fun(&crackVigenereStringX), "crackVigenereString");
	mptr->add(fun(&encipherOTPStringX), "encipherOTPString");
	mptr->add(fun(&decipherOTPStringX), "decipherOTPString");
	mptr->add(fun(&genKeySquareString), "genKeySquare");
	mptr->add(fun(&encipherPlayfairStringX), "encipherPlayfairString");
	mptr->add(fun(&decipherPlayfairStringX), "decipherPlayfairString");
	return mptr;
	}
#endif
