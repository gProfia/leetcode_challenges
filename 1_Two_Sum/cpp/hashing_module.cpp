#include <iostream>
#include <vector>
#include <unordered_map>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {

       // MOD TESTING WITH HASHMAP
       std::unordered_multimap < int,int > mods(nums.size());
       auto mod_of_sub = [](int t, int m) {return (t - m >= 0 ? t - m : m -t);};
       auto mod = [](int x) {return (x >= 0? x : -x);};
       // [mod(value), {indexes}]
       mods.emplace(mod(nums[0]),0); // n = | t - m |        
        for(int i = 1; i< nums.size(); i++){                        
            
            int n = mod_of_sub(target, nums[i]);
            auto mod_key = mods.find(n);
            if(mod_key != mods.end()){ 
                if(nums[i] + nums[mod_key->second] == target)
                    return {i, mod_key->second};
            }
            mods.emplace(mod(nums[i]),i);
        }
        return {EXIT_FAILURE};
        
    }
};
int main(){
        
    std::vector<int> t1 = {3,2,95,4,-3};  // -1
    std::vector<int> res;
    Solution s;
    
    res =  s.twoSum(t1,92);    
    
    for(int i: res)
        std::cout << i << ' ';
    std::cout << '\n';
    return EXIT_SUCCESS;
}