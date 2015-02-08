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

#include "lexer.h"

/*** Constructor ***/
Lexer::Lexer(char* filename)
{
	// Open the input file
	file.open(filename, std::ifstream::in);
	lineNum = 1;
	colNum = 0;
	oldLine = 1;
	oldCol = 0;
	comment = false;
}

/*** Determine if the file has been opened ***/
bool Lexer::isOpen()
{
	return file.is_open();
}

/*** Determine if the lexer has more tokens ***/
bool Lexer::hasMoreTokens()
{
	while(hasChars())
	{
		std::string next = peekChar();

		if(next == "\n" || !isWhitespace(next)) {
			return true;
			}
		getChar();
	}

	return false;
}

/*** Get the next token ***/
Token Lexer::getNextToken()
{
	// Create a token to return
	Token nextToken;
	nextToken.setType(T_UNKNOWN);

	// Set lastpos to the current position
	lastpos = file.tellg();
	oldLine = lineNum;
	oldCol = colNum;

	// Run through finite state machine checks
	checkEndOfLine(nextToken);

	checkString(nextToken);

	checkKeyword(nextToken, "and", T_AND);
	checkKeyword(nextToken, "и", T_AND);
	checkKeyword(nextToken, "call", T_CALL);
	checkKeyword(nextToken, "вызов", T_CALL);
	checkKeyword(nextToken, "case", T_CASE);
	checkKeyword(nextToken, "выбор", T_CASE);
	checkKeyword(nextToken, "do", T_DO);
	checkKeyword(nextToken, "цикл", T_DO);
	checkKeyword(nextToken, "else", T_ELSE);
	checkKeyword(nextToken, "иначе", T_ELSE);
	checkKeyword(nextToken, "end", T_END);
	checkKeyword(nextToken, "конец", T_END);
	checkKeyword(nextToken, "esac", T_ESAC);
	checkKeyword(nextToken, "квыбор", T_ESAC);
	checkKeyword(nextToken, "exit", T_EXIT);
	checkKeyword(nextToken, "выход", T_EXIT);
	checkKeyword(nextToken, "extern", T_EXTERN);
	checkKeyword(nextToken, "чужие", T_EXTERN);
	checkKeyword(nextToken, "fi", T_FI);
	checkKeyword(nextToken, "кесли", T_FI);
	checkKeyword(nextToken, "for", T_FOR);
	checkKeyword(nextToken, "для", T_FOR);
	checkKeyword(nextToken, "from", T_FROM);
	checkKeyword(nextToken, "от", T_FROM);
	checkKeyword(nextToken, "fun", T_FUN);
	checkKeyword(nextToken, "функ", T_FUN);
	checkKeyword(nextToken, "if", T_IF);
	checkKeyword(nextToken, "если", T_IF);
	checkKeyword(nextToken, "input", T_INPUT);
	checkKeyword(nextToken, "ввод", T_INPUT);
	checkKeyword(nextToken, "intern", T_INTERN);
	checkKeyword(nextToken, "свои", T_INTERN);
	checkKeyword(nextToken, "nlf", T_NLF);
	checkKeyword(nextToken, "бпс", T_NLF);
	checkKeyword(nextToken, "not", T_NOT);
	checkKeyword(nextToken, "не", T_NOT);
	checkKeyword(nextToken, "od", T_OD);
	checkKeyword(nextToken, "кц", T_OD);
	checkKeyword(nextToken, "or", T_OR);
	checkKeyword(nextToken, "или", T_OR);
	checkKeyword(nextToken, "output", T_OUTPUT);
	checkKeyword(nextToken, "вывод", T_OUTPUT);
	checkKeyword(nextToken, "proc", T_PROC);
	checkKeyword(nextToken, "проц", T_PROC);
	checkKeyword(nextToken, "repeat", T_REPEAT);
	checkKeyword(nextToken, "повтор", T_REPEAT);
	checkKeyword(nextToken, "return", T_RETURN);
	checkKeyword(nextToken, "возврат", T_RETURN);
	checkKeyword(nextToken, "step", T_STEP);
	checkKeyword(nextToken, "шаг", T_STEP);
	checkKeyword(nextToken, "text", T_TEXT);
	checkKeyword(nextToken, "текста", T_TEXT);
	checkKeyword(nextToken, "then", T_THEN);
	checkKeyword(nextToken, "то", T_THEN);
	checkKeyword(nextToken, "to", T_TO);
	checkKeyword(nextToken, "до", T_TO);
	checkKeyword(nextToken, "until", T_UNTIL);
	checkKeyword(nextToken, "по", T_UNTIL);
	checkKeyword(nextToken, "when", T_WHEN);
	checkKeyword(nextToken, "при", T_WHEN);
	checkKeyword(nextToken, "while", T_WHILE);
	checkKeyword(nextToken, "пока", T_WHILE);
	checkKeyword(nextToken, "все", T_G_END);

	checkKeyword(nextToken, "yes", T_YES);
	checkKeyword(nextToken, "да", T_YES);
	checkKeyword(nextToken, "no", T_NO);
	checkKeyword(nextToken, "нет", T_NO);
	checkKeyword(nextToken, "empty", T_EMPTY);
	checkKeyword(nextToken, "пусто", T_EMPTY);

	checkUnsignedReal(nextToken);
	checkUnsignedInteger(nextToken);
	checkIdentifier(nextToken);

	checkSymbol(nextToken, "(", T_LPAREN);
	checkSymbol(nextToken, ")", T_RPAREN);
	checkSymbol(nextToken, "[", T_LBRACKET);
	checkSymbol(nextToken, "]", T_RBRACKET);

	checkSymbol(nextToken, "<*", T_LARROW);
	checkSymbol(nextToken, "*>", T_RARROW);

	checkSymbol(nextToken, "+", T_PLUS);
	checkSymbol(nextToken, "-", T_MINUS);
	checkSymbol(nextToken, "**", T_EXPONENT);
	checkSymbol(nextToken, "*", T_MULTIPLY);

	checkSymbol(nextToken, "/=", T_SLASHEQ);
	checkSymbol(nextToken, "//", T_INTDIVIDE);
	checkSymbol(nextToken, "/%", T_REMAINDER);
	checkSymbol(nextToken, "/", T_DIVIDE);

	checkSymbol(nextToken, ":=", T_ASSIGN);
	checkSymbol(nextToken, ":", T_COLON);
	checkSymbol(nextToken, "#", T_HASH);
	checkSymbol(nextToken, ",", T_COMMA);
	checkSymbol(nextToken, ";", T_SEMICOLON);

	checkSymbol(nextToken, "=>", T_EQGREAT);
	checkSymbol(nextToken, ">=", T_GREATEQ);
	checkSymbol(nextToken, "<=", T_LESSEQ);
	checkSymbol(nextToken, ">", T_GREAT);
	checkSymbol(nextToken, "<", T_LESS);
	checkSymbol(nextToken, "=", T_EQUAL);

	if(nextToken.getType() == T_UNKNOWN)
	{
		// Collect the next few characters
		std::string errMessage = "\"";
		for(unsigned char i = 0; hasChars() && (i < 10); i++)
			errMessage.append(getChar());
		errMessage.append("...\"");

		throw LexerSyntaxException(oldLine, oldCol, errMessage);
	}

	return nextToken;
}

