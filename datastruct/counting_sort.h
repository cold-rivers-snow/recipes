std::vector<int> countingSort(const std::vector<int>& nums) {
    if (nums.empty()) return {};

    int min_val = *std::min_element(nums.begin(), nums.end());
    int max_val = *std::max_element(nums.begin(), nums.end());
    int range = max_val - min_val + 1;

    std::vector<int> count(range, 0);
    for (int x : nums) {
        count[x - min_val]++;
    }

    for (int i = 1; i < range; ++i) {
        count[i] += count[i - 1];
    }

    std::vector<int> result(nums.size());
    for (int i = nums.size() - 1; i >= 0; --i) {
        int idx = nums[i] - min_val;
        result[count[idx] - 1] = nums[i];
        count[idx]--;
    }

    return result;
}
