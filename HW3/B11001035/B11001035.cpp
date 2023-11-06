#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
using namespace std;
string reset_state;

map<pair<string,string>,vector<string>> dot_table;
string outputdotname;

void output_dot(ofstream& foutdot, map<pair<string,string>,pair<string,string>> table, vector<string> states)
{
    foutdot << "digraph STG {" << endl;
    foutdot << "  " << "rankdir=LR;" << endl << endl;
    foutdot << "  " << "INIT [shape=point];" << endl;
    for (int i=0;i<states.size();i++)
    {
        foutdot << "  " << states[i] << " [label=\"" << states[i] << "\"];" << endl;
    }
    foutdot << endl;
    foutdot << "  " << "INIT -> " << reset_state << ";" << endl;
    for (map<pair<string,string>,vector<string>>::iterator it = dot_table.begin(); it != dot_table.end(); it++)
    {
        foutdot << "  " << it->first.first << " -> " << it->first.second << " [label=\"";
        for (int i=0;i<dot_table[pair(it->first.first,it->first.second)].size();i++)
        {
            foutdot << dot_table[pair(it->first.first,it->first.second)][i];
            if (i != dot_table[pair(it->first.first,it->first.second)].size()-1) foutdot << ",";
        }
        foutdot << "\"];" << endl;
    }
    foutdot << "}" << endl;

    outputdotname = outputdotname.substr(0,outputdotname.size()-4);
    string sys_message = "dot -T png " + outputdotname + ".dot > " + outputdotname + "dot.png";
    system(sys_message.c_str());
}

void output_kiss(ofstream& fout, map<pair<string,string>,pair<string,string>> table, vector<string> states)
{
    fout << ".start_kiss" << endl;
    fout << ".i " << table.begin()->first.second.size() << endl;
    fout << ".o " << table.begin()->second.second.size() << endl;
    fout << ".p " << table.size() << endl;
    fout << ".s " << states.size() << endl;
    fout << ".r " << reset_state << endl;
    for (map<pair<string,string>,pair<string,string>>::iterator it = table.begin(); it != table.end(); it++)
    {
        fout << it->first.second << " " << it->first.first << " " << it->second.first << " " << it->second.second << endl;
        dot_table[pair(it->first.first,it->second.first)].push_back(it->first.second + "/" + it->second.second);
    }
    fout << ".end_kiss" << endl;
}

int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        map<pair<string,string>,pair<string,string>> implication_table;

        ifstream fin(argv[1]);
        ofstream fout(argv[2]);
        ofstream foutdot(argv[3]);
        outputdotname = argv[3];


        int input_bit = 0;
        int output_bit = 0;

        //optional
        vector<string> states;
        vector<string> input_exp;
        int states_num = 0;
        int terms = 0;

        string str;

        fin >> str;
        while (str != ".end_kiss")
        {
            if (str == ".i")
            {
                fin >> input_bit;
                fin >> str;
            }
            else if (str == ".o") 
            {
                fin >> output_bit;
                fin >> str;
            }
            else if (str == ".p")
            {
                fin >> terms;
                fin >> str;
            }
            else if (str == ".s")
            {
                fin >> states_num;
                fin >> str;
            }
            else if (str == ".r")
            {
                fin >> reset_state;
                fin >> str;
            }
            else if (str == ".start_kiss")
            {
                fin >> str;
                continue;
            }
            else if (str[0] == '0' || str[0] == '1')
            {
                string ori_state;
                string next_state;
                string output;
                fin >> ori_state >> next_state >> output;

                implication_table[pair(ori_state,str)] = pair(next_state,output);

                if (find(states.begin(),states.end(),ori_state) == states.end()) states.push_back(ori_state);
                if (find(states.begin(),states.end(),next_state) == states.end()) states.push_back(next_state);

                if (find(input_exp.begin(),input_exp.end(),str) == input_exp.end()) input_exp.push_back(str);
                fin >> str;   
            }
            else
            {
                cout << "Unexpected input: " << str << endl;
                return 1;
            }
        }
        
        sort(states.begin(),states.end());
        vector<vector<string>> state_table(states.size()+1,vector<string>(states.size()+1,"-"));

        //remove incompatible states
        for (int i = 1; i < states.size(); i++)
        {
            for (int j = 0; j < states.size()-1; j++)
            {
                if (i <= j) break;
                int bool1 = 0;
                for (int k=0;k<input_exp.size();k++)
                {
                    bool1 += implication_table[pair(states[j],input_exp[k])].second != implication_table[pair(states[i],input_exp[k])].second;
                }
                if (bool1 >= 1)
                {
                    state_table[i][j] = "X";
                }
                else state_table[i][j] = "O";
            }
        }

        //remove redundant states
        bool has_redundant = true;
        while(has_redundant)
        {
            has_redundant = false;
            for (int i = 1; i < states.size(); i++)
            {
                for (int j = 0; j < states.size()-1; j++)
                {
                    if (i <= j) break;
                    for (int k=0;k<input_exp.size();k++)
                    {
                        int coory = find(states.begin(),states.end(),implication_table[pair(states[i],input_exp[k])].first) - states.begin();
                        int coorx = find(states.begin(),states.end(),implication_table[pair(states[j],input_exp[k])].first) - states.begin();
                        //cout << coory << " " << coorx << " ";
                        if ((state_table[coory][coorx] == "X" || state_table[coorx][coory] == "X") && state_table[i][j] != "X")
                        {
                            //cout << "<---redundant" << endl;
                            state_table[i][j] = "X";
                            has_redundant = true;
                            break;
                        }
                        //cout << endl;
                    }
                }
            }
        }


        //merge states
        for (int i = 1; i < states.size(); i++)
        {
            for (int j = 0;j < states.size()-1;j++)
            {
                if (i <= j) break;
                if (state_table[i][j] == "O")
                {
                    state_table[i][j] = "X";
                    state_table[j][i] = "X";
                    for (map<pair<string,string>,pair<string,string>>::iterator it = implication_table.begin(); it != implication_table.end(); it++)
                    {
                        if (it->first.first == states[i])
                        {
                            implication_table.erase(it--);
                        }
                        if (it->second.first == states[i])
                        {
                            it->second.first = states[j];
                        }
                    }
                }
            }
        }

        vector<string> new_states;
        for (map<pair<string,string>,pair<string,string>>::iterator it = implication_table.begin(); it != implication_table.end(); it++)
        {
            if (find(new_states.begin(),new_states.end(),it->first.first) == new_states.end()) new_states.push_back(it->first.first);
            if (find(new_states.begin(),new_states.end(),it->second.first) == new_states.end()) new_states.push_back(it->second.first);
        }
        sort(new_states.begin(),new_states.end());

        //output kiss file
        output_kiss(fout,implication_table,new_states);

        //output dot file
        output_dot(foutdot,implication_table,new_states);
    }
    else
    {
        cout << "Usage: ./B11001035 <input_file> <output_file> <output_dot_file>" << endl;
        return 0;
    }
}
