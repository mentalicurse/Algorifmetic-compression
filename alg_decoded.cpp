#include <stdio.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

#define code_value_bits 16

using namespace std;

int in_bit(unsigned char* rbit, unsigned int* bit_length, FILE* input, unsigned int* fbit) //чтение бита
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

void DecodingAlg(){                     //функция декодирования
  unsigned short * ABC = new unsigned short[256];
    for (int i = 0; i < 256; i++)           //создание массива
    {
        ABC[i] = 0;
    }
    FILE* input = fopen("encoded.txt", "rb");  
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
       count_simvol = static_cast<unsigned int>(count); //кол-во символов
    }

    unsigned char c = 0;
  
  for (int i = 0; i < count_simvol; i++)
    {
        c = fgetc(input);   //считываем символ
        if (!feof(input))
        {
            fread(reinterpret_cast<char*>(&ABC[c]), sizeof(unsigned short), 1, input); //его частоту
        }
        else
        {
            cout<<"Распаковать файл не удается.Ошибка!";
                exit(0);
        }
    }

    vector<pair<char, unsigned int>> vec;
    for (int i = 0; i < 256; i++)       //заносим в вектор
    {
        if (ABC[i] != 0)
        {
            vec.push_back(make_pair(static_cast<char>(i), ABC[i]));
        }
    }

    sort(vec.begin(), vec.end(),[](const pair<char, unsigned int> &l, const pair<char, unsigned int> &r)    //сортируем
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
  
 unsigned int* table = new unsigned int[vec.size() + 2];  //создаем таблицу
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

    unsigned int del = table[vec.size()+1];
    unsigned int min_v = 0;
    unsigned int max_v = ((static_cast<unsigned int> (1) << amount_bits) - 1);
    unsigned int quart1 = (max_v + 1) / 4;
    unsigned int quart2 = quart1 * 2;
    unsigned int quart3 = quart1 * 3;
    FILE* output = fopen("output.txt", "wb +");

  
    for (int i = 1; i <= 16; i++)       // запоминаем биты
    {
        k = in_bit(&rbit, &bit_length, input, &fbit);
        amount = 2 * amount + k;
    }
    unsigned int dif = max_v - min_v + 1;
    
    for (;;)
    { //находим частоту
        unsigned int freq = static_cast<unsigned int> (( (static_cast<unsigned int>(amount) - min_v + 1) * del - 1) / dif);

        int j;

      
        for (j = 1; table[j] <= freq; j++) {} //доходим пока table[j] будет > freq
        //находим max,min
        max_v = min_v +  table[j] * dif / del - 1;
        min_v = min_v + table[j - 1] * dif  / del;

        for (;;) {//меняем область
            if (max_v < quart2) {}
            else if (min_v >= quart2)
            {
                min_v -= quart2;
                max_v -= quart2;
                amount -= quart2;
            }
            else if ((min_v >= quart1) && (max_v < quart3))
            {
                min_v -= quart1;
                max_v -= quart1;
                amount -= quart1;
            }
            else
            {
                break;
            }

            min_v += min_v;
            max_v += max_v + 1;
            k = 0;
            k = in_bit(&rbit, &bit_length, input, &fbit);
            amount += amount + k;
        }

        if (j == 1)
        {
            break;
        }

       
        fputc(vec[j-2].first, output);//записываем в файл
        dif = max_v - min_v + 1;
    }

    fclose(input);  //закрытие
    fclose(output); //файла
}

int main(){
 DecodingAlg(); //вызов функции декодирования
 return 0;
}
