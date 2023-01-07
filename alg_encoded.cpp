#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define amount_bits 16

using namespace std;

void out_bit(unsigned int bit, unsigned int* bit_length, unsigned char* wbit, FILE* output)  //запись бита
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
//записываем последовательность битов
void FollowBit(unsigned int bit, unsigned int* followbit, unsigned int* bit_length, unsigned char* wbit, FILE* output)
{
    out_bit(bit, bit_length, wbit, output);
    
    for (; *followbit > 0; (*followbit)--)
    {
        out_bit(!bit, bit_length, wbit, output);
    }
}

int SimvolIndex(char c, vector<pair<char, unsigned int>> vec) //нахождение индекса символа
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

void CodingAlg(){       //функция кодирования
unsigned short * ABC = new unsigned short[256];
    for (int i = 0; i < 256; i++)   //массив символов
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
            ABC[c]++;   //считываем символ и его частоту
        }
    }

    fclose(input);
  
  vector<pair<char, unsigned int>> vec; //заносим это в вектор
    for (int i=0; i < 256; i++)
    {
        if (ABC[i] != 0)
        {
            vec.push_back(make_pair(static_cast<char>(i), ABC[i]));
        }
    }

    sort (vec.begin(), vec.end(), [](const pair<char, unsigned int> &l, const pair<char, unsigned int> &r) //сортируем
    {
        if (l.second != r.second)
        {
            return l.second >= r.second;
        }

        return l.first < r.first;
    }
    );
  
    unsigned int* table = new unsigned int[vec.size() + 2]; //составляем таблицу
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
    fputc(count_simvol, output); //записываем в файл количество символов


    for (int i = 0; i < 256; i++)
    {
        if (ABC[i] != 0) 
        {
            fputc(static_cast<char>(i), output);    //записываем в файл символ
            fwrite(reinterpret_cast<const char*>(&ABC[i]), sizeof(unsigned short), 1, output);//его частоту
        }
    }
    
    while (!feof(input))
    {  
        c = fgetc(input);

        if (!feof(input))
        {
            j = SimvolIndex(c, vec);        //находим индекс символа

            //сужаем область относительно данного символа
            max_v = min_v +  table[j] * dif / del - 1;
            min_v = min_v + table[j - 1] * dif  / del;

            for (int i = 1; i < 16; i++) //записываем биты в цикле
            {   
                if (max_v < quart2)
                {
                    FollowBit(0, &followbit, &bit_length, &wbit, output);
                }
                else if (min_v >= quart2)
                {
                    FollowBit(1, &followbit, &bit_length, &wbit, output);
                    min_v -= quart2;
                    max_v -= quart2;
                }
                else if ((min_v >= quart1) && (max_v < quart3))
                {
                    followbit++;
                    min_v -= quart1;
                    max_v -= quart1;
                }
                else
                {
                    break;
                }

                min_v += min_v;
                max_v += max_v + 1;
            }
        }
        else
        {  
            max_v = min_v +  table[1] * dif / del - 1;
            min_v = min_v + table[0] * dif  / del;

            for (int i=1;i<=16;i++)
            {
                if (max_v < quart2)
                {
                    FollowBit(0, &followbit, &bit_length, &wbit, output);
                }
                else if (max_v >= quart2)
                {
                    FollowBit(1, &followbit, &bit_length, &wbit, output);
                    min_v -= quart2;
                    max_v -= quart2;
                }
                else if ((min_v >= quart1) && (max_v < quart3))
                {
                    followbit++;
                    min_v -= quart1;
                    max_v -= quart1;
                }
                else
                {
                    break;
                }

                min_v += min_v;
                max_v += max_v + 1;
            }

            followbit+=1;

            if (min_v < quart1)
            {
                FollowBit(0, &followbit, &bit_length, &wbit, output);
            }
            else
            {
                FollowBit(1, &followbit, &bit_length, &wbit, output);
            }

            wbit >>= bit_length;
            fputc(wbit, output);
        }
        dif = max_v - min_v + 1;
    }

    fclose(input); //закрытие
    fclose(output);//файлов
}

int main(){
CodingAlg();//вызов функции кодирования
return 0;
}
