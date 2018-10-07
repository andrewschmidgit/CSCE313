#include "unistd.h"
#include <string>
#include <limits.h>
#include <iostream>
#include "Tokenizer.h"
#include "Shell.h"

using namespace std;

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

    while(printf("%s> ", getcwd(cwd, sizeof(cwd))) && getline(cin, input) && input != "exit") {
        vector<Token> tokens = Tokenizer::Tokenize(input);
        
        // Tokenizer testing
        if(testingFlag) {
            for(auto t: tokens)
                cout << "\'" << t.Value << "\' " << t.Type << ' ';
            cout << endl;
        }

        shell.Execute(tokens, testingFlag);
    }
}