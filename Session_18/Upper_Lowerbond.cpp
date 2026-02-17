#include <iostream>
using namespace std;
int main(){
    int arr[] = {1,2,2,2,4,5,7};
    int n = 7;
    int target = 3;
    int low = 0, high = n-1;
    int lower = -1, upper = -1;
    while(low <= high){
        int mid = (low + high) / 2;
        if(arr[mid] >= target){
            lower = mid;
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    low = 0;
    high = n-1;
    while(low <= high){
        int mid = (low + high) / 2;
        if(arr[mid] > target){
            upper = mid;
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    cout << "Lower Bound index: " << lower << endl;
    cout << "Upper Bound index: " << upper << endl;
    return 0;
}
