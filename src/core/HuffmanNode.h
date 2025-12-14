#pragma once
#include <cstddef>

struct HuffmanNode {
    unsigned char character;
    int frequency;
    HuffmanNode *left;
    HuffmanNode *right;

    // Leaf node constructor
    HuffmanNode(unsigned char c, int f) : character(c), frequency(f), left(nullptr), right(nullptr) {}

    // Internal node constructor
    HuffmanNode(unsigned char c, int f, HuffmanNode* l, HuffmanNode* r) : character(c), frequency(f), left(l), right(r) {}
};

// For Priority Queue
struct CompareNode {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        // Min-Heap: Returns true if l > r, putting the smallest frequency at the top
        return l->frequency > r->frequency;
    }
};
