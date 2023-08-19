#include<iostream>
#include<unordered_map>
#include<string>
#include<fstream>
using namespace std;
struct Student{
    string name;
    string address;
    int rollno;
};
class Db{
    private:
        unordered_map<int, Student>mp;
        string sp;
        fstream save_file;
        bool write(Student s){
            bool success  = (mp.emplace(s.rollno, s)).second;
            if(success){
                return true;
            }
            else{
                return false;
            }
        }
        bool update(Student s){
            if(mp.find(s.rollno) != mp.end()){
                mp.erase(s.rollno);
                bool success = (mp.emplace(s.rollno, s)).second;
                if(success){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        bool read(int rollno, Student &out){
            if(mp.find(rollno) != mp.end()){
                out = mp.at(rollno);
                return true;
            }
            else{
                return false;
            }
        }
        bool remove(int rollno){
            if(mp.find(rollno) != mp.end()){
                mp.erase(rollno);
                return true;
            }
            else{
                return false;
            }
        }
        void commit(){
            ofstream output;
            output.open(sp, ios::out | ios::trunc);
            for(auto it = mp.begin(); it != mp.end(); it++){
                output<<(it->second).rollno<<","<<(it->second).name<<","<<(it->second).address<<endl;
            }
            output.close();
        }
        public:
        void initialize(string save_path){
            ifstream save_file;
            save_file.open(save_path);
            sp = save_path;
            string s;
            getline(save_file, s);
            do{
                if(s.size() != 0){
                        int i=0;
                        string roll_no_s;
                        Student ss;
                        while(s[i] != ','){
                            roll_no_s = roll_no_s+s[i++];
                        }
                        i++;
                        while(s[i] != ','){
                            ss.name = ss.name+s[i++];
                        }
                        i++;
                        while(i < s.size()){
                            ss.address = ss.address+s[i++];
                        }
                        ss.rollno = stoi(roll_no_s);
                        write(ss);
                }
            }while(getline(save_file,s));
            save_file.close();
        }
        void exec(ifstream &inp){
            string ss;
            getline(inp, ss);
            do{
                if(ss.size()>0){
                    string query;
                    int i=0;
                    while(ss[i] != ','){
                        query = query+ss[i++];
                    }
                    i++;
                    if(query == "WRITE" || query == "UPDATE"){
                        string roll_no_s;
                        Student s;
                        while(ss[i] != ','){
                            roll_no_s = roll_no_s+ss[i++];
                        }
                        i++;
                        while(ss[i] != ','){
                            s.name = s.name+ss[i++];
                        }
                        i++;
                        while(i<ss.size()){
                            s.address = s.address+ss[i++];
                        }
                        s.rollno = stoi(roll_no_s);
                        if(query == "WRITE"){
                            bool success = write(s);
                            if(success){
                                cout<<"Query successfully executed!"<<endl;
                            }
                            else{
                                cout<<"Query unsuccessfull..."<<endl;
                            }
                        }
                        else{
                            bool success = update(s);
                            if(success){
                                cout<<"Query successfully executed!"<<endl;
                            }
                            else{
                                cout<<"Query unsuccessfull..."<<endl;
                            }
                        }
                    }
                    else if(query == "DELETE"){
                        string num;
                        while(i<ss.size()){
                            num = num+ss[i++];
                        }
                        bool success = remove(stoi(num));
                        if(success){
                            cout<<"Query successfully executed!"<<endl;
                        }
                        else{
                            cout<<"Query unsuccessfull..."<<endl;
                        }
                    }
                    else if(query == "READ"){
                        string rd;
                        while(i<ss.size()){
                            rd = rd+ss[i++];
                        }
                        if(rd == "ALL"){
                            for(auto it = mp.begin(); it != mp.end(); it++){
                                cout<<(it->second).rollno<<" "<<(it->second).name<<" "<<(it->second).address<<endl;
                            }
                            cout<<"Query successfully executed!"<<endl;
                        }
                        else{
                            Student s;
                            bool success = read(stoi(rd), s);
                            if(success){
                                cout<<s.rollno<<" "<<s.name<<" "<<s.address<<endl;
                                cout<<"Query successfully executed!"<<endl;
                            }
                            else{
                                cout<<"Query unsuccessfull..."<<endl;
                            }
                        }
                    }
                    else{
                        cout<<"Please give a valid query!"<<endl;
                    }
                }
            }while(getline(inp, ss));
            commit();
        }
};

int main(){

    Db a;
    a.initialize("save_file.txt");
    ifstream inp;
    inp.open("input.txt");
    a.exec(inp);
    inp.close();
    return 0;
}