#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H
#include<bits/stdc++.h>
using namespace std;
ofstream outfile("output.txt");


class SymbolInfo
{
private:
    string Name;
    string Type;
    SymbolInfo *next;
    int index,position;
public:
    SymbolInfo()
    {
        next=NULL;
    }
    SymbolInfo(string name,string type)
    {
        Name=name;
        Type=type;
        next=NULL;
    }
    string getName()
    {
        return Name;
    }
    string getType()
    {
        return Type;
    }
    void setNext(SymbolInfo *Nextnew)
    {
        next=Nextnew;
    }
    SymbolInfo *getNext()
    {
        return next;
    }
    void setIndex(int Index)
    {
        index=Index;
    }
    void setPosition(int Position)
    {
        position=Position;
    }
    string getPosition()
    {
        return to_string(index) + ", " + to_string(position);
    }
    ~SymbolInfo()
    {

    }
};
class ScopeTable
{
private:
    SymbolInfo **hashtable;
    string ID;
    ScopeTable *parentScope;
    long long  total_buckets;
    int level_count;
public:
    ScopeTable(ScopeTable *pscope,int n)
    {
        parentScope=pscope;
        total_buckets=n;
        level_count=0;
        if(parentScope==NULL)
            ID="1";
        else
            ID=parentScope->ID + "." + to_string(parentScope->level_count);
        hashtable=new SymbolInfo*[total_buckets+1];
        for(int i=0; i<total_buckets+1; i++)
            hashtable[i]=NULL;
    }
    unsigned long sdbmhash(string key)
    {
        const char *str = key.c_str();
        auto uchrs = reinterpret_cast<unsigned char*>(const_cast<char*>(str));
        unsigned long hash = 0;
        int c;

        while (c = *uchrs++)
            hash += c;

        return hash%total_buckets;
    }


    const string getID()
    {
        return ID;
    }
    void setPscope(ScopeTable *pscope)
    {
        parentScope=pscope;
    }
    ScopeTable *getPscope()const
    {
        return parentScope;
    }
    void increaselevel()
    {
        level_count++;
    }
    int getLevelcount()
    {
        return level_count;
    }
    void Insert(SymbolInfo *s)
    {
        string symbolname=s->getName();
        int hashindex=sdbmhash(symbolname);
        int pos=0;
        if(hashtable[hashindex]==NULL)
        {
            hashtable[hashindex]=s;
        }
        else
        {
            pos++;
            SymbolInfo *current=hashtable[hashindex];
            while(current->getNext()!=NULL)
            {
                pos++;
                current=current->getNext();
            }
            current->setNext(s);
        }
        s->setIndex(hashindex);
        s->setPosition(pos);

        outfile << "Inserted in ScopeTable# " << getID() << " at position " << hashindex << ", " << pos << endl<<endl;

    }
    SymbolInfo *Look_up(string s)
    {
        SymbolInfo *current=hashtable[sdbmhash(s)];
        if(current==NULL)
            return NULL;
        int i=0;
        while(current!=NULL)
        {
            if(current->getName()==s)
            {
                current->setPosition(i);
                return current;
            }
            current=current->getNext();
            i++;
        }
        return NULL;
    }
    bool Delete(string s)
    {
        SymbolInfo *current=hashtable[sdbmhash(s)];
        if(Look_up(s)==NULL)
            return false;
        if (current->getName()==s&&current->getNext()==NULL)
        {
            delete current;
            hashtable[sdbmhash(s)] = NULL;
            return true;
        }
        SymbolInfo *parent=current;
        while(current->getName()!=s&&current->getNext() != NULL)
        {
            parent=current;
            current=current->getNext();
        }
        if (current->getName()==s&&current->getNext()!=NULL)
        {
            {
                hashtable[sdbmhash(s)]=current->getNext();
            }
            parent->setNext(current->getNext());
            current->setNext(NULL);
            delete current;
            return true;
        }
        else
        {
            parent->setNext(NULL);
            current->setNext(NULL);
            delete current;
            return true;
        }
        return false;

    }
    void Print()
    {
        outfile << "ScopeTable # " << getID() << endl;

        for (int i=0; i<total_buckets; i++)
        {
            outfile<<i<< " -->  ";

            SymbolInfo *current= hashtable[i];
            while (current != NULL)
            {
            if(current!=NULL)
                outfile << "< " << current->getName() << " : " << current->getType() << ">  ";
                current = current->getNext();
            }
             outfile << endl;
        }
    }
    ~ScopeTable()
    {
        for(int i=0; i<total_buckets; i++)
        {
            SymbolInfo *temp=hashtable[i];
            while(temp)
            {
                SymbolInfo *next=temp->getNext();
                delete temp;
                temp=next;
            }
        }
        delete []hashtable;
    }
};
class SymbolTable
{
private:
public:
    ScopeTable *current_scopetable;
    int table_count;
    SymbolTable(int tcount)
    {
        table_count = tcount;
        current_scopetable = new ScopeTable( NULL,table_count);
    }
    bool Empty()
    {
        if(!current_scopetable)
        {
            outfile << "NO CURRENT SCOPE" << endl<<endl;
            return true;
        }
        else return false;
    }
    void Enter_Scope()
    {
        current_scopetable->increaselevel();
        current_scopetable = new ScopeTable( current_scopetable,table_count);
    }

