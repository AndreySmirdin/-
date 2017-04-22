#include <sstream>
#include "huffman_test.h"


void HuffmanTest::countFrequencies() {
    std::stringstream in("aba CCdCad b a C d aa \n");
    std::stringstream out;
    HuffmanEncoder encoder(&in, &out);
    encoder.countFrequencies();
    ASSERT_EQ(std::make_pair(7, ' '), encoder.frequency[0]);
    ASSERT_EQ(std::make_pair(6, 'a'), encoder.frequency[1]);
    ASSERT_EQ(std::make_pair(4, 'C'), encoder.frequency[2]);
    ASSERT_EQ(std::make_pair(3, 'd'), encoder.frequency[3]);
    ASSERT_EQ(std::make_pair(2, 'b'), encoder.frequency[4]);
    ASSERT_EQ(std::make_pair(1, '\n'), encoder.frequency[5]);
}

void HuffmanTest::getCodes() {
    std::stringstream in;
    std::stringstream out;
    HuffmanEncoder encoder(&in, &out);

    encoder.frequency[0] = std::make_pair(10, 'a');
    encoder.frequency[1] = std::make_pair(6, 'b');
    encoder.frequency[2] = std::make_pair(5, 'c');
    encoder.frequency[3] = std::make_pair(3, ' ');
    encoder.frequency[4] = std::make_pair(1, '\n');
    encoder.notNullSymbols = 5;

    HuffmanTree t;
    encoder.buildTree(t);
    encoder.makeCodingTable(t);

    ASSERT_TRUE(("0" == encoder.table['a'] && "10" == encoder.table['b'] && "111" == encoder.table['c'] && "1101" == encoder.table[' '] && "1100" == encoder.table['\n']) ||
                ("1" == encoder.table['a'] && "01" == encoder.table['b'] && "000" == encoder.table['c'] && "0010" == encoder.table[' '] && "0011" == encoder.table['\n']));

}

void testOneInput(std::string s) {
    std::stringstream in(s);
    std::stringstream mid;
    std::stringstream out;

    HuffmanEncoder encoder(&in, &mid);
    HuffmanDecoder decoder(&mid, &out);

    encoder.encode();
    decoder.decode();

    ASSERT_EQ(s, out.str());
}

void HuffmanTest::encodeAndDecode() {
    testOneInput(""); // Empty.
    testOneInput("aaa"); // One symbol.
    testOneInput("abba");
    testOneInput("abcd");
    testOneInput("fhdsighfsgjfdkojfdahgojerpkvldjifdkjhkalsdfhsfakhgfdklbgkfljdsh"); // Random symbols.

    std::string s;
    for (int i = 0; i < 1e6; i++)
        s += (char) i % 256;
    testOneInput(s); // Big file with all symbols;
}

void HuffmanTest::printEncodedFile() {
    std::stringstream in("abba cba bac a");
    std::stringstream out;

    HuffmanEncoder encoder(&in, &out);
    encoder.table['a'] = "0";
    encoder.table['b'] = "10";
    encoder.table['c'] = "111";
    encoder.table[' '] = "110";
    std::stringstream s;
    encoder.notNullSymbols = 4;
    int32_t size_in_bits = 28;

    std::string ans;
    ans += ((char) 83); // 01010011
    ans += ((char) 121); // 01111001
    ans += ((char) 167); //10100111
    ans += ((char) 192); // 1100 0000

    s.write((char *) ans.c_str(), 4);
    s.write((char *) &size_in_bits, sizeof(int32_t));
    encoder.printEncodedFile();

    ASSERT_EQ(s.str(), out.str());
}


void HuffmanTest::printDecodedFile() {
    std::stringstream in("Sy\xA7\xC0\\x");
    std::stringstream out;

    HuffmanDecoder decoder(&in, &out);
    decoder.codes["0"] = 'a';
    decoder.codes["10"] = 'b';
    decoder.codes["111"] = 'c';
    decoder.codes["110"] = ' ';
    decoder.size_in_bits = 28;
    decoder.printDecodedFile();

    ASSERT_EQ("abba cba bac a", out.str());
}


TEST_F(HuffmanTest, countFrequencies){
    countFrequencies();
}

TEST_F(HuffmanTest, getCodes){
    getCodes();
}

TEST_F(HuffmanTest, encodeAndDecode){
    encodeAndDecode();
}

TEST_F(HuffmanTest, printEncodedFile) {
    printEncodedFile();
}

TEST_F(HuffmanTest, printDecodedFile) {
    printDecodedFile();
}

