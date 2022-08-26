//
//  Encoder.cpp
//  huffman
//
//  Created by Haoran Qin on 8/26/22.
//

#include "myHuffman.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ./huff fileName" << std::endl;
        exit(1);
    }
    
    std::string inname = argv[1];
    
    std::ifstream infile (inname + ".hufffile");
    std::ifstream treefile (inname + ".hufftree");
    if(!infile.is_open() || !treefile.is_open()) {
        std::cout << "can't open file: " << inname + ".hufffile or " << inname + ".hufftree" << std::endl;
        std::cout << "Please put the compressed file and tree file in the same folder!" << std::endl;
        exit(1);
    }

    std::string outname = inname + ".fromhuff";
    std::ofstream outfile(outname);
    HuffManTree* huffTree = new HuffManTree();
    
    huffTree->Decode(infile, outfile, treefile);
    std::cout << "Decode complete! File name: " + outname << std::endl;
    return 0;
}
