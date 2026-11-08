#include <stdio.h>
#include <string.h>

#include "mp3view.h"
#include "types.h"

// To open the MP3 file in read mode
Status open_mp3_file(MP3ViewInfo *viewInfo)
{
    viewInfo->fptr = fopen(viewInfo->filename, "r");

    if (viewInfo->fptr == NULL)
        return e_failure;

    return e_success;
}

// To close the opened MP3 file
Status close_mp3_file(MP3ViewInfo *viewInfo)
{
    fclose(viewInfo->fptr);
    return e_success;
}

// To check whether the file contains a valid ID3 header
Status check_id3_header(MP3ViewInfo *viewInfo)
{
    char buffer[4];

    // To move the file pointer to the beginning of the file
    rewind(viewInfo->fptr);

    // To read the first three bytes of the file
    if (fread(buffer, 1, 3, viewInfo->fptr) != 3)
        return e_failure;

    buffer[3] = '\0';

    // To verify whether the header contains the ID3 identifier
    if (strcmp(buffer, "ID3") != 0)
        return e_failure;

    // To skip the remaining seven bytes of the ID3 header
    fseek(viewInfo->fptr, 7, SEEK_CUR);

    return e_success;
}

// To read the required metadata tags from the MP3 file
Status read_mp3_tags(MP3ViewInfo *viewInfo)
{
    char buffer[5];

    // To read the five text frames one by one
    for (int i = 0; i < 5; i++)
    {
        if (fread(buffer, 1, 4, viewInfo->fptr) != 4)
            return e_failure;

        buffer[4] = '\0';

        // To identify the title frame and read its data
        if (strcmp(buffer, "TIT2") == 0)
        {
            fseek(viewInfo->fptr, -4, SEEK_CUR);

            if (frame_data("TIT2", viewInfo, viewInfo->title) == e_failure)
                return e_failure;
        }
        // To identify the artist frame and read its data
        else if (strcmp(buffer, "TPE1") == 0)
        {
            fseek(viewInfo->fptr, -4, SEEK_CUR);

            if (frame_data("TPE1", viewInfo, viewInfo->artist) == e_failure)
                return e_failure;
        }
        // To identify the album frame and read its data
        else if (strcmp(buffer, "TALB") == 0)
        {
            fseek(viewInfo->fptr, -4, SEEK_CUR);

            if (frame_data("TALB", viewInfo, viewInfo->album) == e_failure)
                return e_failure;
        }
        // To identify the year frame and read its data
        else if (strcmp(buffer, "TYER") == 0)
        {
            fseek(viewInfo->fptr, -4, SEEK_CUR);

            if (frame_data("TYER", viewInfo, viewInfo->year) == e_failure)
                return e_failure;
        }
        // To identify the music genre frame and read its data
        else if (strcmp(buffer, "TCON") == 0)
        {
            fseek(viewInfo->fptr, -4, SEEK_CUR);

            if (frame_data("TCON", viewInfo, viewInfo->music) == e_failure)
                return e_failure;
        }
        else
        {
            return e_failure;
        }
    }

    // To read the comment frame after reading the five text frames
    if (read_comment(viewInfo) == e_failure)
    {
        printf("Error: Reading comment failed\n");
        return e_failure;
    }

    return e_success;
}

// To read the comment frame from the MP3 file
Status read_comment(MP3ViewInfo *viewInfo)
{
    // To read the comment frame identifier
    char buffer[5];

    if (fread(buffer, 1, 4, viewInfo->fptr) != 4)
    {
        return e_failure;
    }

    buffer[4] = '\0';

    // To verify whether the frame is a comment frame
    if (strcmp(buffer, "COMM") != 0)
    {
        return e_failure;
    }

    // To read the four-byte comment frame size
    unsigned char size_buffer[4];

    if (fread(size_buffer, 1, 4, viewInfo->fptr) != 4)
    {
        return e_failure;
    }

    // To calculate the comment frame size from its four bytes
    unsigned int size = ((unsigned int)size_buffer[0] << 24) |
                        ((unsigned int)size_buffer[1] << 16) |
                        ((unsigned int)size_buffer[2] << 8) |
                        size_buffer[3];

    if (size < 5)
    {
        return e_failure;
    }

    // To skip the two-byte frame flags
    if (fseek(viewInfo->fptr, 2, SEEK_CUR) != 0)
    {
        return e_failure;
    }

    // To skip the text encoding byte
    fgetc(viewInfo->fptr);

    // To skip the three-byte language field
    if (fseek(viewInfo->fptr, 3, SEEK_CUR) != 0)
    {
        return e_failure;
    }

    // To allocate a buffer for the comment frame data
    char data[size + 1];

    // To read the description and comment data from the frame
    if (fread(data, 1, size - 4, viewInfo->fptr) != size - 4)
    {
        return e_failure;
    }

    data[size - 4] = '\0';

    // To locate the null terminator separating the description from the comment
    char *comment = strchr(data, '\0');

    if (comment == NULL || *(comment + 1) == '\0')
    {
        return e_failure;
    }

    // To copy the comment text into the structure
    strcpy(viewInfo->comment, comment + 1);

    return e_success;
}

// To read the size and data of the specified ID3 frame
Status frame_data(char *tag, MP3ViewInfo *viewInfo, char *info)
{
    char buffer[5];

    // To read the frame identifier
    if (fread(buffer, sizeof(char), 4, viewInfo->fptr) != 4)
    {
        return e_failure;
    }

    buffer[4] = '\0';

    // To check whether the frame identifier matches the requested tag
    if (strcmp(buffer, tag) != 0)
        return e_failure;

    // To read the four-byte frame size
    unsigned char size_buffer[4];

    if (fread(size_buffer, sizeof(char), 4, viewInfo->fptr) != 4)
    {
        return e_failure;
    }

    // To calculate the frame data size from its four bytes
    unsigned int size = (size_buffer[0] << 24) |
                        (size_buffer[1] << 16) |
                        (size_buffer[2] << 8) |
                        size_buffer[3];

    if (size <= 0)
        return e_failure;

    // To allocate a buffer for the frame data
    char data[size];

    // To skip the two-byte frame flags and the encoding byte
    fseek(viewInfo->fptr, 2, SEEK_CUR);
    fgetc(viewInfo->fptr);

    // To read the frame text without the encoding byte
    if (fread(data, sizeof(char), size - 1, viewInfo->fptr) != size - 1)
    {
        return e_failure;
    }

    data[size - 1] = '\0';

    // To copy the extracted frame text into the corresponding structure member
    strcpy(info, data);

    return e_success;
}

// To display all the extracted MP3 metadata
void display_mp3_info(MP3ViewInfo *viewInfo)
{
    printf("\n");
    printf("---------------MP3 TAG READER AND EDITOR FOR ID3v2---------------\n");

    printf("TITLE   : %s\n", viewInfo->title);
    printf("ARTIST  : %s\n", viewInfo->artist);
    printf("ALBUM   : %s\n", viewInfo->album);
    printf("YEAR    : %s\n", viewInfo->year);
    printf("MUSIC   : %s\n", viewInfo->music);
    printf("COMMENT : %s\n", viewInfo->comment);

    printf("---------------------------------------------------------------\n");
}