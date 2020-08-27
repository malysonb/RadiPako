#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RadiPakoLib/RadiPako.h"

int main(int argc, char const *argv[])
{
    printf("%s\n", VersionString);
    int notScan = -1;
    char dest[260];
    char paths [argc][260];
    int iterator = 0;
    int length = 0;
    int success = -1;
    printf("%d arguments!\n", argc - 1);
    for (int i = 1; i < argc; i++)
    {
        if (argc > 2)
        {
            if (argv[i][0] == '-')
            {
                switch (argv[i][1])
                {
                case 'o':
                    strcpy(dest, argv[i + 1]);
                    notScan = i + 1;
                    success = 0;
                    break;
                default:
                    break;
                }
            }
            else
            {
                if (i != notScan)
                {
                    strcpy(paths[iterator], argv[i]);
                    iterator++;
                }
            }
        }
        else
        {
            printf("You need to put some arguments.\nExample: xfile.exe -o package.rpk\n");
            return 0;
        }
    }
    char **pathsToUse = malloc(iterator);
    for (int j = 0; j < iterator; j++)
    {
        pathsToUse[j] = strdup(paths[j]);
    }
    RPK_JointFiles(iterator, pathsToUse);
    if (success == 0)
    {
        if (dest != NULL)
        {
            RPK_CreateFile(dest);
            printf("Completed!\n");
        }
        else
        {
            RPK_CreateFile("Package.rpk");
            printf("Completed!\n");
        }
    }
    else
    {
        printf("Error!");
    }
    return 0;
}
