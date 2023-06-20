#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

const char del = ',';


//every cell in table
class cell{
public:
    bool is_counted;
    string s_meaning;
    int meaning;
};

//map for each cell in columns
map<string, map<int, cell> > card_csv;
vector<string> head;
vector<int> nums;


bool if_num(const string *str){
    for(int c = 0; c < str->size(); c++){
        if ((*str)[c] > '9' || (*str)[c] < '0')
            return false;
    }
    return true;
}

int str_to_num(string *str){
    int res = 0;
    for(int c = 0; c < str->size(); c++){
        if((*str)[c] != ' ')
            res = res*10 + (*str)[c] - '0';
    }
    return res;
}

void line_def(string const &str, vector<string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(del, end)) != string::npos)
    {
        end = str.find(del, start);
        out.push_back(str.substr(start, end - start));
    }
};

void adding_maps(string line){
    map<int, cell> column;
    line_def(line, head);
    for (int i = 0; i < head.size(); i++){
        card_csv[head[i]] = column;
    }
};

map <char, int> mapping;
const int PLUS = 1, MINUS = 2, MULT = 3, DEV = 4;

void remaking_cell(string column, int line){
    string string_find = card_csv[column][line].s_meaning;
    int res;
    string first_part;
    string second_part;
    int ind_of;
    for(int i = 0; i < string_find.size(); i++){

        if(string_find[i] == '+' || string_find[i] == '-' || string_find[i] == '*' || string_find[i] == '/'){
            ind_of = i;
            break;
            }
    }
    first_part = string_find.substr(0, ind_of);
    second_part = string_find.substr(ind_of + 1, string_find.size() - 1);


    if(if_num(&first_part) && if_num(&second_part)){
        switch (string_find[ind_of]){
                case PLUS:
                    res = str_to_num(&first_part) + str_to_num(&second_part);
                    break;
                case MINUS:
                    res = str_to_num(&first_part) - str_to_num(&second_part);
                    break;
                case MULT:
                    res = str_to_num(&first_part) * str_to_num(&second_part);
                    break;
                case DEV:
                    res = str_to_num(&first_part) / str_to_num(&second_part);
                    break;
        }
    }
    card_csv[column][line].s_meaning = res;
    card_csv[column][line].is_counted = true;
};

int main()
{
    //input
    ifstream fin("coma-separated_values.csv");
    string line;
    getline(fin, line);
    adding_maps(line);
    vector<string> vec;
    int now_line;
    cell new_cell;
    while(getline(fin, line)){
        vec.clear();
        line_def(line, vec);
        now_line = str_to_num(&vec[0]);
        nums.push_back(now_line);
        for(int i = 1; i < vec.size(); i++){
            new_cell.s_meaning = vec[i];
            if (if_num(&vec[i])){
                new_cell.is_counted = true;
                new_cell.meaning = str_to_num(&(new_cell.s_meaning));
            }else
                new_cell.is_counted = false;
            card_csv[head[i - 1]][now_line] = new_cell;
        }
    }

    //counting
    mapping['+'] = PLUS;
    mapping['-'] = MINUS;
    mapping['*'] = MULT;
    mapping['/'] = DEV;

    for(int i = 0; i < nums.size(); i++){
        for (int j = 0; j < head.size(); j++){
            if(!(card_csv[head[j]][nums[i]]).is_counted){
                remaking_cell(head[j], nums[i]);
            }
        }
    }

    //output
    for(int i = 0; i < head.size(); i++){
        cout << del << head[i];
    }
    cout << endl;
    for(int i = 0; i < nums.size(); i++){
        cout << nums[i];
        for (int j = 0; j < head.size(); j++){
            cout << del << (card_csv[head[j]][nums[i]]).s_meaning;
        }
        cout << endl;
    }

    return 0;
}
