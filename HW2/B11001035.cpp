#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;

int var = 0;

vector<vector<string>> make_group(vector<string> vec)
{
    vector<vector<string>> group(var+1);
    vector<string> temp;
    for (int i=0;i<vec.size();i++)
    {
        int flag = 0;
        for (int j=0;j<vec[i].length();j++)
        {
            if (vec[i][j] == '1') flag++;
        }
        group[flag].push_back(vec[i]);
    }
    return group;
}

template<class T>
void findrepeat(vector<T> &vec)
{
    sort(vec.begin(), vec.end());
    for (int i=0;i<vec.size()-1;i++)
    {
        if (vec[i] == vec[i+1])
        {
            vec.erase(vec.begin()+i);
            i--;
        }
    }
    return;
}

int string_to_int(string s)
{
    int num = 0;
    for (int i=0;i<s.length();i++)
    {
        if (s[i] == '1')
        {
            num += pow(2, s.length()-i-1);
        }
        else if (s[i] == '-')
        {
            return -1;
        }
    }
    return num;
}

vector<string> get_all_expr(vector<string> vec)
{
    bool has_dash = false;
    vector<string> temp;
    for (int i=0;i<vec.size();i++)
    {
        for (int j=0;j<vec[i].length();j++)
        {
            if (vec[i][j] == '-')
            {
                vec[i][j] = '0';
                temp.push_back(vec[i]);
                vec[i][j] = '1';
                temp.push_back(vec[i]);
                has_dash = true;
                break;
            }
        }
    }
    if (has_dash) return get_all_expr(temp);
    else return vec;
}

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        ifstream fin(argv[1]);
        ofstream fout(argv[2]);

        string str = "";
        int out_var = 0;

        vector<string> varlab;
        string func_label = "";
        int statement_var = 0;
        vector<vector<string>> statement;
        vector<string> expr;

        vector<int> minterms;
        vector<string> minterms_str;
        vector<int> dontcare;
        vector<string> dontcare_str;

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
                    if (str[0] != '0' && str[0] != '1' && str[0] != '-')
                    {
                        cout << "Unexpected Error: Invalid input. " << "\"" << str << "\"" << endl;
                        exit(0);
                    }
                    string temp_char = "";
                    fin >> temp_char;
                    statement_var = 0;
                    while (str != ".e")
                    {
                        statement_var++;
                        string temp_str = str;
                        vector<string> temp_expr;
                        temp_expr.push_back(temp_str);
                        temp_expr = get_all_expr(temp_expr);
                        if (temp_char == "1")
                        {
                            for (int i=0;i<temp_expr.size();i++)
                            {
                                int tempint = string_to_int(temp_expr[i]);
                                minterms.push_back(tempint);
                                minterms_str.push_back(temp_expr[i]);
                            }
                        }
                        else
                        {
                            for (int i=0;i<temp_expr.size();i++)
                            {
                                int tempint = string_to_int(temp_expr[i]);
                                dontcare.push_back(tempint);
                                dontcare_str.push_back(temp_expr[i]);
                            }
                        }
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
                        fin >> temp_str >> temp_char;
                        vector<string> temp_expr;
                        temp_expr.push_back(temp_str);
                        temp_expr = get_all_expr(temp_expr);
                        if (temp_char == "1")
                        {
                            for (int i=0;i<temp_expr.size();i++)
                            {
                                int tempint = string_to_int(temp_expr[i]);
                                minterms.push_back(tempint);
                                minterms_str.push_back(temp_expr[i]);
                            }
                        }
                        else
                        {
                            for (int i=0;i<temp_expr.size();i++)
                            {
                                int tempint = string_to_int(temp_expr[i]);
                                dontcare.push_back(tempint);
                                dontcare_str.push_back(temp_expr[i]);
                            }
                        }
                    }
                    fin >> str;
                }
            }
        }
        findrepeat(minterms);
        findrepeat(dontcare);
        findrepeat(minterms_str);
        findrepeat(dontcare_str);
        cout << "minterms: ";
        for (int i=0;i<minterms.size();i++)
        {
            cout << minterms[i] << " ";
        }
        cout << endl;
        cout << "dontcare: ";
        for (int i=0;i<dontcare.size();i++)
        {
            cout << dontcare[i] << " ";
        }
        cout << endl;
        cout << "minterms: ";
        for (int i=0;i<minterms_str.size();i++)
        {
            cout << minterms_str[i] << " ";
        }
        cout << endl;
        cout << "dontcare: ";
        for (int i=0;i<dontcare_str.size();i++)
        {
            cout << dontcare_str[i] << " ";
        }
        cout << endl;
        vector<vector<string>> group;
        vector<string> cat;
        cat.insert(cat.end(), minterms_str.begin(), minterms_str.end());
        cat.insert(cat.end(), dontcare_str.begin(), dontcare_str.end());
        group = make_group(cat);
        for (int i=0;i<group.size();i++)
        {
            cout << "group " << i << ": ";
            for (int j=0;j<group[i].size();j++)
            {
                cout << group[i][j] << " ";
            }
            cout << endl;
        }
        vector<vector<string>> prime_implicant(var+1);
        int literal = var - 1;
        for (int i=0;i<group.size();i++)
        {
            for (int j=0;j<group[i].size();j++)
            {
                prime_implicant[literal+1].push_back(group[i][j]);
            }
        }
        while (literal >= 0)
        {
            bool has_change = false;
            for (int i=0;i<var;i++)
            {
                for (int j=0;j<group[i].size();j++)
                {
                    for (int k=0;k<group[i+1].size();k++)
                    {
                        int flag = 0;
                        int loc = 0;
                        for (int l=0;l<group[i][j].length();l++)
                        {
                            if (group[i][j][l] != group[i+1][k][l])
                            {
                                flag++;
                                loc = l;
                            }
                        }
                        if (flag == 1)
                        {
                            has_change = true;
                            string temp = group[i][j];
                            temp[loc] = '-';
                            prime_implicant[literal].push_back(temp);
                        }
                    }
                }
            }
            if (!has_change) break;
            group = make_group(prime_implicant[literal]);
            literal--;
        }
        findrepeat(prime_implicant[literal+1]);
        for (int i=var;i>=0;i--)
        {
            if (prime_implicant[i].empty()) break;
            cout << "prime_implicant " << i << ": ";
            for (int j=0;j<prime_implicant[i].size();j++)
            {
                cout << prime_implicant[i][j] << " ";
            }
            cout << endl;
        }
    }
    else
    {
        cout << "Invalid input." << endl;
        return 0;
    }
}
