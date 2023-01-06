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
  
  k = in_bit(&rbit, &bit_length, input, &fbit);
}

int main(){
 DecodingAlg();
 return 0;
}
