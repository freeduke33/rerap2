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
#include <locale>
#include <codecvt>
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
Text::Text(char32_t pValue)
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
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> uconv;
	try {
		value = uconv.from_bytes(pValue);
		utf8value = pValue;
	} catch(const std::range_error& e) {
		utf8value = pValue.substr(0, uconv.converted());
		value = uconv.from_bytes(utf8value);
		std::cerr <<"UCS failed after producing " <<std::dec <<value.size() <<" characters" <<std::endl;
	}
}

/*** Set this text's value ***/
void Text::setValue(char32_t pValue)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> uconv;
	try {
		utf8value = uconv.to_bytes(pValue);
		value = std::u32string(1, pValue);
	} catch(const std::range_error& e) {
		utf8value.clear();
		value.clear();
		std::cerr <<"UCS failed producing single character" <<std::endl;
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
	return new Text(value.at(index));
}

/*** Destructor ***/
Text::~Text()
{
}
