#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h> // stat function
#include <unistd.h>   //   function
#include <string.h>   // strchr, strcmp
#include <time.h>
#include "headers/search.h"
#include "headers/linkedlist.h"
#include "headers/constant.h"
#include "headers/util.h"

/**
 * Function: search_by_name
 * @Exec Search all the files with name indicated by filelist and print the directory
 * @Para1 name - the required file name to be searched
 * @Para2 filelist - pointer to the head of the linkedlist of the filenames
 * @Return pointer to the head of the linkedlist of the searched file
 *          NULL if fails
 */
node* search_by_name(char* name, node* filelist)
{
    DIR *dir;

    char *filepath;
    int position;
    int position2;

    dir = opendir(filelist->filepath);

    node *filelist_temp = filelist;
    node *filelist_result;
    node *tail;
    create_node(&filelist_result, "");
    tail = filelist_result;

    while (filelist_temp != NULL)
    {
        filepath = filelist_temp->filepath;

        // find the position of the last slash "/"
        char *part2;
        part2 = strchr(filepath, '/');
        while (part2 != NULL)
        {
            position2 = part2 - filepath;
            part2 = strchr(part2 + 1, '/');
        }

        // find the position of the last dot "."
        char *part;
        part = strchr(filepath, '.');
        // if filename contains dot
        if (part != NULL)
        {
            while (part != NULL)
            {
                position = part - filepath;
                part = strchr(part + 1, '.');
            }
        }
        // if filename dones't contain dot
        else
        {
            position = strlen(filepath);
        }

        // write and compare file name
        char *file_name = malloc(4);
        strncpy(file_name, filepath + position2 + 1, position - position2 - 1);

        /* hui's fix: add '\0' to the end of the file_name */
        file_name[position - position2 - 1] = '\0';
        //printf("%s \n", file_name); // for test

        if (strcmp(file_name, name) == 0)
        {
            add_node(&tail, filepath); // add required node
        }

        filelist_temp = filelist_temp->next;
    }
    printf("\n");
    return filelist_result->next;
}

/**
 * Function: search_by_modification_time
 * @Exec Search all the files with modification time mentioned by min 
 *          Three conditions: less than, exactly equal to, and larger than 
 * @Para1 min - the required modification time
 * @Para2 filelist - pointer to the head of the linkedlist of the filenames
 * @Return pointer to the head of the linkedlist of the searched file
 *          NULL if fails
 */
node* search_by_modification_time(char* min, node* filelist)
{
    time_t t;
    struct tm *timeinfo;
    time(&t);
    timeinfo = localtime(&t);
    struct stat buf;
    node *final_filelist;
    node *tail;
    node *temp = filelist;

    int i_min; // = atoi(min);// make minutes in char to int

    int result;
    if (filelist == NULL)
    {
        //  printf("filelist is empty.\n");
        return NULL;
    }

    create_node(&final_filelist, "");
    tail = final_filelist;

    // printf("creat new node\n");
    if (min[0] == '+')
    {
        //test
        printf("search.c min[0]+ -> before atoi token3: %s\n", min);
        i_min = atoi((min) + 1);
        printf("search.c min[0]+ -> after atoi token3: %s\n", min);
        printf("minutes after '+': %d \n", i_min);

        while ((temp->filepath) != NULL)
        {

            stat(temp->filepath, &buf);

            printf("the filepath: %s\n", temp->filepath);
            //printf("modified time %ld\n", (time(NULL) - buf.st_mtime));
            printf("modified time: %s\n", ctime(&buf.st_mtime));
            if ((time(NULL) - buf.st_mtime) >= (i_min * 60))
            {
                if (S_ISREG(buf.st_mode))
                {
                    add_node(&tail, temp->filepath);
                }
                // printf("added filepath'-': %s \n", temp->filepath);
            }
            //printf("not added filepath'-': %s \n", temp->filepath);

            temp = temp->next;
        }
        printf("\n");
        return final_filelist->next;
    }

    else if (min[0] == '-')
    {
        i_min = atoi((min) + 1);
        printf("minutes after '-': %d \n", i_min);
        while ((temp->filepath) != NULL)

        {

            result = stat(temp->filepath, &buf);
            printf("the filepath: %s\n", temp->filepath);

            printf("modified time: %s\n", ctime(&buf.st_mtime));
            if ((time(NULL) - buf.st_mtime) < (i_min * 60))
            {
                add_node(&tail, temp->filepath);
                printf("added filepath'-': %s \n", temp->filepath);
            }

            temp = temp->next;
        }
        printf("\n");
        return final_filelist->next;
    }

    else
    {
        i_min = atoi(min);
        while ((temp->filepath) != NULL)
        {

            result = stat(temp->filepath, &buf);
            printf("the filepath: %s\n", temp->filepath);
            printf("modified time %s\n", ctime(&buf.st_mtime));
            if ((int)((time(NULL) - buf.st_mtime) / 60) == (i_min))
            {
                if (S_ISREG(buf.st_mode))
                {
                    add_node(&tail, temp->filepath);
                }
                // printf("filepath'-': %s \n", temp->filepath);
            }
            temp = temp->next;
        }
        printf("\n");
        return final_filelist->next;
    }
}

/**
 * Function: search_by_inode
 * @Exec Search all the files with i-node mentioned by inum
 * @Para1 min - the required i-node
 * @Para2 filelist - pointer to the head of the linkedlist of the filenames
 * @Return pointer to the head of the linkedlist of the searched file
 *          NULL if fails
 */
node* search_by_inode(char* inum, node* filelist)
{
    node *final_filelist;
    node *temp = filelist;
    node *tail;
    // DIR *dir;

    struct stat buf;
    int result;
    //struct dirent *ptr;
    long int_num = atol(inum); // make minutes in char to int

    if (filelist == NULL)
    {
        return NULL;
    }
    // else if ((dir = opendir(filelist->filepath)) == NULL)
    // {
    //     return NULL;
    // }
    create_node(&final_filelist, "");
    tail = final_filelist;

    while (temp != NULL)
    {
        //buf = malloc(sizeof(struct stat));
        //ptr = readdir(dir);
        //result =
        stat(temp->filepath, &buf);
        printf("the current file's inode is: %ld of %s \n", buf.st_ino, temp->filepath);
        if ((long)(buf.st_ino) == int_num)
        {
            if (S_ISREG(buf.st_mode))
            {
                add_node(&tail, temp->filepath);
            }
        }
        //free(buf);
        temp = temp->next;
    }
    printf("\n");
    return final_filelist->next;
}
