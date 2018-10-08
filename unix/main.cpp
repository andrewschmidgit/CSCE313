#include "unistd.h"
#include <string>
#include <limits.h>
#include <pwd.h>
#include <iostream>
#include "Tokenizer.h"
#include "Shell.h"

using namespace std;

bool Prompt(bool isSilent) {
    char cwd[PATH_MAX];
    // if(!isSilent)
        cout << getpwuid(geteuid())->pw_name << ":" << getcwd(cwd, sizeof(cwd)) << "$ ";
    return true;
}

int main(int argc, char **argv) {
    int argDelimiter;
    bool testingFlag = false;
    while ((argDelimiter = getopt(argc, argv, "t")) != -1)
    {
        switch (argDelimiter)
        {
            case 't':
            {
                testingFlag = true;
                break;
            }
            case '?':
                exit(1);
                break;
            default:
                break;
        }
    }

    string input;
    char cwd[PATH_MAX];

    Shell shell;
    if(testingFlag)
        cout << endl << "------------------- Testing --------------------" << endl << endl;
    
    Prompt(testingFlag);
    while(getline(cin, input)) {
        vector<Token> tokens = Tokenizer::Tokenize(input, testingFlag);
        
        // Tokenizer testing
        if(testingFlag) {
            for(auto t: tokens)
                cout << "\'" << t.Value << "\' " << t.Type << ' ';
            cout << endl;
        }

        shell.Execute(tokens, testingFlag);
        Prompt(testingFlag);
    }
}