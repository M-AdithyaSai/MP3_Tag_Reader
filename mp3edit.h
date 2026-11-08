#ifndef MP3EDIT_H
#define MP3EDIT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

typedef struct
{
    FILE *fptr;

    char *filename;
    char *new_value;

    char tag[5];

} MP3EditInfo;


/* File operations */
Status open_file_for_edit(MP3EditInfo *editInfo);
Status close_file_after_edit(MP3EditInfo *editInfo);


/* Editing operations */
Status edit_mp3_tag(MP3EditInfo *editInfo);
Status identify_edit_tag(char *option, MP3EditInfo *editInfo);

#endif