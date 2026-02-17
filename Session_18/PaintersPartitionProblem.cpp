class Solution {
  public:
  
    bool possible(vector<int>& arr, int k, long long mid) {
        long long sum = 0;
        int painters = 1;
        
        for(int i = 0; i < arr.size(); i++) {
            if(sum + arr[i] <= mid) {
                sum += arr[i];
            } else {
                painters++;
                sum = arr[i];
            }
        }
        
        return painters <= k;
    }
  
    int minTime(vector<int>& arr, int k) {
        
        long long low = arr[0];
        long long high = 0;
        
        for(int i = 0; i < arr.size(); i++) {
            if(arr[i] > low)
                low = arr[i];
            high += arr[i];
        }
        
        long long ans = high;
        
        while(low <= high) {
            long long mid = (low + high) / 2;
            
            if(possible(arr, k, mid)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        
        return ans;
    }
};