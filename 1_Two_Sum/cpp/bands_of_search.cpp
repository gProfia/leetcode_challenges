#include <iostream>
#include <vector>
#include <unordered_map>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
       
        // EXCLUDING BANDS OF THE SEARCH AREA
       //        -A        B          B1           A 
        // |<----------||-------> <--------||----------->|
        // |-----------|---------|----------@------------|-------------|
        // MIN         Z         MT         T         NMAX         MAX

        // |<----------||-------> <--------||----------->|
        // |-----------@---------|----------|------------|-------------|
        // MIN         T         MT         Z         NMAX         MAX

        //             |<--------||--------> <----------||------------>|
        // |-----------|---------|----------|------------@-------------|
        // MIN         NMIN      Z         MT           T            MAX

        //             |<--------||--------> <----------||------------>|        
        // |-----------|---------@----------|------------|-------------|
        // MIN         NMIN      T         MT            Z           MAX

        bool not_found = true, mt_even=false, negative_target=false;
        int min_amp_viable_sum = nums[0], max_amp_viable_sum = nums[0], zero = 0, nmax, nmin, MT=target/2, T=target;
        
        //std::vector < int > index_not_excluded_nums(nums.size(), 0); // [00     0100   001   0000] 1=flagd
        //std::vector < int > index_bands(nums.size(), 0); //         [min-z; z-mt; mt-t; t-max] 
                                                         //          len1   len2  len3  len4
                                                         // CPU INTESIVE
        
       int len_1 = 0, len_2 = 0, len_3 = 0, len_4 = 0;
       std::vector < int > i_band_1(nums.size(), 0);  //  T+        [min-z] len 1 
       std::vector < int > i_band_2(nums.size(), 0);  //            [z-mt]  len 2
       std::vector < int > i_band_3(nums.size(), 0);  //            [mt-t]  len 3
       std::vector < int > i_band_4(nums.size(), 0);  //            [t-max] len 4
                                                         // MEM INTENSIVE
        // find min e max values
        for(int i=0; i < nums.size(); i++)
        {
            if(nums[i] < min_amp_viable_sum)
                min_amp_viable_sum = nums[i];
            if(nums[i] > max_amp_viable_sum)
                max_amp_viable_sum = nums[i];
        }    
        nmax = (T < 0 && min_amp_viable_sum < 0 ? -T - min_amp_viable_sum :T - min_amp_viable_sum);
        nmin = T - max_amp_viable_sum;

        // find divisive parameters
        if (T % 2 == 0){
            mt_even = true;
        }
        // case T- 
        if(T < 0){
            T = zero;
            zero = target;

            // fill the bands and exclude non usable band
            bool one_half = false;
            int mid1;
            for (int i=0; i < nums.size(); i++){     

                if(nmin <= nums[i] && nums[i] < zero){                
                    i_band_1[len_1] = i;
                    len_1++;
                    continue;
                }
                if(zero <= nums[i] && nums[i] < MT){ 
                    if(mt_even && nums[i] == MT){ 
                        if(one_half)
                            return {mid1, i};
                        one_half=true;
                        mid1 = i;
                    }
                    i_band_2[len_2] = i;
                    len_2++;
                    continue;
                }
                if(MT <= nums[i] && nums[i] <= T){
                    i_band_3[len_3] = i;
                    len_3++;
                    continue;
                }
                if(T < nums[i] && nums[i] <= nmax){
                    i_band_4[len_4] = i;
                    len_4++;
                    continue;
                }
            }
        }else{            
            // fill the bands and exclude non usable band
            bool one_half = false;
            int mid1;
            for (int i=0; i < nums.size(); i++){     

                if(nmin <= nums[i] && nums[i] < zero){                
                    i_band_1[len_1] = i;
                    len_1++;
                    continue;
                }
                if(zero <= nums[i] && nums[i] <= MT){ 
                    if(mt_even && nums[i] == MT){ 
                        if(one_half)
                            return {mid1, i};
                        one_half=true;
                        mid1 = i;
                    }
                    i_band_2[len_2] = i;
                    len_2++;
                    continue;
                }
                if(MT < nums[i] && nums[i] <= T){
                    i_band_3[len_3] = i;
                    len_3++;
                    continue;
                }
                if(T < nums[i] && nums[i] <= nmax){
                    i_band_4[len_4] = i;
                    len_4++;
                    continue;
                }
            }
        }

        auto mod_of_sub = [](int t, int m) {return (t - m >= 0 ? t - m : m -t);};
        auto mod = [](int x) {return (x >= 0? x : -x);};
        
        if(len_1 && len_4){ // 1st and 4rt band are not empty
            int il = (len_1 < len_4? len_1 : len_4),
                 sl = (len_1 < len_4? len_4 : len_1);

            std::vector< int > minor_band = (len_1 < len_4? i_band_1: i_band_4);
            std::vector< int > larger_band = (len_1 < len_4? i_band_4: i_band_1);

            std::unordered_multimap < int,int > mods(il);

            // [mod(value), {indexes}]
            for(int i = 0; i < il; i++){
                mods.emplace(mod(nums[minor_band[i]]), minor_band[i]);
            }

            for(int i = 0; i < sl; i++){
                auto mod_key = mods.find(mod_of_sub(target, nums[larger_band[i]]));
                if(mod_key != mods.end())
                    return {larger_band[i], mod_key->second};
            }            
        }
        if(len_2 && len_3){                
            int il = (len_2 < len_3? len_2 : len_3),
                 sl = (len_2 < len_3? len_3 : len_2);

            std::vector< int > minor_band = (len_2 < len_3? i_band_2: i_band_3);
            std::vector< int > larger_band = (len_2 < len_3? i_band_3: i_band_2);

            std::unordered_multimap < int,int > mods(il);

            // [mod(value), {indexes}]
            for(int i = 0; i < il; i++){
                mods.emplace(mod(nums[minor_band[i]]), minor_band[i]);
            }

            for(int i = 0; i < sl; i++){                
                auto mod_key = mods.find(mod_of_sub(target, nums[larger_band[i]]));
                if(mod_key != mods.end())
                    return {larger_band[i], mod_key->second};
            }            
        } 
        return {EXIT_FAILURE};
        
    }
};