/*** Destructor ***/
Lexer::~Lexer()
{
	// Close the input file
	file.close();
}

/*** Return true if the file has more characters ***/
bool Lexer::hasChars()
{
	return (file.good() && file.peek() != EOF);
}

/*** Peek at the next character ***/
std::string Lexer::peekChar()
{
	unsigned int safeLine=lineNum,safeCol=colNum;
	std::streampos safePos = file.tellg();

	std::string ret = getChar();

	lineNum=safeLine;
	colNum=safeCol;
	file.seekg(safePos);
	if(file.eof()) file.clear(file.rdstate() & ~std::ifstream::eofbit); // clear EOF bit

	return ret;
}

/*** Get a character ***/
std::string Lexer::getChar()
{
	std::string ret;
	char result[4];
	int cnt;

	result[0] = file.get();

	if(static_cast<unsigned char>(result[0])<0x80) {
	    cnt = 1;
	    if(result[0] == '\\')
		comment = true;
	    else if(result[0] == '\n') {
		// Reset the line and column numbers
		lineNum++;
		colNum = 0;
		comment = false;
		}
	    }
	else if((static_cast<unsigned char>(result[0])>>5) == 0x06) cnt = 2;
	else if((static_cast<unsigned char>(result[0])>>4) == 0x0E) cnt = 3;
	else if((static_cast<unsigned char>(result[0])>>3) == 0x1E) cnt = 4;
	else cnt = 0;
	
	// Increment the column number
	colNum++;

	// If the lexer is currently processing
	// a comment, then return whitespace
	if(comment && cnt) {
	    result[0]=' ';
	    cnt=1;
	    }

	for(int i=1;i<cnt;i++) {
	    if(!file.good()) cnt=0;
	    result[i] = file.get();
	    }

	ret.assign(result,cnt);
	return ret;
}

