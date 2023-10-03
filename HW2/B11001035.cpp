#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;

int var = 0;
vector<string> varlab;
string func_label = "";

void output_file(ofstream &fout, vector<string> vec)
{
    int lit = 0;
    fout << ".i " << var << endl;
    fout << ".o 1" << endl;
    fout << ".ilb ";
    for (int i=0;i<varlab.size();i++)
    {
        if (i != varlab.size()-1) fout << varlab[i] << " ";
        else fout << varlab[i] << endl;
    }
    fout << ".ob " << func_label << endl;
    fout << ".p " << vec.size() << endl;
    for (int i=0;i<vec.size();i++)
    {
        for (int j=0;j<vec[i].size();j++)
        {
            if (vec[i][j] == '1' || vec[i][j] == '0') lit++;
        }
        fout << vec[i] << " 1" << endl;
    }
    fout << ".e" << endl;
    cout << "Total number of terms: " << vec.size() << endl;
    cout << "Total number of literals: " << lit << endl;
    exit(0);
    return;
}

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
void findrepeat(T &vec)
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

vector<string> petrick_method(vector<vector<string>> &vec)
{
    if (vec.size() > 1)
    {
        vector<string> tempvec;
        for (int i=0;i<vec[0].size();i++)
        {
            for (int j=0;j<vec[1].size();j++)
            {
                string tempstr = "";
                tempstr += vec[0][i];
                tempstr += vec[1][j];
                tempvec.push_back(tempstr);
            }
        }
        findrepeat(tempvec);
        vec.erase(vec.begin()+1);
        vec[0] = tempvec;
        return petrick_method(vec);
    }
    else
    {
        return vec[0];
    }
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
        // cout << "minterms: ";
        // for (int i=0;i<minterms.size();i++)
        // {
        //     cout << minterms[i] << " ";
        // }
        // cout << endl;
        // cout << "dontcare: ";
        // for (int i=0;i<dontcare.size();i++)
        // {
        //     cout << dontcare[i] << " ";
        // }
        // cout << endl;
        // cout << "minterms: ";
        // for (int i=0;i<minterms_str.size();i++)
        // {
        //     cout << minterms_str[i] << " ";
        // }
        // cout << endl;
        // cout << "dontcare: ";
        // for (int i=0;i<dontcare_str.size();i++)
        // {
        //     cout << dontcare_str[i] << " ";
        // }
        // cout << endl;
        vector<vector<string>> group;
        vector<string> cat;
        cat.insert(cat.end(), minterms_str.begin(), minterms_str.end());
        cat.insert(cat.end(), dontcare_str.begin(), dontcare_str.end());
        group = make_group(cat);
        // for (int i=0;i<group.size();i++)
        // {
        //     cout << "group " << i << ": ";
        //     for (int j=0;j<group[i].size();j++)
        //     {
        //         cout << group[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        vector<vector<string>> prime_implicant(var+1);
        vector<vector<bool>> prime_implicant_bool(var+1);
        int literal = var - 1;
        for (int i=0;i<group.size();i++)
        {
            for (int j=0;j<group[i].size();j++)
            {
                prime_implicant[literal+1].push_back(group[i][j]);
                prime_implicant_bool[literal+1].push_back(false);
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
                            prime_implicant_bool[literal].push_back(false);
                            vector<string>::iterator it = find(prime_implicant[literal+1].begin(), prime_implicant[literal+1].end(), group[i][j]);
                            if (it != prime_implicant[literal+1].end())
                            {
                                prime_implicant_bool[literal+1][it-prime_implicant[literal+1].begin()] = true;
                            }
                            it = find(prime_implicant[literal+1].begin(), prime_implicant[literal+1].end(), group[i+1][k]);
                            if (it != prime_implicant[literal+1].end())
                            {
                                prime_implicant_bool[literal+1][it-prime_implicant[literal+1].begin()] = true;
                            }                        
                        }
                    }
                }
            }
            if (!has_change) break;
            findrepeat(prime_implicant[literal]);
            group = make_group(prime_implicant[literal]);
            literal--;
        }

        findrepeat(prime_implicant[literal+1]);
        vector<string> essential_prime_implicant;

        for (int i=var;i>=0;i--)
        {
            if (prime_implicant[i].empty()) break;
            //cout << "prime_implicant " << i << ": ";
            for (int j=0;j<prime_implicant[i].size();j++)
            {
                //cout << prime_implicant[i][j] << "(" << prime_implicant_bool[i][j] << ") ";
                if (prime_implicant_bool[i][j] == false)
                {
                    essential_prime_implicant.push_back(prime_implicant[i][j]);
                }
            }
            //cout << endl;
        }
        //find essential prime implicant
        
        vector<vector<string>> essential_prime_implicant_int(pow(2,var));
        vector<string> candidates;

        for (int i=0;i<essential_prime_implicant.size();i++)
        {
            vector<string> temp_expr;
            temp_expr.push_back(essential_prime_implicant[i]);
            temp_expr = get_all_expr(temp_expr);
            for (int j=0;j<temp_expr.size();j++)
            {
                int tempint = string_to_int(temp_expr[j]);
                essential_prime_implicant_int[tempint].push_back(essential_prime_implicant[i]);
            }
        }

        for (int i=0;i<essential_prime_implicant_int.size();i++)
        {
            if (essential_prime_implicant_int[i].empty() || find(minterms.begin(),minterms.end(),i) == minterms.end()) continue;
            if (essential_prime_implicant_int[i].size() == 1)
            {
                candidates.push_back(essential_prime_implicant_int[i][0]);
                for (int j=0;j<essential_prime_implicant_int.size();j++)
                {
                    if (!essential_prime_implicant_int[j].empty())
                    {
                        if (find(essential_prime_implicant_int[j].begin(), essential_prime_implicant_int[j].end(), essential_prime_implicant_int[i][0]) != essential_prime_implicant_int[j].end())
                        {
                            essential_prime_implicant_int[j].clear();
                            vector<int>::iterator it = find(minterms.begin(), minterms.end(), j);
                            if (it != minterms.end())
                            {
                                minterms.erase(it);
                            }
                        }
                    }
                }
                i = 0;
            }
        }

        // cout << "essential prime implicants: ";
        // for (int i=0;i<candidates.size();i++)
        // {
        //     cout << candidates[i] << " ";
        // }
        // cout << endl;

        //if all candidates are found, output the result, else, do petrick's method
        if (minterms.empty()) output_file(fout, candidates);

        //petrick's method
        vector<string> symbol;
        vector<string> left_candidates;
        vector<vector<string>> petrick(pow(2,var));
        for (int i=0;i<minterms.size();i++)
        {
            for (int j=0;j<essential_prime_implicant_int[minterms[i]].size();j++)
            {
                if (find(left_candidates.begin(), left_candidates.end(), essential_prime_implicant_int[minterms[i]][j]) == left_candidates.end())
                {
                    left_candidates.push_back(essential_prime_implicant_int[minterms[i]][j]);
                }
            }
        }

        for (int i=0;i<left_candidates.size();i++)
        {
            string temp = "";
            temp.push_back('A'+i);
            symbol.push_back(temp);
            //cout << symbol[i] << ": " << left_candidates[i] << endl;
        }

        for (int i=0;i<minterms.size();i++)
        {
            for (int j=0;j<essential_prime_implicant_int[minterms[i]].size();j++)
            {
                for (int k=0;k<left_candidates.size();k++)
                {
                    if (essential_prime_implicant_int[minterms[i]][j] == left_candidates[k] && find(petrick[minterms[i]].begin(), petrick[minterms[i]].end(), symbol[k]) == petrick[minterms[i]].end())
                    {
                        petrick[minterms[i]].push_back(symbol[k]);
                    }
                }
            }
        }

        // for (int i=0;i<petrick.size();i++)
        // {
        //     if (petrick[i].empty()) continue;
        //     for(int j=0;j<petrick[i].size();j++)
        //     {
        //         if (j != petrick[i].size()-1) cout << petrick[i][j] << "+";
        //         else cout << petrick[i][j];
        //     }
        //     cout << endl;
        // }

        vector<vector<string>> petrick_copy;
        for (int i=0;i<petrick.size();i++)
        {
            if (!petrick[i].empty())
            {
                petrick_copy.push_back(petrick[i]);
            }
        }

        vector<string> petrick_result = petrick_method(petrick_copy);
        for (int i=0;i<petrick_result.size();i++)
        {
            findrepeat(petrick_result[i]);
        }
        findrepeat(petrick_result);

        string result = "";
        //cout << "petrick result:" << endl;
        for (int i=0;i<petrick_result.size();i++)
        {
            //if (i != petrick_result.size()-1) cout << petrick_result[i] << "+";
            //else cout << petrick_result[i] << endl;
            if (result == "")
            {
                result = petrick_result[i];
            }
            else
            {
                if (petrick_result[i].length() < result.length())
                {
                    result = petrick_result[i];
                }
            }
        }
        for (int i=0;i<result.length();i++)
        {
            candidates.push_back(left_candidates[result[i]-'A']);
        }
        output_file(fout, candidates);
    }
    else
    {
        cout << "Invalid input." << endl;
        return 0;
    }
}
