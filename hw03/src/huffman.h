#ifndef HW03_HUFFMAN_H
#define HW03_HUFFMAN_H

#include <map>
#include <vector>
#include <fstream>
#include "huffman_test.h"

const int CHARS_NUMBER = 256;

class HuffmanTree{
public:

    ~HuffmanTree(){
        delete root;
    }

    class TreeNode{
    public:

        TreeNode *nxt, *prev1, *prev2;
        bool bit;
        int value;

        TreeNode() :nxt(NULL), prev1(NULL), prev2(NULL) {};
        struct cmpNodePointers{
            bool operator() (const TreeNode* n1, const TreeNode* n2){
                return -n1->value < -n2->value;
            }
        };

        ~TreeNode(){
            delete prev1;
            delete prev2;
        }

    private:
        TreeNode(const TreeNode& t);
        TreeNode& operator=(const TreeNode t);
    };

    TreeNode* root = NULL;
    std::vector<TreeNode*> leafs;

};

//------------------------------------------------------------------------------------------

class HuffmanArchiever{
    friend class HuffmanTest;
protected:
    int32_t source_size = 0, output_size = 0, info_size = 8, size_in_bits;

    std::istream* ifs;
    std::ostream* ofs;

    int32_t notNullSymbols = 0;
    std::pair<int32_t, char> frequency[CHARS_NUMBER];
    std::string table[CHARS_NUMBER];

    HuffmanTree& buildTree(HuffmanTree& tree);
    void makeCodingTable(HuffmanTree& tree);

    void printInfo();

public:

    HuffmanArchiever(std::istream* ifs, std::ostream* ofs) : ifs(ifs), ofs(ofs) {};
};


class HuffmanEncoder:public HuffmanArchiever{
    friend class HuffmanTest;

public:
    HuffmanEncoder(std::istream* ifs, std::ostream* ofs) : HuffmanArchiever(ifs, ofs) {};
    void encode();

private:
    void countFrequencies();
    void printTable();
    void printEncodedFile();
};


class HuffmanDecoder:public HuffmanArchiever{
    friend class HuffmanTest;

public:
    HuffmanDecoder(std::istream* ifs, std::ostream* ofs) : HuffmanArchiever(ifs, ofs) {};
    void decode();

private:
    void readTable();
    void printDecodedFile();

    std::map<std::string, char> codes;
};

#endif //HW03_HUFFMAN_H
