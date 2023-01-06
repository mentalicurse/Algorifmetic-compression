#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define code_value_bits 16

using namespace std;

int in_bit(unsigned char* rbit, unsigned int* bit_length, FILE* input, unsigned int* fbit)
{
    if ( (*bit_length) == 0 )
    {
        (*rbit) = fgetc(input);
        if (feof(input))
        {
            (*fbit)++;
            if ( (*fbit) > 14 )
            {
                cout<<"Распаковать файл не удается.Ошибка!";
                exit(0);
            }
        }
        (*bit_length) = 8;
    }
    int t = (*rbit) & 1;
    (*rbit) >>= 1;
    (*bit_length)--;
    return t;
}

void DecodingAlg(){
  unsigned short * ABC = new unsigned short[256];
    for (int i = 0; i < 256; i++)
    {
        ABC[i] = 0;
    }
    FILE* input = fopen("encoded.txt", "rb");  // Open input file
    if (input == nullptr)
    {
       cout<<"Файд не открывается.Исправьте ошибку!";
        exit(0);
    }
    unsigned char count = 0;
    unsigned int count_simvol = 0;
    count = fgetc(input);
    if (!feof(input))
    {
       count_simvol = static_cast<unsigned int>(count);
    }

    unsigned char c = 0;
  
  for (int i = 0; i < count_simvol; i++)
    {
        c = fgetc(input);
        if (!feof(input))
        {
            fread(reinterpret_cast<char*>(&ABC[c]), sizeof(unsigned short), 1, input);
        }
        else
        {
            cout<<"Распаковать файл не удается.Ошибка!";
                exit(0);
        }
    }

    vector<pair<char, unsigned int>> vec;
    for (int i = 0; i < 256; i++)
    {
        if (ABC[i] != 0)
        {
            vec.push_back(make_pair(static_cast<char>(i), ABC[i]));
        }
    }

    sort(vec.begin(), vec.end(),[](const pair<char, unsigned int> &l, const pair<char, unsigned int> &r)
    {
        if (l.second != r.second)
        {
            return l.second >= r.second;
        }
        return l.first < r.first;
    }
    );
  unsigned int bit_length = 0;
    unsigned char rbit = 0;
    unsigned int fbit = 0;
    unsigned short amount = 0;
    int k = 0;
  
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

    unsigned int bit_length = 0;
    unsigned char rbit = 0;
    unsigned int fbit = 0;
    unsigned int amount = 0;
    unsigned int del = table[vec.size()+1];
    unsigned int min_v = 0;
    unsigned int max_v = ((static_cast<unsigned int> (1) << amount_bits) - 1);
    unsigned int quart1 = (max_v + 1) / 4;
    unsigned int quart2 = quart1 * 2;
    unsigned int quart3 = quart1 * 3;
    FILE* output = fopen("output.txt", "wb +");
    int k = 0;

  
    for (int i = 1; i <= 16; i++)
    {
        k = in_bit(&rbit, &bit_length, input, &fbit);
        amount = 2 * amount + k;
    }
    unsigned int dif = max_v - min_v + 1;
}

int main(){
 DecodingAlg();
 return 0;
}
