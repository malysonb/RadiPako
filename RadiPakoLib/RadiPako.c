/**
  * Loads or writes a binary file to an a buffer that can be a struct or class (c++).
  * @file RadiPako.c
  * @author Malyson Breno de Souza
  * @version 1.0
  */

#include "RadiPako.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

char* VersionString = "RadiPako v1.2.0";

char Version[4] = {1, 2, 0, 0};
const int FirstFileAddress = 0x10;

long totalsize = 0;
int RPK_filesize = 0;

union AddressType {
    int integer;
    char bytes[4];
} Address;

typedef struct ContentType
{
    int sizeOfTheFile;
    char nameOfTheFile[260];
    unsigned char *allContent;
} Content;

typedef struct Rpk
{
    char MagicNumber[4];
    int Size;
    char FileVersion[4];
    int NumberOfFiles;
} RpkFile;

RpkFile *temp;
Content **files;

unsigned char *RPK_GetFile_Uchar(const char *RPKpath, const char *Filename)
{
    FILE *myfile = fopen(RPKpath, "rb");
    if (myfile != NULL)
    {
        fseek(myfile, 0L, SEEK_END);
        int size = ftell(myfile);
        fseek(myfile, 0L, SEEK_SET);
        unsigned char *buffer = malloc(size * sizeof(char));
        fread(buffer, size, 1, myfile);
        int iterator;
        for (iterator = FirstFileAddress; iterator <= size;)
        {
            int actualSector = iterator;
            Address.bytes[0] = buffer[iterator];
            Address.bytes[1] = buffer[iterator + 1];
            Address.bytes[2] = buffer[iterator + 2];
            Address.bytes[3] = buffer[iterator + 3];
            int end = Address.integer;
            iterator += 4;
            int ci = 0;
            unsigned char thisfile[1000];
            while (buffer[iterator] != '\0')
            {
                thisfile[ci] = buffer[iterator];
                ci++;
                iterator++;
            }
            thisfile[ci] = '\0';
            if (strcmp(Filename, thisfile) == 0)
            {
                iterator++;
                int realsize = end - ci;
                RPK_filesize = realsize;
                unsigned char *res = malloc(realsize);
                int resIt = 0;
                int final = realsize - 1;
                while (final > 0)
                {
                    res[resIt] = buffer[iterator];
                    resIt++;
                    iterator++;
                    final--;
                }
                return res;
            }
            else
            {
                iterator = ((actualSector + 4) + end) + 1;
            }
        }
    }
    else
    {
        printf("This file doesn't exists!");
    }
    return NULL;
}

char *RPK_GetFile(const char *RPKpath, const char *Filename)
{
    FILE *myfile = fopen(RPKpath, "rb");
    if (myfile != NULL)
    {
        fseek(myfile, 0L, SEEK_END);
        int size = ftell(myfile);
        fseek(myfile, 0L, SEEK_SET);
        char *buffer = malloc(size * sizeof(char));
        fread(buffer, size, 1, myfile);
        int iterator;
        for (iterator = FirstFileAddress; iterator <= size;)
        {
            int actualSector = iterator;
            Address.bytes[0] = buffer[iterator];
            Address.bytes[1] = buffer[iterator + 1];
            Address.bytes[2] = buffer[iterator + 2];
            Address.bytes[3] = buffer[iterator + 3];
            int end = Address.integer;
            iterator += 4;
            int ci = 0;
            char thisfile[1000];
            while (buffer[iterator] != '\0')
            {
                thisfile[ci] = buffer[iterator];
                ci++;
                iterator++;
            }
            thisfile[ci] = '\0';
            if (strcmp(Filename, thisfile) == 0)
            {
                iterator++;
                int realsize = end - ci;
                RPK_filesize = realsize;
                char *res = malloc(realsize);
                int resIt = 0;
                int final = realsize - 1;
                while (final > 0)
                {
                    res[resIt] = buffer[iterator];
                    resIt++;
                    iterator++;
                    final--;
                }
                return res;
            }
            else
            {
                iterator = ((actualSector + 4) + end) + 1;
            }
        }
    }
    else
    {
        printf("This file doesn't exists!");
    }
    return NULL;
}

int RPK_CreateFile(const char *path)
{
    FILE *myfile = fopen(path, "wb");
    fwrite(temp, temp->Size, 1, myfile);
    fclose(myfile);
    FILE *myContent = fopen(path, "ab");
    for (int i = 0; i < temp->NumberOfFiles; i++)
    {
        int trueEnding = files[i]->sizeOfTheFile + 1;
        fwrite(&trueEnding, sizeof(int), 1, myContent);
        fwrite(files[i]->nameOfTheFile, strlen(files[i]->nameOfTheFile), 1, myContent);
        fwrite("\0", sizeof(char), 1, myContent);
        fwrite(files[i]->allContent, files[i]->sizeOfTheFile - strlen(files[i]->nameOfTheFile), 1, myContent);
        fwrite("\0", sizeof(char), 1, myContent);
    }
    fclose(myfile);
    return 0;
}

int RPK_CreatePackage(int nOfFiles)
{
    temp = malloc(sizeof(RpkFile));
    temp->MagicNumber[0] = 'R';
    temp->MagicNumber[1] = 'P';
    temp->MagicNumber[2] = 'K';
    temp->MagicNumber[3] = 'O';
    temp->Size = sizeof(RpkFile);
    temp->FileVersion[0] = Version[0];
    temp->FileVersion[1] = Version[1];
    temp->FileVersion[2] = Version[2];
    temp->FileVersion[3] = Version[3];
    temp->NumberOfFiles = nOfFiles;
    return 0;
}

int RPK_JointALotOfFiles(int numberoffiles, ...)
{
    va_list valist;
    va_start(valist, numberoffiles);
    files = malloc(numberoffiles * sizeof(Content));
    for (int i = 0; i < numberoffiles; i++)
    {
        files[i] = malloc(sizeof(Content));
        char *filename = va_arg(valist, char *);
        int len = strlen(filename) + 1;
        strcpy(files[i]->nameOfTheFile, filename);
        FILE *tempfile = fopen(filename, "rb");
        fseek(tempfile, 0L, SEEK_END);
        int size = ftell(tempfile);
        fseek(tempfile, 0L, SEEK_SET);
        files[i]->allContent = malloc(size * sizeof(char));
        totalsize += size + sizeof(Content);
        fread(files[i]->allContent, size, 1, tempfile);
        fclose(tempfile);
        files[i]->sizeOfTheFile = size + strlen(files[i]->nameOfTheFile);
    }
    RPK_CreatePackage(numberoffiles);
}

int RPK_JointFiles(int numberoffiles, char **filepath)
{
    files = malloc(numberoffiles * sizeof(Content));
    for (int i = 0; i < numberoffiles; i++)
    {
        files[i] = malloc(sizeof(Content));
        char *filename = filepath[i];
        int len = strlen(filename) + 1;
        strcpy(files[i]->nameOfTheFile, filename);
        FILE *tempfile = fopen(filename, "rb");
        fseek(tempfile, 0L, SEEK_END);
        int size = ftell(tempfile);
        fseek(tempfile, 0L, SEEK_SET);
        files[i]->allContent = malloc(size * sizeof(char));
        totalsize += size + sizeof(Content);
        fread(files[i]->allContent, size, 1, tempfile);
        fclose(tempfile);
        files[i]->sizeOfTheFile = size + strlen(files[i]->nameOfTheFile);
    }
    RPK_CreatePackage(numberoffiles);
}