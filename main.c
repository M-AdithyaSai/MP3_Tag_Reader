/*
Description : 
Name            : M ADITHYA SAI
Roll No         : 26012_15
Project title   : MP3_Tag_Reader
Start date      : 12/10/2026
End date        : 16/10/2026
Sample input    : 
./a.out -v sample.mp3 
./a.out -e -t adithya sample.mp3

Sample output   : 

----------------SELECTED VIEW DETAILS----------------
---------------MP3 TAG READER AND EDITOR FOR ID3v2---------------
TITLE   : Red Sea :: SenSongsMp3.Com
ARTIST  : Anirudh Ravichander
ALBUM   : Devara (2024)
YEAR    : 2024
MUSIC   : Telugu
COMMENT : SenSongsMp3.Co
---------------------------------------------------------------
----------------DETAILS DISPLAYED SUCCESSFULLY----------------


----------------SELECTED EDIT DETAILS----------------
EDIT OPTION : -t
NEW VALUE   : adithya

----------------TAG CHANGED SUCCESSFULLY----------------


----------------SELECTED VIEW DETAILS----------------
---------------MP3 TAG READER AND EDITOR FOR ID3v2---------------
TITLE   : adithya
ARTIST  : Anirudh Ravichander
ALBUM   : Devara (2024)
YEAR    : 2024
MUSIC   : Telugu
COMMENT : SenSongsMp3.Co
---------------------------------------------------------------

----------------DETAILS DISPLAYED SUCCESSFULLY----------------

*/

#include <stdio.h>
#include <string.h>

#include "types.h"
#include "mp3view.h"
#include "mp3edit.h"

/* Function declarations */
OperationType check_operation_type(char *argv[]);
void print_help(void);
Status validate_view_args(int argc, char *argv[]);
Status validate_edit_args(int argc, char *argv[]);


int main(int argc, char *argv[])
{   
    // To check whether the required command-line arguments are passed
    if(argc<2){
       printf("\n");
        printf("------------------------------------------------------------\n");
        printf("ERROR: %s : INVALID ARGUMENTS\n", argv[0]);

        printf("\nUSAGE:\n");

        printf("To view please pass like:\n");
        printf("./a.out -v mp3filename\n");

        printf("\nTo edit please pass like:\n");
        printf("./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");

        printf("\nTo get help pass like:\n");
        printf("./a.out --help\n");

        printf("------------------------------------------------------------\n");
       return -1;
    }

    OperationType operation;

    // To identify the operation based on the command-line argument
    operation = check_operation_type(argv);

    if (operation == e_view)
    {
        MP3ViewInfo viewInfo;

        // To validate the arguments required for viewing MP3 details
        if (validate_view_args(argc, argv) == e_failure)
        {
            printf("ERROR: Invalid arguments\n");
            return 1;
        }

        // To store the MP3 filename
        viewInfo.filename = argv[2];

        printf("\n");
        printf("----------------SELECTED VIEW DETAILS----------------\n");

        // To open the MP3 file
        if (open_mp3_file(&viewInfo) == e_failure)
        {
            printf("ERROR: Unable to open MP3 file\n");
            return 1;
        }

        // To check whether the file contains a valid ID3 header
        if (check_id3_header(&viewInfo) == e_failure)
        {
            printf("ERROR: Invalid ID3v2 MP3 file\n");
            close_mp3_file(&viewInfo);
            return 1;
        }

        // To read the metadata tags from the MP3 file
        if (read_mp3_tags(&viewInfo) == e_failure)
        {
            printf("ERROR: Unable to read MP3 tags\n");
            close_mp3_file(&viewInfo);
            return 1;
        }

        // To display the extracted MP3 details
        display_mp3_info(&viewInfo);

        // To close the MP3 file
        close_mp3_file(&viewInfo);

        printf("\n----------------DETAILS DISPLAYED SUCCESSFULLY----------------\n");
    }


    else if (operation == e_edit)
    {
        MP3EditInfo editInfo;

        // To validate the arguments required for editing MP3 tags
        if (validate_edit_args(argc, argv) == e_failure)
        {
            printf("ERROR: Invalid edit arguments\n");
            return 1;
        }

        // To store the new tag value and MP3 filename
        editInfo.new_value = argv[3];
        editInfo.filename = argv[4];

        // To identify the tag that needs to be edited
        if (identify_edit_tag(argv[2], &editInfo) == e_failure)
        {
            printf("ERROR: Invalid edit option\n");
            return 1;
        }

        printf("\n");
        printf("----------------SELECTED EDIT DETAILS----------------\n");

        printf("EDIT OPTION : %s\n", argv[2]);
        printf("NEW VALUE   : %s\n", argv[3]);

        // To open the MP3 file for editing
        if (open_file_for_edit(&editInfo) == e_failure)
        {
            printf("ERROR: Unable to open MP3 file\n");
            return 1;
        }

        // To update the selected tag with the new value
        if (edit_mp3_tag(&editInfo) == e_failure)
        {
            printf("ERROR: Failed to edit MP3 tag\n");
            close_file_after_edit(&editInfo);
            return 1;
        }

        // To close the MP3 file after editing
        close_file_after_edit(&editInfo);

        printf("\n----------------TAG CHANGED SUCCESSFULLY----------------\n");
    }


    else if (operation == e_help)
    {
        // To display the help menu and available commands
        print_help();
    }


    else
    {
        // To display the usage instructions for an unsupported operation
        printf("\n");
        printf("------------------------------------------------------------\n");
        printf("ERROR: %s : INVALID ARGUMENTS\n", argv[0]);

        printf("\nUSAGE:\n");

        printf("To view please pass like:\n");
        printf("./a.out -v mp3filename\n");

        printf("\nTo edit please pass like:\n");
        printf("./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");

        printf("\nTo get help pass like:\n");
        printf("./a.out --help\n");

        printf("------------------------------------------------------------\n");
    }

    return 0;
}