/*** Fail a finite state machine token test ***/
void Lexer::fail()
{
	// Reset the position of the get pointer
	file.seekg(lastpos);
	if(file.eof()) file.clear(file.rdstate() & ~std::ifstream::eofbit); // clear EOF bit
	lineNum = oldLine;
	colNum = oldCol;
}

void Lexer::checkEndOfLine(Token &target)
{
	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(T_EOL);
	result.setLineNumber(lineNum);
	result.setLexeme("EOL");

	if(getChar() == "\n")
	{
		target = result;
		return;
	}

	fail();
}

void Lexer::checkString(Token &target)
{
	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(T_STRING);
	result.setLineNumber(lineNum);
	result.setColumnNumber(colNum);

	std::string lexeme = "";

	if(getChar() != "\"")
	{
		fail();
		return;
	}

	while(hasChars() && peekChar() != "\"")
	{
		std::string next = getChar();

		if(next == "\n")
		{
			fail();
			return;
		}

		lexeme.append(next);
	}

	if(getChar() != "\"")
	{
		fail();
		return;
	}

	result.setLexeme(lexeme);
	target = result;
}

void Lexer::checkIdentifier(Token &target)
{
	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(T_IDENTIFIER);
	result.setLineNumber(lineNum);
	result.setColumnNumber(colNum);

	std::string lexeme = "";

	if(!hasChars() || !isAlpha(peekChar()))
	{
		fail();
		return;
	}

	do
	{
		lexeme.append(tolower(getChar()));
	}
	while(hasChars() && (isAlpha(peekChar()) || isNumber(peekChar()) || peekChar() == "_"));

	result.setLexeme(lexeme);
	target = result;
}

void Lexer::checkKeyword(Token &target, std::string keyword, unsigned char type)
{
	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(type);
	result.setLexeme(keyword);
	result.setLineNumber(lineNum);
	result.setColumnNumber(colNum);

	std::string ch;
	for(unsigned int i = 0; i < keyword.length(); i+=ch.length())
	{
		// Get the next character from the file
		if(hasChars()) ch=getChar();
		if(!hasChars() || keyword.substr(i,ch.length()) != tolower(ch))
		{
			fail();
			return;
		}
	}

	// Confirm that the next character is whitespace or a special character
	if(hasChars() && ((!isWhitespace(peekChar()) && !isSpecial(peekChar())) || peekChar() == "_"))
	{
		fail();
		return;
	}

	target = result;
}

void Lexer::checkSymbol(Token &target, std::string keyword, unsigned char type)
{
	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(type);
	result.setLexeme(keyword);
	result.setLineNumber(lineNum);
	result.setColumnNumber(colNum);

	std::string ch;
	for(unsigned int i = 0; i < keyword.length(); i+=ch.length())
	{
		// Get the next character from the file
		if(hasChars()) ch=getChar();
		if(!hasChars() || keyword.substr(i,ch.length()) != ch)
		{
			fail();
			return;
		}
	}

	target = result;
}

void Lexer::checkUnsignedInteger(Token &target)
{
	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(T_UNSIGNEDINT);
	result.setLineNumber(lineNum);

	std::string lexeme = "";

	if(!hasChars() || !isNumber(peekChar()))
	{
		fail();
		return;
	}

	do
	{
		lexeme.append(getChar());
	}
	while(hasChars() && isNumber(peekChar()));

	result.setLexeme(lexeme);
	target = result;
}

