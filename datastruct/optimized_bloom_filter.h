#include <vector>
#include <cstdint>

class OptimizedBloomFilter {
private:
    std::vector<uint64_t> bitArray;  // 使用uint64_t提高空间效率
    size_t numHashFunctions;
    size_t bitSize;
    
    // MurmurHash3 的简化版本
    uint32_t murmurHash(const char* data, size_t len, uint32_t seed) const {
        uint32_t h = seed;
        uint32_t c1 = 0xcc9e2d51;
        uint32_t c2 = 0x1b873593;
        const uint32_t* chunks = reinterpret_cast<const uint32_t*>(data);
        const uint32_t* end = chunks + (len / 4);
        
        while (chunks < end) {
            uint32_t k = *chunks++;
            k *= c1;
            k = (k << 15) | (k >> 17);
            k *= c2;
            
            h ^= k;
            h = (h << 13) | (h >> 19);
            h = h * 5 + 0xe6546b64;
        }
        
        const uint8_t* tail = reinterpret_cast<const uint8_t*>(chunks);
        uint32_t k1 = 0;
        
        switch (len & 3) {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1: k1 ^= tail[0];
                    k1 *= c1;
                    k1 = (k1 << 15) | (k1 >> 17);
                    k1 *= c2;
                    h ^= k1;
        }
        
        h ^= len;
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        
        return h;
    }
    
    bool getBit(size_t index) const {
        size_t wordIndex = index / 64;
        size_t bitIndex = index % 64;
        return (bitArray[wordIndex] >> bitIndex) & 1ULL;
    }
    
    void setBit(size_t index) {
        size_t wordIndex = index / 64;
        size_t bitIndex = index % 64;
        bitArray[wordIndex] |= (1ULL << bitIndex);
    }

public:
    OptimizedBloomFilter(size_t expectedElements, double falsePositiveRate = 0.01) {
        size_t m = static_cast<size_t>(
            -expectedElements * std::log(falsePositiveRate) / 
            (std::log(2) * std::log(2))
        );
        
        numHashFunctions = static_cast<size_t>(
            (static_cast<double>(m) / expectedElements) * std::log(2)
        );
        
        if (numHashFunctions == 0) numHashFunctions = 1;
        
        bitSize = m;
        bitArray.resize((m + 63) / 64, 0);
    }
    
    void add(const std::string& key) {
        const char* data = key.c_str();
        size_t len = key.length();
        
        for (size_t i = 0; i < numHashFunctions; ++i) {
            uint32_t hash = murmurHash(data, len, static_cast<uint32_t>(i));
            size_t index = hash % bitSize;
            setBit(index);
        }
    }
    
    bool contains(const std::string& key) const {
        const char* data = key.c_str();
        size_t len = key.length();
        
        for (size_t i = 0; i < numHashFunctions; ++i) {
            uint32_t hash = murmurHash(data, len, static_cast<uint32_t>(i));
            size_t index = hash % bitSize;
            if (!getBit(index)) {
                return false;
            }
        }
        return true;
    }
};