/* --------------------------------------------------------- */

// To identify whether the user selected view, edit or help operation
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
    {
        return e_view;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        return e_edit;
    }

    if (strcmp(argv[1], "--help") == 0)
    {
        return e_help;
    }

    // To return unsupported when the operation does not match any valid option
    return e_unsupported;
}


/* --------------------------------------------------------- */

// To validate the command-line arguments provided for viewing MP3 details
Status validate_view_args(int argc, char *argv[])
{
    // To check whether the correct number of arguments is passed
    if (argc != 3)
    {   
        printf("\n");
        printf("------------------------------------------------------------\n");
        printf("ERROR: %s %s : INVALID ARGUMENTS\n", argv[0],argv[1]);

        printf("\nUSAGE:\n");

        printf("To view please pass like:\n");
        printf("./a.out -v mp3filename\n");

        printf("------------------------------------------------------------\n");
        return e_failure;
    }

    // To check whether the selected operation is view
    if (strcmp(argv[1], "-v") != 0)
    {
        return e_failure;
    }

    // To check whether the filename contains the .mp3 extension
    if (strstr(argv[2], ".mp3") == NULL)
    {
        return e_failure;
    }

    return e_success;
}


/* --------------------------------------------------------- */

// To validate the command-line arguments provided for editing MP3 tags
Status validate_edit_args(int argc, char *argv[])
{
    // To check whether the correct number of arguments is passed
    if (argc != 5)
    {   
        printf("\n");
        printf("------------------------------------------------------------\n");
        printf("ERROR: %s %s : INVALID ARGUMENTS\n", argv[0],argv[1]);

        printf("\nUSAGE:\n");
        printf("\nTo edit please pass like:\n");
        printf("./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");

        printf("------------------------------------------------------------\n");
        return e_failure;
    }

    // To check whether the selected operation is edit
    if (strcmp(argv[1], "-e") != 0)
    {
        return e_failure;
    }

    // To check whether the tag option is one of the supported options
    if (strcmp(argv[2], "-t") != 0 &&
        strcmp(argv[2], "-a") != 0 &&
        strcmp(argv[2], "-A") != 0 &&
        strcmp(argv[2], "-m") != 0 &&
        strcmp(argv[2], "-y") != 0 &&
        strcmp(argv[2], "-c") != 0)
    {
        return e_failure;
    }

    // To check whether the filename contains the .mp3 extension
    if (strstr(argv[4], ".mp3") == NULL)
    {
        return e_failure;
    }

    return e_success;
}


/* --------------------------------------------------------- */

// To display the help menu with the available operations and tag options
void print_help(void)
{
    printf("\n");
    printf("----------------------------HELP MENU----------------------------\n");

    printf("\n1. -v -> to view mp3 file contents\n");

    printf("\n2. -e -> to edit mp3 file contents\n");

    printf("\n");
    printf("    2.1. -t -> to edit song title\n");
    printf("    2.2. -a -> to edit artist name\n");
    printf("    2.3. -A -> to edit album name\n");
    printf("    2.4. -y -> to edit year\n");
    printf("    2.5. -m -> to edit content/music\n");
    printf("    2.6. -c -> to edit comment\n");

    printf("\n------------------------------------------------------------------\n");
}