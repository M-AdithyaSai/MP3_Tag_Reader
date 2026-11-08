#ifndef MP3VIEW_H
#define MP3VIEW_H

#include <stdio.h>
#include "types.h"

typedef struct
{
    FILE *fptr;
    char *filename;

    char title[100];
    char artist[100];
    char album[100];
    char year[20];
    char music[100];
    char comment[200];

} MP3ViewInfo;


/* File operations */
Status open_mp3_file(MP3ViewInfo *viewInfo);
Status close_mp3_file(MP3ViewInfo *viewInfo);

/* ID3v2 operations */
Status check_id3_header(MP3ViewInfo *viewInfo);
Status read_mp3_tags(MP3ViewInfo *viewInfo);

/* Read individual tags */
Status frame_data(char *tag,MP3ViewInfo *viewInfo,char *info);
Status read_comment(MP3ViewInfo *viewInfo);

/* Display */
void display_mp3_info(MP3ViewInfo *viewInfo);

#endif