#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define code_value_bits 16

using namespace std;

void out_bit(unsigned int bit, unsigned int* bit_length, unsigned char* wbit, FILE* output)
{
    (*wbit) >>= 1;
    if (bit) (*wbit) |= 128;
    (*bit_length)--;
    if ( (*bit_length) == 0 )
    {
        fputc((*wbit), output);
        (*bit_length) = 8;
    }
}

int SimvolIndex(char c, vector<pair<char, unsigned int>> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (c == vec[i].first)
        {
            return i+2;
        }
    }

    return -1;
}

void CodingAlg(){
unsigned short * ABC = new unsigned short[256];
    for (int i = 0; i < 256; i++)
    {
        ABC[i] = 0;
    }

    FILE* input = fopen("input.txt", "rb");
    if (input == nullptr)
    {
        cout<<"Файд не открывается.Исправьте ошибку!";
        exit(0);
    }

    unsigned char c = 0;

    while (!feof(input))
    {   
        c = fgetc(input);
        if (!feof(input))
        {
            ABC[c]++;
        }
    }

    fclose(input);
  
  vector<pair<char, unsigned int>> vec;
    for (int i=0; i < 256; i++)
    {
        if (ABC[i] != 0)
        {
            vec.push_back(make_pair(static_cast<char>(i), ABC[i]));
        }
    }

    sort (vec.begin(), vec.end(), [](const pair<char, unsigned int> &l, const pair<char, unsigned int> &r)
    {
        if (l.second != r.second)
        {
            return l.second >= r.second;
        }

        return l.first < r.first;
    }
    );
  
  int j = SimvolIndex(c, vec);
}

int main(){
CodingAlg();
return 0;
}
