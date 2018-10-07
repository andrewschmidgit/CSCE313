#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "Tokenizer.h"

using namespace std;

struct Command
{
  public:
    string Name;
    vector<Token> Arguments;
};

class Shell
{
  private:
  public:
    void Execute(vector<Token> tokens, bool testing = false)
    {
        try
        {
            Command current;
            for (int i = 0; i < tokens.size(); i++)
            {
                Token token = tokens[i];
                switch (token.Type)
                {
                case TokenType::CommandName:
                {

                    if (token.Value == "cd")
                        chdir(tokens[i + 1].Value.c_str());
                    else
                        current.Name = token.Value;
                    break;
                }
                case TokenType::Argument:
                {
                    current.Arguments.push_back(token);
                    break;
                }
                case TokenType::Redirect:
                {

                    
                }
                case TokenType::Pipe:
                {
                    // Setting up input args to exec
                    // Name of command + arguments + terminate
                    const char **args = new const char *[current.Arguments.size() + 3];
                    args[0] = current.Name.c_str();

                    for (int i = 0; i < current.Arguments.size(); i++)
                    {
                        args[i + 1] = current.Arguments[i].Value.c_str();
                    }
                    args[current.Arguments.size() + 1] = NULL;
                    
                    // Actually executing the process
                    int fd[2];
                    pipe(fd);
                    pid_t p;
                    p = fork();
                    if(p < 0) cout << stderr << " it failed" << endl;
                    else if(p == 0) {
                        // Child
                        
                    }
                    else {
                        // Parent
                    }

                    // TEST: Output programName and args for testing
                    if (testing)
                    {
                        for (int i = 0; args[i] != NULL; i++)
                        {
                            cout << args[i] << " ";
                        }
                        cout << endl;
                    }
                    break;
                }
                }
            }
        }
        catch (const std::exception &e)
        {
            cout << e.what() << '\n';
            return;
        }
    }
};