class Solution {
  public:
    int aggressiveCows(vector<int> &stalls, int k) {
        sort(stalls.begin(), stalls.end());
        int left = 1;
        int right = stalls.back() - stalls.front();
        int ans = 0;
        while(left <= right)
        {
            int mid = left + (right - left) / 2;
            int count = 1;
            int lastPoss = stalls[0];
            for(int i = 1; i < stalls.size(); i++)
            {
                if(stalls[i] - lastPoss >= mid)
                {
                    count++;
                    lastPoss = stalls[i];
                }
            }
            if(count >= k)
            {
                ans = mid ;
                left = mid + 1;
                
            }
            else
            {
                right = mid - 1;
            }
        }
        return ans;  
    }
};