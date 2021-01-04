#include <iostream>
#include "RadiPako.hpp"
#include <fstream>
#include <vector>
#include <string>

const char *RadiPako::VersionString() { return "RadiPako 2.0.0.0"; }
const char VB[4] = {2, 0, 0, 0};

const int startByte = 0x10;

struct File
{
public:
    int size = 0;
    std::string name;
    unsigned char *content;
};

class RPK
{
public:
    const char *MagicNumber = "RPKO";
    int size = 0;
    char Version[4];
    int NumOfFiles = 0;
    std::vector<File *> Files;
};

char *ConvertToByte(int value)
{
    union uni
    {
        char byte[4];
        int integer;
        float point;
    };
    uni *un = (uni *)malloc(sizeof(uni));
    un->integer = value;
    char *temp = un->byte;
    return temp;
}

int ConvertToint(char *value)
{
    union uni
    {
        char byte[4];
        int integer;
        float point;
    };
    uni *un = (uni *)malloc(sizeof(uni));
    un->byte[0] = value[0];
    un->byte[1] = value[1];
    un->byte[2] = value[2];
    un->byte[3] = value[3];
    int temp = un->integer;
    return temp;
}

File *RadiPako::GetFile(RPK *File, const char *Filename)
{
    for (int i = 0; i < File->NumOfFiles; i++)
    {
        if (File->Files[i]->name == Filename)
        {
            return File->Files[i];
        }
    }
    return nullptr;
}

char *RadiPako::GetFile_Content(RPK *File, const char *Filename)
{
    for (int i = 0; i < File->NumOfFiles; i++)
    {
        if (File->Files[i]->name == Filename)
        {
            int size = abs((File->Files[i]->name.length() + 1) - File->Files[i]->size);
            char *ret = new char[size];
            for (int x = 0; x < size; x++)
            {
                ret[x] = File->Files[i]->content[x];
            }
            return ret;
        }
    }
    return nullptr;
}

char *RadiPako::GetFile_Content(File *File, const char *Filename)
{
    if (File->name == Filename)
    {
        int size = abs((File->name.length() + 1) - File->size);
        char *ret = new char[size];
        for (int x = 0; x < size; x++)
        {
            ret[x] = File->content[x];
        }
        return ret;
    }
    return nullptr;
}

unsigned char *RadiPako::GetFile_Content_Uchar(RPK *File, const char *Filename)
{
    for (int i = 0; i < File->NumOfFiles; i++)
    {
        if (File->Files[i]->name == Filename)
        {
            int size = abs((File->Files[i]->name.length() + 1) - File->Files[i]->size);
            unsigned char *ret = new unsigned char[size];
            for (int x = 0; x < size; x++)
            {
                ret[x] = File->Files[i]->content[x];
            }
            return ret;
        }
    }
    return nullptr;
}

unsigned char *RadiPako::GetFile_Content_Uchar(File *File, const char *Filename)
{
    if (File->name == Filename)
    {
        int size = abs((File->name.length() + 1) - File->size);
        unsigned char *ret = new unsigned char[size];
        for (int x = 0; x < size; x++)
        {
            ret[x] = File->content[x];
        }
        return ret;
    }
    return nullptr;
}

RPK *RadiPako::JointFiles(int numberoffiles, char **filepath)
{
    RPK *rpk_f = new RPK();
    for (int i = 0; i < numberoffiles; i++)
    {
        File *temp = new File;
        std::ifstream stream(filepath[i], std::ios_base::binary);
        if (!stream.good())
        {
            std::cout << filepath[i] << " do not exists! iterator = " << i << std::endl;
            return nullptr;
        }
        temp->name = filepath[i];
        stream.seekg(0, std::ios_base::end);
        temp->size = stream.tellg();
        stream.seekg(0, std::ios_base::beg);
        temp->content = new unsigned char[temp->size];
        stream.read((char *)temp->content, temp->size);
        rpk_f->Files.push_back(temp);
        rpk_f->size += temp->size;
        rpk_f->NumOfFiles++;
        rpk_f->Version[0] = VB[0];
        rpk_f->Version[1] = VB[1];
        rpk_f->Version[2] = VB[2];
        rpk_f->Version[3] = VB[3];
        stream.close();
    }
    return rpk_f;
}

RPK *RadiPako::JointFiles(int numberoffiles, std::vector<std::string> filepath)
{
    RPK *rpk_f = new RPK();
    for (int i = 0; i < numberoffiles; i++)
    {
        File *temp = new File;
        std::ifstream stream(filepath[i], std::ios_base::binary);
        if (!stream.good())
        {
            std::cout << filepath[i] << " do not exists! iterator = " << i << std::endl;
            return nullptr;
        }
        temp->name = filepath[i];
        stream.seekg(0, std::ios_base::end);
        temp->size = stream.tellg();
        stream.seekg(0, std::ios_base::beg);
        temp->content = new unsigned char[temp->size];
        stream.read((char *)temp->content, temp->size);
        rpk_f->Files.push_back(temp);
        rpk_f->size += temp->size;
        rpk_f->NumOfFiles++;
        rpk_f->Version[0] = VB[0];
        rpk_f->Version[1] = VB[1];
        rpk_f->Version[2] = VB[2];
        rpk_f->Version[3] = VB[3];
        stream.close();
    }
    return rpk_f;
}

int RadiPako::CreateFile(RPK *rpk_file, const char *path)
{
    std::ofstream stream(path, std::ios_base::ate);
    stream.write(rpk_file->MagicNumber, 4);
    stream.write(ConvertToByte(rpk_file->size), 4);
    stream.write(rpk_file->Version, 4);
    stream.write(ConvertToByte(rpk_file->NumOfFiles), 4);
    for (int i = 0; i < rpk_file->NumOfFiles; i++)
    {
        File *file = rpk_file->Files[i];
        int size = file->size + file->name.length() + 1;
        stream.write(ConvertToByte(size), 4);
        stream.write(file->name.c_str(), file->name.length());
        stream << '\0';
        stream.write((char *)file->content, file->size);
    }
    stream.close();
    return 1;
}

RPK *RadiPako::LoadRPKFile(const char *path)
{
    std::ifstream stream(path, std::ios_base::binary);
    if (!stream.good())
    {
        return nullptr;
    }
    RPK *temp = new RPK();
    stream.seekg(4, std::ios_base::beg);
    char bytes[4];
    stream.read(bytes, 4);
    temp->size = ConvertToint(bytes);
    stream.seekg(8, std::ios_base::beg);
    stream.read(temp->Version, 4);
    stream.seekg(12, std::ios_base::beg);
    stream.read(bytes, 4);
    temp->NumOfFiles = ConvertToint(bytes);
    stream.seekg(startByte);
    for (int i = 0; i < temp->NumOfFiles; i++)
    {
        File *file = new File();
        stream.read(bytes, 4);
        file->size = ConvertToint(bytes);
        char c;
        while (stream.get(c) && c != '\0')
        {
            file->name += c;
        }
        //stream.seekg(file->name.length(), std::ios_base::cur);
        int size = abs((file->name.length() + 1) - file->size);
        file->content = new unsigned char[size];
        for (int i = 0; i < size; i++)
        {
            stream.get(c);
            file->content[i] = c;
        }
        temp->Files.push_back(file);
    }
    return temp;
}