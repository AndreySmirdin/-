#ifndef HW03_HUFFMAN_TEST_H
#define HW03_HUFFMAN_TEST_H

#include <gtest/gtest.h>
#include "huffman.h"

class HuffmanTest : public testing::Test{
public:

    void countFrequencies();
    void getCodes(); // Tests codes that we get from a tree.
    void printEncodedFile();
    void printDecodedFile();

    void encodeAndDecode(); // This test checks whole program.
};

#endif //HW03_HUFFMAN_TEST_H
