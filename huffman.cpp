#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <cstring>

using namespace std;

struct Node {
    char character;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f)  {
        character=c;
        freq=f;
        left=right=NULL;
    }
};

struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (const auto& pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second));
    }
    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();
        Node* internalNode = new Node('$', left->freq + right->freq);
        internalNode->left = left;
        internalNode->right = right;
        minHeap.push(internalNode);
    }
    return minHeap.top();
}

void generateCodes(Node* root, const string& currentCode, unordered_map<char, string>& huffmanCodes) {
    if (root == nullptr) return;
    if (!root->left && !root->right) {
        huffmanCodes[root->character] = currentCode;
    }
    generateCodes(root->left, currentCode + "0", huffmanCodes);
    generateCodes(root->right, currentCode + "1", huffmanCodes);
}

void writeCompressedFile(const string& inputFile, const string& outputFile, const unordered_map<char, string>& huffmanCodes) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    size_t mapSize = huffmanCodes.size();
    outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    for (const auto& pair : huffmanCodes) {
        outFile.write(&pair.first, sizeof(pair.first));
        size_t codeLength = pair.second.length();
        outFile.write(reinterpret_cast<const char*>(&codeLength), sizeof(codeLength));
        outFile.write(pair.second.c_str(), codeLength);
    }
    string encodedData;
    char ch;
    while (inFile.get(ch)) {
        encodedData += huffmanCodes.at(ch);
    }
    size_t padding = 8 - (encodedData.length() % 8);
    encodedData.append(padding, '0');
    outFile.write(reinterpret_cast<const char*>(&padding), sizeof(padding));
    for (size_t i = 0; i < encodedData.length(); i += 8) {
        bitset<8> byte(encodedData.substr(i, 8));
        unsigned char byteValue = static_cast<unsigned char>(byte.to_ulong());
        outFile.write(reinterpret_cast<const char*>(&byteValue), sizeof(byteValue));
    }
    inFile.close();
    outFile.close();
}

void readHuffmanCodes(ifstream& inFile, unordered_map<char, string>& huffmanCodes) {
    size_t mapSize;
    inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
    for (size_t i = 0; i < mapSize; ++i) {
        char ch;
        inFile.read(&ch, sizeof(ch));
        size_t codeLength;
        inFile.read(reinterpret_cast<char*>(&codeLength), sizeof(codeLength));
        vector<char> codeBuffer(codeLength);
        inFile.read(codeBuffer.data(), codeLength);
        huffmanCodes[ch] = string(codeBuffer.begin(), codeBuffer.end());
    }
}

void decodeFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(outputFile, ios::binary);
    unordered_map<char, string> huffmanCodes;
    readHuffmanCodes(inFile, huffmanCodes);
    unordered_map<string, char> reverseHuffmanCodes;
    for (const auto& pair : huffmanCodes) {
        reverseHuffmanCodes[pair.second] = pair.first;
    }
    size_t padding;
    inFile.read(reinterpret_cast<char*>(&padding), sizeof(padding));
    string encodedData;
    char byte;
    while (inFile.read(&byte, sizeof(byte))) {
        bitset<8> byteBits(byte);
        encodedData += byteBits.to_string();
    }
    encodedData = encodedData.substr(0, encodedData.length() - padding);
    string currentCode;
    for (char bit : encodedData) {
        currentCode += bit;
        if (reverseHuffmanCodes.find(currentCode) != reverseHuffmanCodes.end()) {
            outFile.put(reverseHuffmanCodes[currentCode]);
            currentCode.clear();
        }
    }
    inFile.close();
    outFile.close();
}

int main() {
    string inputFile = "crossword.txt";
    string compressedFile = "input_compressed.txt";
    string decompressedFile = "input_decompressed.txt";
    unordered_map<char, int> freqMap;
    ifstream inFile(inputFile, ios::binary);
    char ch;
    while (inFile.get(ch)) {
        freqMap[ch]++;
    }
    inFile.close();
    Node* root = buildHuffmanTree(freqMap);
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);
    writeCompressedFile(inputFile, compressedFile, huffmanCodes);
    decodeFile(compressedFile, decompressedFile);
    cout << "Compression and decompression completed successfully!" << endl;
    return 0;
}