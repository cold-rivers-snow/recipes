package twoSum

func twoSum(nums []int, target int) []int {
	numMap := make(map[int]int)
	for index, num := range nums {
		if i, ok := numMap[target-num]; ok {
			return []int{i, index}
		}
		numMap[num] = index
	}
	return nil
}

// author jx H
// date 2025-09-27
//暴力破解法
class Solution {
	public:
			vector<int> twoSum(vector<int>& nums, int target) {
					vector<int> result;
					unordered_map<int, int> m_mid;
					for (int i = 0; i < nums.size();i++) {
							if (m_mid.find(nums[i]) == m_mid.end()) {
									m_mid[target-nums[i]] = i;
							} else {
									result.push_back(m_mid[nums[i]]);
									result.push_back(i);
									break;
							}
					}
					return result;
			}
	};

	