    void Exit_Scope()
    {
        if(!Empty())
        {
            if (current_scopetable->getPscope() == NULL)
            {
                outfile<<"ScopeTable# 1 cannot be removed"<<endl<<endl;
            }
            else
            {
                ScopeTable *parentScope = current_scopetable->getPscope();

                // outfile<<"ScopeTable# "<<current_scopetable->getID()<<" removed"<<endl<<endl;
                delete current_scopetable;
                current_scopetable = parentScope;
            }
        }
    }

    bool Insert(SymbolInfo *news)
    {
        if(current_scopetable == NULL)
        {
            current_scopetable = new ScopeTable( NULL,table_count);
        }
        if (current_scopetable->Look_up(news->getName()))
        {
            outfile << "<" << news->getName() << "," << news->getType() << ">" << " already exists in current ScopeTable" <<endl<<endl;
            delete news;
            return false;
        }

        current_scopetable->Insert(news);
        return true;
    }

    bool Remove(string s)
    {

        SymbolInfo *current = current_scopetable->Look_up(s);
        if (current_scopetable->Delete(s))
        {
            outfile<<"Found in ScopeTable# "<<current_scopetable->getID()<<" at position "<<current->getPosition()<<endl<<endl;
            outfile<<"Deleted Entry "<<current->getPosition()<<" from current ScopeTable"<<endl<<endl;
            return true;
        }

        ScopeTable *scope = current_scopetable;
        outfile<<"Not found"<<endl<<endl;
        outfile<<s<<" not found"<<endl<<endl;
        return false;
    }

    bool Look_up(string s)
    {
        ScopeTable *scope = current_scopetable;
        while (scope)
        {
            SymbolInfo *current = scope->Look_up(s);
            if (current != NULL)
            {
                outfile<< "Found in ScopeTable# " << scope->getID() << " at position " << current->getPosition() << endl<<endl;
                return true;
            }
            else scope = scope->getPscope();
        }
        outfile <<"Not found" << endl<<endl;
        return false;
    }

    void Print_Current_Scopetable()
    {
        if(!Empty())
            current_scopetable->Print();
        outfile<<endl;
    }

    void Print_All_ScopeTable()
    {
        ScopeTable *temp = current_scopetable;

        while (temp)
        {
            temp->Print();
            outfile<<endl;
            temp = temp->getPscope();
            if(temp) outfile<<endl;
        }
    }
    void all_scope_table_deleted()
    {
        while(current_scopetable->getPscope()!=NULL)
        {
            ScopeTable *parentScope = current_scopetable->getPscope();
            delete current_scopetable;
            current_scopetable = parentScope;
        }
    }
    ~SymbolTable()
    {
        if(current_scopetable->getPscope()!=NULL)
        {
            delete current_scopetable->getPscope();
        }
        delete current_scopetable;
    }
};

#endif
