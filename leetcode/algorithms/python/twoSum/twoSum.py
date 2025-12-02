# author jx H
# date 2025-09-27
#暴力破解法
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        for i in range(len(nums)):
            for j in range(i + 1, len(nums)):
                if nums[i] + nums[j] == target:
                    return [i, j]
        return []
        
# 哈希表
class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        m_mid = dict()
        for i,num in enumerate(nums):
            if target - num in m_mid:
                return [m_mid[target - num], i]
            m_mid[num] = i
        return []
        