#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include "huffman.h"

void HuffmanEncoder::countFrequencies() {
    for (int i = 0; i < CHARS_NUMBER; i++) {
        frequency[i].first = 0;
        frequency[i].second = (char) i;
    }
    unsigned char c;
    ifs->read((char*) &c, sizeof(char));
    while (ifs->gcount() > 0) {

        frequency[c].first++;
        ifs->read((char*) &c, sizeof(char));
    }

    std::sort(frequency, frequency + CHARS_NUMBER);
    std::reverse(frequency, frequency + CHARS_NUMBER);

    while (notNullSymbols < CHARS_NUMBER && frequency[notNullSymbols].first != 0) // Finding chars that appeared in the file.
        notNullSymbols++;
}


HuffmanTree& HuffmanArchiever::buildTree(HuffmanTree& tree) {
    std::priority_queue<HuffmanTree::TreeNode*, std::vector<HuffmanTree::TreeNode*>, HuffmanTree::TreeNode::cmpNodePointers> q;

    for (int i = 0; i < notNullSymbols; i++) {
        HuffmanTree::TreeNode *node = new HuffmanTree::TreeNode;
        node->value = frequency[i].first;
        q.push(node);
        tree.leafs.push_back(node);
    }

    if (notNullSymbols == 1) {
        tree.leafs[0]->nxt = tree.root;
        tree.leafs[0]->bit = 1;
        return tree;
    }

    while (!q.empty()){
        HuffmanTree::TreeNode *n1 = q.top(); q.pop();

        if (q.empty()){
            tree.root = n1;
            break;
        }

        HuffmanTree::TreeNode *n2 = q.top(); q.pop();
        HuffmanTree::TreeNode *n3 = new HuffmanTree::TreeNode(); // Sum of two nodes.
        n3->value = n1->value + n2->value;
        n3->prev1 = n1;
        n3->prev2 = n2;
        q.push(n3);
        n1->nxt = n2->nxt = n3;
        n1->bit = 0;
        n2->bit = 1;
    }

    return tree;
}


void HuffmanArchiever::makeCodingTable(HuffmanTree& tree) {
    HuffmanTree::TreeNode* node;
    for (int i = 0; i < notNullSymbols; i++) { // Getting codes from the tree.
        node = tree.leafs[i];
        while (node != tree.root) {
            if (node->bit)
                table[(unsigned char) frequency[i].second] += '1';
            else
                table[(unsigned char) frequency[i].second] += '0';
            node = node->nxt;
        }
    }

    for (int i = 0; i < CHARS_NUMBER; i++) { // Reverse codes.
        std::string s = table[i];
        std::reverse(s.begin(), s.end());
        table[i] = s;
    }
}


void HuffmanArchiever::printInfo() {
    std::cout << source_size << "\n" << output_size << "\n" << info_size;
}


void HuffmanEncoder::printTable() {
    ofs->write((char*) &notNullSymbols, sizeof(int32_t));
    int32_t empty = 0;
    ofs->write((char*) &empty, sizeof(int32_t)); // Need space to write size of encoded text later.
    for (int i = 0; i < notNullSymbols; i++){
        info_size += 5; // Sizeof(char) + sizeof(int)
        ofs->write(&frequency[i].second, sizeof(char));
        ofs->write((char*) &frequency[i].first, sizeof(int32_t));
    }
}


char getByte(std::string s){
    char byte = 0;
    for (int i = 7; i >= 0; i--)
        byte += (s[i] - '0') << (7 - i);
    return byte;
}


void HuffmanEncoder::printEncodedFile() {
    char c, byte;

    std::string buffer;
    ifs->read(&c, sizeof(char));
    while (ifs->gcount() > 0) {
        buffer += table[(unsigned char) c];
        while (buffer.length() >= 8) { // Writing next byte if possible.
            byte = getByte(buffer);
            ofs->write(&byte, sizeof(char));
            buffer.erase(buffer.begin(), buffer.begin() + 8);
            output_size++;
        }
        source_size++;
        ifs->read(&c, sizeof(char));
    }

    char last_size = buffer.length();
    size_in_bits = output_size * 8;
    if (last_size != 0){ // Some difficulties if we have from 1 to 7 bits left.
        while (buffer.length() != 8)
            buffer += "0";
        byte = getByte(buffer);
        ofs->write(&byte, sizeof(char));
        output_size++;
        size_in_bits += last_size;
    }

    ofs->seekp(4, ofs->beg);
    ofs->write((char *) &size_in_bits, sizeof(int32_t));
}


void HuffmanEncoder::encode() {
    countFrequencies();

    ifs->clear();// Reading file again.
    ifs->seekg(0);

    HuffmanTree tree;
    buildTree(tree);
    makeCodingTable(tree);

    printTable();
    printEncodedFile();

    printInfo();
}


void HuffmanDecoder::readTable() {
    ifs->read((char*) &notNullSymbols, sizeof(int32_t));
    ifs->read((char*) &size_in_bits, sizeof(int32_t));

    std::string s;
    for (int i = 0; i < notNullSymbols; i++){
        info_size += 5;
        ifs->read(&frequency[i].second, sizeof(char));
        ifs->read((char*) &frequency[i].first, sizeof(int32_t));
    }
}


std::string getStringFromByte(char b){
    std::string res = "";
    for (int i = 0; i < 8; i++)
        res += (((b >> (7 - i)) & 1) + '0');
    return res;
}


void HuffmanDecoder::printDecodedFile() {
    std::string buffer = "", byte = "";
    char nextbyte;

    for (int i = 0; i < size_in_bits; i++){
        if (byte == "") { // Reading next byte is necessary.
            ifs->read(&nextbyte, sizeof(nextbyte));
            source_size++;
            byte = getStringFromByte(nextbyte);
        }

        buffer += byte[0];
        byte.erase(byte.begin(), byte.begin() + 1);

        if (codes.count(buffer)){ // Printing next element.
            *ofs << codes[buffer];
            output_size++;
            buffer.clear();
        }
    }

}

void HuffmanDecoder::decode() {
    readTable();

    HuffmanTree tree;
    buildTree(tree);
    makeCodingTable(tree);

    for (int i = 0; i < CHARS_NUMBER; i++)
        if (table[i] != "")
            codes[table[i]] = i;

    printDecodedFile();

    printInfo();
}