void Lexer::checkUnsignedReal(Token &target)
{

	// If the token already has a type, skip the check
	if(target.getType() != T_UNKNOWN)
		return;

	// Create a token to return
	Token result;
	result.setType(T_UNSIGNEDREAL);
	result.setLineNumber(lineNum);

	std::string lexeme = "";

	unsigned char state = 0;
	while(hasChars())
	{
		switch(state)
		{
			case 0:
				if(isNumber(peekChar()))
				{
					lexeme.append(getChar());
					state = 1;
				}
				else
				{
					fail();
					return;
				}
				break;
			case 1:
				if(isNumber(peekChar()))
					lexeme.append(getChar());
				else if(peekChar() == ".")
				{
					lexeme.append(getChar());
					state = 2;
				}
				else
				{
					fail();
					return;
				}
				break;
			case 2:
				if(isNumber(peekChar()))
				{
					lexeme.append(getChar());
					state = 3;
				}
				else
				{
					fail();
					return;
				}
				break;
			case 3:
				if(isNumber(peekChar()))
					lexeme.append(getChar());
				else if(peekChar() == "e" || peekChar() == "E")
				{
					lexeme.append(getChar());
					state = 4;
				}
				else
				{
					result.setLexeme(lexeme);
					target = result;
					return;
				}
				break;
			case 4:
				if(peekChar() == "+" || peekChar() == "-")
				{
					lexeme.append(getChar());
					state = 5;
				}
				else if(isNumber(peekChar()))
				{
					lexeme.append(getChar());
					state = 6;
				}
				else
				{
					fail();
					return;
				}
				break;
			case 5:
				if(isNumber(peekChar()))
				{
					lexeme.append(getChar());
					state = 6;
				}
			case 6:
				if(isNumber(peekChar()))
					lexeme.append(getChar());
				else
				{
					result.setLexeme(lexeme);
					target = result;
					return;
				}
				break;
				
		}
	}

	fail();
}

bool Lexer::isWhitespace(std::string input)
{
	return (input == " " || input == "\t" || input == "\n" || input == "\r");
}

bool Lexer::isSpecial(std::string input)
{
	return (input == "." || input == "," || input == ":" || input == ";" || input == "(" || input == ")" || input == "[" || input == "]" || input == "<" || input == ">" || input == "_" || input == "+" || input == "-" || input == "*" || input == "/" || input == "%" || input == "#" || input == "=" || input == "\"");
}

bool Lexer::isAlpha(std::string input)
{
	bool ret = input.length()>0 && (
	    // english letters
	   (input[0] >= 'a' && input[0] <= 'z') || (input[0] >= 'A' && input[0] <= 'Z') ||
	    // russian letters
	   (static_cast<unsigned char>(input[0])==0xD0 &&
		((static_cast<unsigned char>(input[1])>=0x90 && static_cast<unsigned char>(input[1])<=0xBF) ||
		  static_cast<unsigned char>(input[1])==0x81)) ||
	   (static_cast<unsigned char>(input[0])==0xD1 &&
		((static_cast<unsigned char>(input[1])>=0x80 && static_cast<unsigned char>(input[1])<=0x8F) ||
		 static_cast<unsigned char>(input[1])==0x91))
	   );
	return ret;
}

bool Lexer::isNumber(std::string input)
{
	return (input[0] >= '0' && input[0] <= '9');
}

std::string Lexer::tolower(std::string input)
{
	std::string ret;
	
	if(input.length()>0) {
	    if(static_cast<unsigned char>(input[0]) < 0x80) ret.push_back(::tolower(input[0]));
	    else if(static_cast<unsigned char>(input[0])==0xD0 &&
		    static_cast<unsigned char>(input[1])>=0x90 && static_cast<unsigned char>(input[1])<=0x9F) {
		ret.push_back('\xD0');
		ret.push_back(static_cast<char>(static_cast<unsigned char>(input[1])+0x20));
	     }
	    else if(static_cast<unsigned char>(input[0])==0xD0 &&
		    static_cast<unsigned char>(input[1])>=0xA0 && static_cast<unsigned char>(input[1])<=0xAF) {
		ret.push_back('\xD1');
		ret.push_back(static_cast<char>(static_cast<unsigned char>(input[1])-0x20));
	     }
	    else if(static_cast<unsigned char>(input[0])==0xD0 &&
		    static_cast<unsigned char>(input[1])==0x81) {
		ret="ё";
	     }
	    else ret=input;
	}
	return ret;
}

