#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fp_passwd, *fp_shadow, *fp_text;
    char *passwd, *shadow;
    char *line_p = NULL, *line_s = NULL;
    size_t len_p = 0, len_s = 0;
    ssize_t read_passwd, read_shadow;

    //Checking if the files are not given along with the command line
    if (argc < 3)
    {
        printf("Missing files\n");
        return 1;
    }

    else
    {
        passwd = argv[1];
        shadow = argv[2];
    }

    //Opening all the files (etc/passwd,etc/shadow and the combined file)
    fp_passwd = fopen(passwd, "r");
    fp_shadow = fopen(shadow, "r");
    fp_text = fopen("passwordfile.txt", "w");

    //Checking if the files are present
    if (!fp_passwd && !fp_shadow)
    {
        fprintf(stderr, "Failed to open the file");
        return 1;
    }

    //Reading the contents of both the files line by line
    while ((read_passwd = getline(&line_p, &len_p, fp_passwd)) != -1 && (read_shadow = getline(&line_s, &len_s, fp_shadow)) != -1)
    {
        char *first_token, *second_token, *temp, *x_pos;
        char *new_line = (char *)malloc(1024);

        //Finding the first occuerence of ':'
        x_pos = strchr(line_p, ':');
        temp = x_pos;
        temp += 2;

        strncpy(new_line, line_p, x_pos - line_p);

        //Fetching the hashed cypherkey from shadow file
        first_token = strtok(line_s, ":");
        second_token = strtok(NULL, ":");

        //Concatenating the required strings
        strcat(new_line, ":");
        strcat(new_line, second_token);
        strcat(new_line, temp);
        printf("%s", new_line);
        //printf("Hash from shadow file: %s\n", second_token);
        fprintf(fp_text, "%s", new_line);
    }
    fclose(fp_passwd);
    fclose(fp_shadow);
    fclose(fp_text);
}