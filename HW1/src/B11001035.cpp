#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;

void findRedundant(vector<pair<pair<int,string>,pair<int,int>>> &binary_tree)
{
    bool has_redundant = false;
    for (int i=1;i<binary_tree.size();i++)
    {
        if (binary_tree[i].second.first == binary_tree[i].second.second)
        {
            has_redundant = true;
            for (int j=1;j<i;j++)
            {
                if (binary_tree[j].second.first == binary_tree[i].first.first)
                {
                    binary_tree[j].second.first = binary_tree[i].second.first;
                }
                if (binary_tree[j].second.second == binary_tree[i].first.first)
                {
                    binary_tree[j].second.second = binary_tree[i].second.second;
                }
            }
            binary_tree.erase(binary_tree.begin()+i);
        }
        else
        {
            for (int m = i+1;m<binary_tree.size();m++)
            {
                if (binary_tree[i].first.second == binary_tree[m].first.second && binary_tree[i].second.first == binary_tree[m].second.first && binary_tree[i].second.second == binary_tree[m].second.second)
                {
                    has_redundant = true;
                    for (int j=1;j<i;j++)
                    {
                        if (binary_tree[j].second.first == binary_tree[m].first.first)
                        {
                            binary_tree[j].second.first = binary_tree[i].first.first;
                        }
                        if (binary_tree[j].second.second == binary_tree[m].first.first)
                        {
                            binary_tree[j].second.second = binary_tree[i].first.first;
                        }
                    }
                    binary_tree.erase(binary_tree.begin()+m);
                }
            }
        }
    }
    if (has_redundant)
    {
        return findRedundant(binary_tree);
    }
    return;
}

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        ifstream fin(argv[1]);
        ofstream fout(argv[2]);

        bool endoffile = false;
        string str = "";
        int var = 0;
        int out_var = 0;

        vector<string> varlab;
        string func_label = "";
        int statement_var = 0;
        vector<vector<string>> statement;
        vector<string> expr;

        fin >> str;
        while (str != ".e")
        {
            if (str == ".i")
            {
                fin >> var;
                fin >> str;
            }
            else if (str == ".o") 
            {
                fin >> out_var;
                fin >> str;
            }
            else if (str == ".ob")
            {
                fin >> func_label;
                fin >> str;
            }
            else if (str == ".ilb")
            {
                for (int i=0;i<var;i++)
                {
                    string temp;
                    fin >> temp;
                    varlab.push_back(temp);
                }
                fin >> str;
            }
            else
            {
                if (str != ".p")
                {
                    
                    string temp_char = "";
                    fin >> temp_char;
                    statement_var = 0;
                    while (str != ".e")
                    {
                        statement_var++;
                        vector<string> temp;
                        string tempexpr = "";
                        for (int p=0;p<varlab.size();p++) {
                            tempexpr += "2";
                        }
                        string temp_str = str;
                        for (int j=0;j<var;j++)
                        {
                            if (temp_str[j] != '-')
                            {
                                if (temp_str[j] == '0')
                                {
                                    temp.push_back(varlab[j]);
                                    tempexpr[j] = '0';
                                }
                                else
                                {
                                    temp.push_back(varlab[j]);
                                    tempexpr[j] = '1';
                                }
                                
                            }  
                        }
                        statement.push_back(temp);
                        expr.push_back(tempexpr);
                        fin >> str >> temp_char;
                    }
                }
                else
                {
                    fin >> statement_var;
                    for (int i=0;i<statement_var;i++)
                    {
                        vector<string> temp;
                        string temp_str = "";
                        string temp_char = "";
                        string tempexpr = "";
                        for (int p=0;p<varlab.size();p++) {
                            tempexpr += "2";
                        }
                        fin >> temp_str >> temp_char;
                        for (int j=0;j<var;j++)
                        {
                            if (temp_str[j] != '-')
                            {
                                if (temp_str[j] == '0')
                                {
                                    temp.push_back(varlab[j]);
                                    tempexpr[j] = '0';
                                }
                                else
                                {
                                    temp.push_back(varlab[j]);
                                    tempexpr[j] = '1';
                                }
                                
                            }   
                        }
                        statement.push_back(temp);
                        expr.push_back(tempexpr);
                    }
                    
                    fin >> str;
                }
            }
        }

        vector<pair<pair<int,string>,pair<int,int>>> binary_tree;
        binary_tree.push_back(make_pair(make_pair(0,"0"),make_pair(0,0)));
        vector<int> record_bool(pow(2,var+1),-1);
        for (int m=1;m<=pow(2,var)-1;m++)
        {
            int current_layer = floor(log2(m));
            binary_tree.push_back(make_pair(make_pair(m,varlab[current_layer]),make_pair(m*2,m*2+1)));
            record_bool[m*2] = 0;
            record_bool[m*2+1] = 1;
            if (current_layer == var-1)
            {
                vector<int> dyna_variable;
                dyna_variable.push_back(0);
                int upper_layer = m;
                while(upper_layer > 0) { 
                    dyna_variable.push_back(record_bool[upper_layer]);
                    upper_layer = floor(upper_layer / 2);
                }
                dyna_variable.pop_back();
                reverse(dyna_variable.begin(),dyna_variable.end());
                int res1 = 0;
                int v = 0;
                for (v;v<statement.size();v++)
                {
                    int current_res = 1;
                    int index = 0;
                    for (int k=0;k<statement[v].size();k++)
                    {
                        for (int z=0;z<varlab.size();z++)
                        {
                            if (statement[v][k] == varlab[z])
                            {
                                index = z;
                                break;
                            }
                        }
                        if (expr[v][index] == '0')
                        {
                            current_res = current_res & ~dyna_variable[index];
                        }
                        else if (expr[v][index] == '1')
                        {
                            current_res = current_res & dyna_variable[index];
                        }
                        else current_res = current_res;
                    }
                    if (v == 0) res1 = current_res;
                    else res1 = res1 | current_res;
                }
                res1 = (!res1) ? 0 : pow(2,var);
                dyna_variable.pop_back();
                dyna_variable.push_back(1);
                int res2 = 0;
                v = 0;
                for (v;v<statement.size();v++)
                {
                    int current_res = 1;
                    int index = 0;
                    for (int k=0;k<statement[v].size();k++)
                    {
                        for (int z=0;z<varlab.size();z++)
                        {
                            if (statement[v][k] == varlab[z])
                            {
                                index = z;
                                break;
                            }
                        }
                        if (expr[v][index] == '0')
                        {
                            current_res = current_res & ~dyna_variable[index];
                        }
                        else if (expr[v][index] == '1')
                        {
                            current_res = current_res & dyna_variable[index];
                        }
                        else current_res = current_res;
                    }
                    if (v == 0) res2 = current_res;
                    else res2 = res2 | current_res;
                }
                res2 = (!res2) ? 0 : pow(2,var);
                binary_tree[m].second.first = res1;
                binary_tree[m].second.second = res2;
            }
        }
        findRedundant(binary_tree);

        //start output
        fout << "digraph ROBDD {" << endl;
        for(int i=1;i<binary_tree.size();i++)
        {
            if (i == 1)
            {
                fout << "  {rank = same " << binary_tree[i].first.first;
            }
            else if (i != 1 && binary_tree[i].first.second != binary_tree[i-1].first.second)
            {
                fout << "}" << "\n";
                fout << "  {rank = same " << binary_tree[i].first.first;
            }
            else
            {
                fout << " " << binary_tree[i].first.first;
            }
        }
        if (binary_tree.size() != 1) fout << "}" << "\n" << "\n";

        fout << "  0 [label=0, shape=box]" << endl;
        for (int i=1;i<binary_tree.size();i++)
        {
            fout << "  " << binary_tree[i].first.first << " [label=\"" << binary_tree[i].first.second << "\"]" << endl;
        }
        fout << "  " << pow(2,var) << " [label=1, shape=box]" << endl;
        fout << "\n";

        for (int i=1;i<binary_tree.size();i++)
        {
            fout << "  " << binary_tree[i].first.first << " -> " << binary_tree[i].second.first << " [label=0, style=dotted]" << endl;
            fout << "  " << binary_tree[i].first.first << " -> " << binary_tree[i].second.second << " [label=1, style=solid]" << endl;
        }

        if (binary_tree.size() == 1)
        {
            if (statement.empty())
            {
                fout << "  0 -> " << pow(2,var) << " [label=0, style=dotted]" << endl;
            }
            else
            {
                fout << "  0 -> " << pow(2,var) << " [label=1, style=solid]" << endl;
            }
        }

        fout << "}" << endl;
        //end of output
        return 0;
    }
    else
    {
        cout << "Invalid input." << endl;
        return 0;
    }
}
