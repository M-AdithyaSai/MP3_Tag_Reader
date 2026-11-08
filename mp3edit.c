#include "mp3edit.h"
#include "types.h"


Status open_file_for_edit(MP3EditInfo *editInfo)
{
    //To Open the MP3 file in read and write mode
    editInfo->fptr = fopen(editInfo->filename,"r+");

    if(editInfo->fptr == NULL)
        return e_failure;


    return e_success;
}


Status close_file_after_edit(MP3EditInfo *editInfo)
{
    //To Close the MP3 file
    fclose(editInfo->fptr);
    return e_success;
}


Status identify_edit_tag(char *option, MP3EditInfo *editInfo)
{
    //To Map the command-line option to its corresponding ID3 frame identifier
    if (strcmp(option, "-t") == 0)
    {
        strcpy(editInfo->tag, "TIT2");
    }
    else if (strcmp(option, "-a") == 0)
    {
        strcpy(editInfo->tag, "TPE1");
    }
    else if (strcmp(option, "-A") == 0)
    {
        strcpy(editInfo->tag, "TALB");
    }
    else if (strcmp(option, "-y") == 0)
    {
        strcpy(editInfo->tag, "TYER");
    }
    else if (strcmp(option, "-m") == 0)
    {
        strcpy(editInfo->tag, "TCON");
    }
    else if (strcmp(option, "-c") == 0)
    {
        strcpy(editInfo->tag, "COMM");
    }
    else
    {
        return e_failure;
    }

    return e_success;
}


Status edit_mp3_tag(MP3EditInfo *editInfo)
{
    FILE *temp;
    unsigned char header[10];
    unsigned char frame_header[10];
    unsigned char size_buf[4];

    unsigned int tag_size, frame_size;
    unsigned int count = 0, new_tag_size = 0;
    int found = 0;

    char temp_file[100];


    strcpy(temp_file, "temp.mp3");


    editInfo->fptr = fopen(editInfo->filename, "r");

    if (editInfo->fptr == NULL)
        return e_failure;

    temp = fopen(temp_file, "w");

    if (temp == NULL)
        return e_failure;

    //To Read the ID3 header and extract the existing tag size
    fread(header, 1, 10, editInfo->fptr);

    tag_size = (header[6] << 21) |
               (header[7] << 14) |
               (header[8] << 7)  |
                header[9];

    fwrite(header, 1, 10, temp);

    //To Traverse the ID3 frames until the existing tag size is processed
    while (count < tag_size)
    {
        fread(frame_header, 1, 10, editInfo->fptr);
        count += 10;

        // Extracts frame size
        frame_size = (frame_header[4] << 24) |
                     (frame_header[5] << 16) |
                     (frame_header[6] << 8)  |
                      frame_header[7];

        //To Check whether the current frame matches the selected tag
        if (strncmp((char *)frame_header, editInfo->tag, 4) == 0)
        {   

            // Handle comment frames separately to preserve their additional fields
            if (strncmp((char *)frame_header, "COMM", 4) == 0)
            {
                unsigned char *data = malloc(frame_size);

                if (data == NULL){
                    fclose(temp);
                    remove(temp_file);
                    return e_failure;
                }

                //To Read the existing comment frame data
                if (fread(data, 1, frame_size, editInfo->fptr) != frame_size)
                {
                    free(data);
                    fclose(temp);
                    remove(temp_file);
                    return e_failure;
                }

                // Locate the null terminator at the end of the comment description
                unsigned int pos = 4;

                while (pos < frame_size && data[pos] != '\0')
                    pos++;

                if (pos >= frame_size)
                {
                    free(data);
                    fclose(temp);
                    remove(temp_file);
                    return e_failure;
                }

                // Calculate's the updated frame size while preserving the comment's existing fields
                unsigned int new_size = pos + 1 +
                                        strlen(editInfo->new_value);

                fwrite(frame_header, 1, 4, temp);

                //To Write the updated comment frame size
                size_buf[0] = (new_size >> 24) & 0xFF;
                size_buf[1] = (new_size >> 16) & 0xFF;
                size_buf[2] = (new_size >> 8) & 0xFF;
                size_buf[3] = new_size & 0xFF;

                fwrite(size_buf, 1, 4, temp);
                fwrite(frame_header + 8, 1, 2, temp);

                //Preserve's the existing comment description and append the new comment
                fwrite(data, 1, pos + 1, temp);
                fwrite(editInfo->new_value, 1,
                       strlen(editInfo->new_value), temp);



                new_tag_size += 10 + new_size;
                count += frame_size;
                found = 1;

                free(data);
            }
            else
            {
                // Calculate's the new text frame size, including the encoding byte
                unsigned int new_size = strlen(editInfo->new_value) + 1;

                fwrite(frame_header, 1, 4, temp);

                //To Write the updated frame size
                size_buf[0] = (new_size >> 24) & 0xFF;
                size_buf[1] = (new_size >> 16) & 0xFF;
                size_buf[2] = (new_size >> 8) & 0xFF;
                size_buf[3] = new_size & 0xFF;

                fwrite(size_buf, 1, 4, temp);
                fwrite(frame_header + 8, 1, 2, temp);

                //To Write the encoding byte and replacement text
                fputc(0, temp);
                fwrite(editInfo->new_value, 1,
                       strlen(editInfo->new_value), temp);

                // Skip's the old frame data because it is being replaced
                fseek(editInfo->fptr, frame_size, SEEK_CUR);

                count += frame_size;
                new_tag_size += 10 + new_size;
                found = 1;
            }
        }

        else
            {
                //To Copy an unchanged frame and its data to the temporary file
                fwrite(frame_header, 1, 10, temp);

                for (unsigned int i = 0; i < frame_size; i++)
                {
                    int ch = fgetc(editInfo->fptr);

                    if (ch == EOF){
                        fclose(temp);
                        remove(temp_file);
                        return e_failure;
                    }

                    fputc(ch, temp);
                }

                new_tag_size += 10 + frame_size;
            }

            count += frame_size;
        }

    // Return's failure if the requested tag was not found
    if (!found)
    {
        fclose(temp);
        remove(temp_file);
        return e_failure;
    }

    // Copies the remaining audio data without modification
    int ch;

    while ((ch = fgetc(editInfo->fptr)) != EOF)
        fputc(ch, temp);

    // Update's the ID3 header with the new tag size in synchsafe format
    size_buf[0] = (new_tag_size >> 21) & 0x7F;
    size_buf[1] = (new_tag_size >> 14) & 0x7F;
    size_buf[2] = (new_tag_size >> 7) & 0x7F;
    size_buf[3] = new_tag_size & 0x7F;

    fseek(temp, 6, SEEK_SET);
    fwrite(size_buf, 1, 4, temp);


    fclose(temp);

    // Replace's the original MP3 file with the edited temporary file
    remove(editInfo->filename);
    rename(temp_file, editInfo->filename);

    return e_success;


}