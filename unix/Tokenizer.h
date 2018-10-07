#pragma once

#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <algorithm>

using namespace std;

enum TokenType {
    Pipe,
    Redirect,
    Background,
    CommandName,
    Argument
};

struct Token {
    TokenType Type;
    string Value;
    Token(string value): Value(value) {}
};

class Tokenizer
{
  public:
    static vector<Token> Tokenize(const string &s) {
        vector<Token> ret;
        stringstream ss(s);
        string token;

        regex pipe("^(\\|)$");
        regex redirect("^(<|>)$");
        regex background("^(&)$");
        regex isString("(\".+)|(.+\")");

        TokenType lastToken;
        bool openString = false;
        string str = "";
        while(ss >> token) {
            Token t(token);
            if(regex_match(t.Value, pipe)) t.Type = TokenType::Pipe;
            else if(regex_match(t.Value, redirect)) t.Type = TokenType::Redirect;
            else if(regex_match(t.Value, background)) t.Type = TokenType::Background;
            else if(lastToken == TokenType::CommandName) t.Type = TokenType::Argument;
            else t.Type = TokenType::CommandName;

            bool isEndOfString = regex_match(t.Value, isString);

            if(openString || isEndOfString) {
                str += t.Value;
                if(isEndOfString) openString = !openString;

                if(openString) {
                    str += " ";
                    continue;
                }
                else {
                    str.erase(remove(str.begin(), str.end(), '\"'), str.end());
                    t.Value = str;
                    str = "";
                }
            }

            lastToken = t.Type;
            ret.push_back(t);
        }

        return ret;
    }
};