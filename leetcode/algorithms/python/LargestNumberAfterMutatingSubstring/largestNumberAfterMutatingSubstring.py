# Source : https://leetcode.com/problems/largest-number-after-mutating-substring/
# Author : Jx H
# Date   : 2025-09-21

'''***************************************************************************************************** 
 *
 * You are given a string num, which represents a large integer. You are also given a 0-indexed 
 * integer array change of length 10 that maps each digit 0-9 to another digit. More formally, digit d 
 * maps to digit change[d].
 * 
 * You may choose to mutate a single substring of num. To mutate a substring, replace each digit 
 * num[i] with the digit it maps to in change (i.e. replace num[i] with change[num[i]]).
 * 
 * Return a string representing the largest possible integer after mutating (or choosing not to) a 
 * single substring of num.
 * 
 * A substring is a contiguous sequence of characters within the string.
 * 
 * Example 1:
 * 
 * Input: num = "132", change = [9,8,5,0,3,6,4,2,6,8]
 * Output: "832"
 * Explanation: Replace the substring "1":
 * - 1 maps to change[1] = 8.
 * Thus, "132" becomes "832".
 * "832" is the largest number that can be created, so return it.
 * 
 * Example 2:
 * 
 * Input: num = "021", change = [9,4,3,5,7,2,1,9,0,6]
 * Output: "934"
 * Explanation: Replace the substring "021":
 * - 0 maps to change[0] = 9.
 * - 2 maps to change[2] = 3.
 * - 1 maps to change[1] = 4.
 * Thus, "021" becomes "934".
 * "934" is the largest number that can be created, so return it.
 * 
 * Example 3:
 * 
 * Input: num = "5", change = [1,4,7,5,3,2,5,6,9,4]
 * Output: "5"
 * Explanation: "5" is already the largest number that can be created, so return it.
 * 
 * Constraints:
 * 
 * 	1 <= num.length <= 10^5
 * 	num consists of only digits 0-9.
 * 	change.length == 10
 * 	0 <= change[d] <= 9
 ******************************************************************************************************'''


# 注：这个题要理解，子串，连续的突变，一旦有一个不突变，则就要跳出。
# 由于高位突变才会导致数值变大，因此，这个解法从左到右遍历，一旦突变中断，则说明子串中断，则即为最大突变值
class Solution:
    def maximumNumber(self, num: str, change: List[int]) -> str:
        num_list = list(num)  # 将字符串转换为列表，以便修改
        started_replacing = False  # 标记是否已开始替换
        
        for i in range(len(num_list)):
            digit = int(num_list[i])
            
            if change[digit] > digit:
                # 如果替换后的数字更大，则替换
                num_list[i] = str(change[digit])
                started_replacing = True
            elif change[digit] < digit and started_replacing:
                # 如果已经开始替换，且当前数字会变小，则停止替换
                break
            # 如果相等，保持不变
        
        return ''.join(num_list)  # 将列表转换回字符串

# 以下解法利用边界法，由于是一个子串，突变是连续的，找到左边的突变边界和右边的突变边界，修改中间突变的位置的字符串，就是最后突变的最大串。
class Solution:
    def maximumNumber(self, num: str, change: List[int]) -> str:
        num_list = list(num)  # 将字符串转换为列表，以便修改
        
        for i in range(len(num_list)):
            digit = int(num_list[i])
            
            if change[digit] > digit:
                while i < len(num_list) and change[int(num_list[i])] >= int(num_list[i]):
                    # 如果替换后的数字更大，则替换
                    num_list[i] = str(change[int(num_list[i])])
                    i += 1
                break
        
        return ''.join(num_list)  # 将列表转换回字符串
