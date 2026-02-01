#include <iostream>
#include <unordered_map>
using namespace std;
int main() {
    int n;
    cout<<"Enter Number of Attendance: ";
    cin >> n;
    char arr[n];
    cout<<"Enter "<< n <<" Enteries P OR A : ";
    for (int i = 0; i < n; i++){
        cin >> arr[i];
    }  
    unordered_map<int, int> mp;
    int s = 0;
    int maxLen = 0;
    mp[0] = -1;
    for (int i = 0; i < n; i++) {
        if (arr[i] == 'P')
        {
            s += 1;
        }
        else
        {
            s -= 1;
        }
        if (mp.find(s) != mp.end()) {
            int len = i - mp[s];
            if (len > maxLen){
                maxLen = len;
            }
        } 
        else {
            mp[s] = i;
        }
    }
    cout << maxLen << endl;
    return 0;
}
