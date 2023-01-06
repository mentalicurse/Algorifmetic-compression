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

void FollowBit(unsigned int bit, unsigned int* followbit, unsigned int* bit_length, unsigned char* wbit, FILE* output)
{
    out_bit(bit, bit_length, wbit, output);
    
    for (; *followbit > 0; (*followbit)--)
    {
        out_bit(!bit, bit_length, wbit, output);
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
  
    unsigned int* table = new unsigned int[vec.size() + 2];
    table[0] = 0;
    table[1] = 1;
    for (int i = 0; i < vec.size(); i++)
    {
        unsigned int b = vec[i].second;
        for (int j = 0; j < i; j++)
        {
            b += vec[j].second;
        }
        table[i+2] = b;
    }

    if (table[vec.size()] > (1 << ((amount_bits - 2)) - 1))
    {
        cout<<"Слишком много.Ошибка!";
        exit(0);
    }

    unsigned int min_v = 0;
    unsigned int max_v = ((static_cast<unsigned int> (1) << amount_bits) - 1);
    unsigned int followbit = 0;
    unsigned int bit_length = 8;
    unsigned char wbit = 0;
    unsigned int quart1 = (max_v + 1) / 4;
    unsigned int quart2 = quart1 * 2;
    unsigned int quart3 = quart1 * 3;
    unsigned int del = table[vec.size()+1];
    unsigned int dif = max_v - min_v + 1;

    int j = 0;

    input = fopen("input.txt", "rb");
    FILE* output = fopen("encoded.txt", "wb +");

    char count_simvol = vec.size();
    fputc(count_simvol, output);


    for (int i = 0; i < 256; i++)
    {
        if (ABC[i] != 0) 
        {
            fputc(static_cast<char>(i), output);
            fwrite(reinterpret_cast<const char*>(&ABC[i]), sizeof(unsigned short), 1, output);
        }
    }
    
}

int main(){
CodingAlg();
return 0;
}
