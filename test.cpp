#include <string.h>
#include <cmath>
#include <iostream>
using namespace std;
int transitions(int token,int n) {
    if(token == 1) {
        return pow(2,n-1)*n;
    }
    else {
        return pow(1+token,n-1)*pow(n,2);
    }
}
int places(int token,int n) {
    return pow(1+token,n);
}
int main() {
    int t = 9;
    int u = 1;
    int v = 1;
    while (t>1) {
        t--;
        u=u+2;
        v=v+u;
    }
    cout << t << endl;
    cout << v;
}