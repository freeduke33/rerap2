// ReRap Version 0.9
// Copyright 2011 Matthew Mikolay.
//
// This file is part of ReRap.
//
// ReRap is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ReRap is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ReRap.  If not, see <http://www.gnu.org/licenses/>.

#include "text.h"
#include "../exceptions/invalidindex.h"
#include <cwchar>
#include <iostream>

/*** Constructor ***/
Text::Text()
{
	setValue("");
}

/*** Constructor ***/
Text::Text(Token tok)
{
	if(tok.getType() != T_STRING)
		throw InvalidInitException(getLineNumber(), getColumnNumber(), OBJ_TEXT);

	setValue(tok.getLexeme());
	setLineNumber(tok.getLineNumber());
	setColumnNumber(tok.getColumnNumber());
}

/*** Constructor ***/
Text::Text(std::string pValue)
{
	setValue(pValue);
}

/*** Constructor ***/
Text::Text(std::wstring pValue)
{
	setValue(pValue);
}

/*** Constructor ***/
Text::Text(wchar_t pValue)
{
	setValue(pValue);
}

/*** Get this object's type ***/
unsigned char Text::getType()
{
	return OBJ_TEXT;
}

/*** Get this text's length ***/
unsigned int Text::getLength()
{
	return value.length();
}

/*** Set this text's value ***/
void Text::setValue(std::string pValue)
{
	mbstate_t state;
	const char* buf = pValue.c_str();
	size_t len = mbsrtowcs(NULL, &buf, 0, &state);
	if (len > 0 && len != (size_t)-1) {
		buf = pValue.c_str();
		wchar_t* tmp = new wchar_t[len+1];
		mbsrtowcs(tmp, &buf, len, &state);
		tmp[len] = L'\0';
		value = std::wstring(tmp);
		utf8value = pValue;
		delete[] tmp;
	} else {
		value.clear();
		utf8value.clear();
		if (len < 0)
			std::cerr <<"UCS failed producing characters" <<std::endl;
	}
}

/*** Set this text's value ***/
void Text::setValue(std::wstring pValue)
{
	mbstate_t state;
	const wchar_t* buf = pValue.c_str();
	size_t len = wcsrtombs(NULL, &buf, 0, &state);
	if (len > 0 && len != (size_t)-1) {
		buf = pValue.c_str();
		char* tmp = new char[len+1];
		wcsrtombs(tmp, &buf, len, &state);
		tmp[len] = '\0';
		value = pValue;
		utf8value = std::string(tmp);
		delete[] tmp;
	} else {
		value.clear();
		utf8value.clear();
		if (len < 0)
			std::cerr <<"UTF8 failed producing characters" <<std::endl;
	}
}

/*** Set this text's value ***/
void Text::setValue(wchar_t pValue)
{
	mbstate_t state;
	char tmp[6 + 1]; // UTF8 max bytes + terminal symbol
	size_t len = wcrtomb(tmp, pValue, &state);
	if (len > 0 && len != (size_t)-1) {
		tmp[len] = '\0';
		utf8value = std::string(tmp);
		value = std::wstring(1, pValue);
	} else {
		value.clear();
		utf8value.clear();
		if (len < 0)
			std::cerr <<"UCS failed producing character" <<std::endl;
	}
}

/*** Get this text's value ***/
std::string Text::getValue()
{
	return utf8value;
}

/*** Evaluate this object ***/
Object* Text::evaluate()
{
	return clone();
}

/*** Get a character ***/
Text* Text::getChar(unsigned int index)
{
	return new Text(value.at(index - 1));
}

Text* Text::replace(unsigned int indexBeg, unsigned int indexEnd, Text* pReplacement)
{
	if(indexBeg > getLength() || indexBeg > indexEnd)
		throw InvalidIndexException(getLineNumber(), getColumnNumber(), indexBeg);
	if(indexEnd > getLength())
		throw InvalidIndexException(getLineNumber(), getColumnNumber(), indexEnd);

	std::wstring modified(value);
	modified.replace(indexBeg - 1, indexEnd - indexBeg + 1, pReplacement->value);

	return new Text(modified);
}

Text* Text::substr(unsigned int indexBeg, unsigned int indexEnd)
{
	if(indexBeg > indexEnd)
		return new Text();

	if(indexBeg > getLength())
		throw InvalidIndexException(getLineNumber(), getColumnNumber(), indexBeg);
	if(indexEnd > getLength())
		throw InvalidIndexException(getLineNumber(), getColumnNumber(), indexEnd);

	std::wstring modified(value.substr(indexBeg - 1, indexEnd - indexBeg + 1));

	return new Text(modified);
}

size_t Text::find(Text* text)
{
	return value.find(text->value);
}


/*** Destructor ***/
Text::~Text()
{
}
