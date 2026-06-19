#include "SymbolInfo.h"
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int number_of_buckets;
    ifstream infile ("input.txt");

    if (!infile.is_open())
    {
        cerr << "Could not open input file";
        return EXIT_FAILURE;
    }

    infile >> number_of_buckets;
    SymbolTable symbolTableobject(number_of_buckets);

    char token;
    string extraWord;
    int i=0;
    while(infile >> token)
    {
        switch (token)
        {
        case 'I':
        {
            string symbolName, symbolType;
            infile >> symbolName;
            if(infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile<<"I ";
                outfile<<symbolName<<endl<<endl;
                outfile<<"	Wrong number of arugments for the command I"<<endl<<endl;
            }
            else
            {
                infile>>symbolType;
                if (infile.peek() == '\n' || infile.peek() == EOF)
                {

                    outfile<<"I "<<symbolName<<" "<<symbolType<<endl<<endl;
                    symbolTableobject.Insert(new SymbolInfo(symbolName, symbolType));
                }
                else
                {
                    infile>>extraWord;
                    outfile<<"I "<<symbolName<<" "<<symbolType<<" "<<extraWord<<endl<<endl;
                    outfile<<"	Wrong number of arugments for the command I"<<endl<<endl;
                }
            }
            i++;
            break;
        }
        case 'L':
        {
            string symbolName;
            infile >> symbolName;
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile<<"L "<<symbolName<<endl<<endl;
                symbolTableobject.Look_up(symbolName);
            }
            else
            {
                infile>>extraWord;
                outfile<<"L "<<symbolName<<" "<<extraWord<<endl<<endl;
                outfile<<"	Wrong number of arugments for the command L"<<endl<<endl;
            }
            i++;
            break;
        }
        case 'D':
        {
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile<<"D "<<endl<<endl;
                outfile<<"	Wrong number of arugments for the command D"<<endl<<endl;
            }
            else{
            string symbolName;
            infile >> symbolName;
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile<<"D "<<symbolName<<endl<<endl;
                symbolTableobject.Remove(symbolName);
            }
            else
            {
                infile>>extraWord;
                outfile<<"D "<<symbolName<<" "<<extraWord<<endl<<endl;
                outfile<<"	Wrong number of arugments for the command D"<<endl<<endl;
            }
            }
            i++;
            break;
        }
        case 'P':
        {
            string printtoken;
            infile >> printtoken;
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                if (printtoken == "A")
                {
                    outfile<<"P "<<printtoken<<endl<<endl;
                    outfile<<endl;
                    symbolTableobject.Print_All_ScopeTable();
                }
                else if(printtoken == "C")
                {
                    outfile<<"P "<<printtoken<<endl<<endl;
                    outfile<<endl;
                    symbolTableobject.Print_Current_Scopetable();
                }
                else
                {
                    outfile<<"P "<<printtoken<<endl<<endl;
                    outfile<<endl;

                    outfile<<"	Invalid argument for the command P"<<endl;
                }
            }
            else
            {
                infile>>extraWord;
                outfile<<"P "<<printtoken<<" "<<extraWord<<endl<<endl;

                outfile<<"	Invalid argument for the command P"<<endl;
            }
            i++;
            break;
        }
        case 'S':
        {
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile <<"S"<< endl<<endl ;
                symbolTableobject.Enter_Scope();
                outfile<<"New ScopeTable with id "<<symbolTableobject.current_scopetable->getID()<<" created"<<endl<<endl;
            }
            else
            {
                infile>>extraWord;
                outfile <<"S"<<" "<<extraWord<<endl<< endl ;
                outfile<<"	Wrong number of arugments for the command S"<<endl<<endl;
            }
            i++;

            break;
        }
        case 'E':
        {
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile<<"E"<<endl<<endl;
                outfile<<"ScopeTable with id "<<symbolTableobject.current_scopetable->getID()<<" removed"<<endl<<endl;
                symbolTableobject.Exit_Scope();
            }
            i++;
            break;
        }
        case 'Q':
        {
            if (infile.peek() == '\n' || infile.peek() == EOF)
            {
                outfile<<"Q"<<endl<<endl;
                symbolTableobject.all_scope_table_deleted();
            }
            else
            {
                infile>>extraWord;
                outfile<<"Q"<<" "<<extraWord<<endl<<endl;
                outfile<<"	Wrong number of arugments for the command P"<<endl<<endl;
            }
        }
        }
    }
        infile.close();
    }
