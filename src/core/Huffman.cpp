#include "Huffman.h" // Fixed missing extension
#include "HuffmanNode.h"
#include <iostream>
#include <map>
#include <queue>
#include <string>

using namespace std;

map<unsigned char, int> calculateFrequency(const string &s) {
    map<unsigned char, int> freq;
    for (char ch : s)
        freq[static_cast<unsigned char>(ch)]++;

    return freq;
}

HuffmanNode *buildTree(map<unsigned char, int> freq) {
    if (freq.empty()) {
        return nullptr;
    }

    priority_queue<HuffmanNode *, vector<HuffmanNode *>, CompareNode> pq;
    for (auto &[ch, cnt] : freq) {
        pq.push(new HuffmanNode(ch, cnt));
    }

    while (pq.size() != 1) {
        HuffmanNode *right = pq.top();
        pq.pop();
        HuffmanNode *left = pq.top();
        pq.pop();

        pq.push(new HuffmanNode('\0', right->frequency + left->frequency, left,
                                right));
    }

    return pq.top();
}

void buildCodeTable(HuffmanNode *root, const string &code,
                    map<unsigned char, string> &chCodes) {
    if (root == nullptr)
        return;

    // If it's a leaf node
    if (root->left == nullptr && root->right == nullptr) {
        // Handle single character edge case
        chCodes[root->character] = code.empty() ? "0" : code;
    }

    buildCodeTable(root->left, code + '0', chCodes);
    buildCodeTable(root->right, code + '1', chCodes);
}

void deleteTree(HuffmanNode *root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

pair<string, map<unsigned char, int>> encodeString(const string &s) {
    map<unsigned char, int> freq = calculateFrequency(s);

    HuffmanNode *root = buildTree(freq);

    map<unsigned char, string> charactersCodes;
    buildCodeTable(root, "", charactersCodes);

    string encoded = "";
    encoded.reserve(4 * s.length()); // Avoid using realloc() repeatedly
    for (char ch : s) {
        encoded += charactersCodes[static_cast<unsigned char>(ch)];
    }

    if (root != nullptr)
        deleteTree(root);

    return make_pair(encoded, freq);
}

std::string decodedString(const std::string &encodedString,
                          std::map<unsigned char, int> k) {
    HuffmanNode *root = buildTree(k);

    HuffmanNode *currentNode = root;
    std::string decoded = "";
    for (char ch : encodedString) {
        if (ch == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        // Leaf check
        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            decoded += currentNode->character;
            currentNode = root;
        }
    }

    deleteTree(root);

    return decoded;
}