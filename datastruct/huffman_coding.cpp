#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <functional>

struct HuffmanNode {
    char ch;                    // 字符
    int freq;                   // 频率
    HuffmanNode* left;          // 左子节点
    HuffmanNode* right;         // 右子节点
    
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    
    // 用于优先队列的比较（最小堆）
    bool operator>(const HuffmanNode& other) const {
        return freq > other.freq;
    }
};

class HuffmanCoding {
private:
    HuffmanNode* root;
    std::unordered_map<char, std::string> codes;
    std::unordered_map<char, int> freqMap;
    
    // 统计字符频率
    void buildFrequencyMap(const std::string& text) {
        freqMap.clear();
        for (char c : text) {
            freqMap[c]++;
        }
    }
    
    // 构建霍夫曼树
    HuffmanNode* buildHuffmanTree() {
        if (freqMap.empty()) return nullptr;
        
        // 最小堆（优先队列）
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, 
                          std::function<bool(HuffmanNode*, HuffmanNode*)>> pq(
            [](HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; }
        );
        
        // 将所有字符加入优先队列
        for (const auto& pair : freqMap) {
            pq.push(new HuffmanNode(pair.first, pair.second));
        }
        
        // 合并节点直到只剩一个根节点
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            
            HuffmanNode* merged = new HuffmanNode('\0', left->freq + right->freq);
            merged->left = left;
            merged->right = right;
            
            pq.push(merged);
        }
        
        return pq.top();
    }
    
    // 生成霍夫曼编码
    void generateCodes(HuffmanNode* node, const std::string& code) {
        if (!node) return;
        
        if (node->ch != '\0') {
            // 叶子节点，存储编码
            codes[node->ch] = code;
        } else {
            // 内部节点，递归生成左右子树编码
            generateCodes(node->left, code + "0");
            generateCodes(node->right, code + "1");
        }
    }
    
    // 释放霍夫曼树内存
    void deleteTree(HuffmanNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    HuffmanCoding() : root(nullptr) {}
    
    ~HuffmanCoding() {
        deleteTree(root);
    }
    
    // 构建霍夫曼编码表
    void build(const std::string& text) {
        if (text.empty()) return;
        
        buildFrequencyMap(text);
        root = buildHuffmanTree();
        codes.clear();
        generateCodes(root, "");
    }
    
    // 编码文本
    std::string encode(const std::string& text) {
        std::string encoded;
        for (char c : text) {
            encoded += codes[c];
        }
        return encoded;
    }
    
    // 解码二进制字符串
    std::string decode(const std::string& encoded) {
        if (!root) return "";
        
        std::string decoded;
        HuffmanNode* current = root;
        
        for (char bit : encoded) {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            
            // 到达叶子节点
            if (current->ch != '\0') {
                decoded += current->ch;
                current = root; // 重置到根节点
            }
        }
        
        return decoded;
    }
    
    // 打印编码表
    void printCodes() const {
        for (const auto& pair : codes) {
            std::cout << "'" << pair.first << "': " << pair.second << std::endl;
        }
    }
    
    // 获取压缩率信息
    void printCompressionInfo(const std::string& original) const {
        if (original.empty()) return;
        
        size_t originalBits = original.length() * 8;
        size_t compressedBits = 0;
        for (char c : original) {
            compressedBits += codes.at(c).length();
        }
        
        double compressionRatio = (double)compressedBits / originalBits;
        std::cout << "Original size: " << originalBits << " bits" << std::endl;
        std::cout << "Compressed size: " << compressedBits << " bits" << std::endl;
        std::cout << "Compression ratio: " << compressionRatio * 100 << "%" << std::endl;
    }
};

int main() {
    std::string text = "aaabacc";
    std::cout << "Original text: " << text << std::endl;
    
    HuffmanCoding huffman;
    huffman.build(text);
    
    std::cout << "\nHuffman codes:" << std::endl;
    huffman.printCodes();
    
    std::string encoded = huffman.encode(text);
    std::cout << "\nEncoded: " << encoded << std::endl;
    
    std::string decoded = huffman.decode(encoded);
    std::cout << "Decoded: " << decoded << std::endl;
    
    std::cout << "\nCompression info:" << std::endl;
    huffman.printCompressionInfo(text);
    
    return 0;
}