#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RadiPakoLib/RadiPako.h"

int main(int argc, char const *argv[])
{
    printf("%s\n", VersionString);
    int notScan = -1;
    char *dest = NULL;
    char **paths = malloc(argc * sizeof(char));
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
                    length = strlen(argv[i + 1]);
                    dest = malloc(length * sizeof(char));
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
                    int len = strlen(argv[i]);
                    paths[iterator] = malloc(256 * sizeof(char));
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
    /*char **pathsToUse = malloc(iterator);
    for (int j = 0; j < iterator; j++)
    {
        pathsToUse[j] = malloc(sizeof(paths[j]));
        strcpy(pathsToUse[j], paths[j]);
    }*/
    printf("Ready to archive!\n");
    RPK_JointFiles(iterator, paths);
    printf("the files is all together!\n");
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
