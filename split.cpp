#include <iostream>
#include <string>
using namespace std;
int numberOfWords(string input,char del = ' ') {
    int count = 0;
    int n = input.length();
    for(int i = 0 ; i < n ; i++) {
        if(input[i] == del) count++;
    }
    return count+1;
}
string* tokenize(string input, string del = " ") {
    char s = del[0];
    int numberOfWord = numberOfWords(input,s);
    int i=0;
    string *cmd = new string[numberOfWord];
    int start = 0;
    int end = (int)input.find(del); 
    while (end != -1) {
        cmd[i] = input.substr(start, end - start);
        start = (int)end + (int)del.size();
        end = (int)input.find(del, start);
        i++;
    }
    cmd[i] = input.substr(start, end - start);
    return cmd;
}

int main() {
    string line = "|ab,a,sd|afas|as,n,n,n";
    while(true) {
        if(line.length() == 0) break;
        int i = 0;
        for(i = line.length(); i >= 0; i--) {
            if(line[i] == '|') break;
        }
        string temp = line.substr(i+1);
        int sizeBeRemoved = numberOfWords(temp,',');
        string* beRemoved = tokenize(temp,",");
        for(int j = 0 ; j < sizeBeRemoved ; j++) {
            cout << beRemoved[j] <<  " ";
        }
        cout << endl;
        line = line.substr(0,i);
        cout << line;
        cout << endl;
    }
}