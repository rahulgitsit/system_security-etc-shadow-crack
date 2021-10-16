#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
    double exec_time = 0.0;
    clock_t start = clock();

    FILE *fp_wordlist, *fp_wordlist_hash, *fp_passwordfile, *fp_wordlist_cracked;
    char *word_list, *passwordfile, *word_hash, *salt = "$1$GC$"; // salt is identified from the password hashes
    ssize_t read_wordlist, read_pass;
    size_t len_w = 0, len_p = 0;
    char *line_w, *line_p = NULL;

    // Check to see if both the files are given in command line
    if (argc < 3)
    {
        printf("Missing files\n");
        return 1;
    }

    else
    {
        passwordfile = argv[1];
        word_list = argv[2];
    }

    fp_wordlist = fopen(word_list, "r"); // TODO:not required after the first while loop
    fp_wordlist_hash = fopen("hashed250.txt", "w");
    fp_wordlist_cracked = fopen("allcrackedpasswords.txt", "w");

    // reading the wordlist line by line
    while ((read_wordlist = getline(&line_w, &len_w, fp_wordlist)) != -1)
    {

        // retreiving the password from the top250 file. strrchr to retrieve the last character; here tab
        char *last_tab = strrchr(line_w, '\t');
        last_tab[strlen(last_tab) - 1] = '\0';
        last_tab = last_tab + 1;
        // hashing the top250 passwords along with the salt and storing it in the hashed250.txt file + the original plain text
        word_hash = crypt(last_tab, salt);
        fprintf(fp_wordlist_hash, "%s", word_hash);
        fprintf(fp_wordlist_hash, ":%s\n", last_tab);
    }

    fclose(fp_wordlist);
    fclose(fp_wordlist_hash);

    char *passwordfile_username, *wordlist_password, *passwordfile_hash, *wordlist_hash;

    int counter = 0;
    fp_wordlist_hash = fopen("hashed250.txt", "r");
    fprintf(fp_wordlist_cracked, "%s", "Username:\t\tPassword:\n");
    fprintf(fp_wordlist_cracked, "%s", "______________________\n");

    // reading the passwordfile line by line
    while ((read_wordlist = getline(&line_w, &len_w, fp_wordlist_hash)) != -1)
    {
        wordlist_hash = strtok(line_w, ":");
        wordlist_password = strtok(NULL, "\t");
        fp_passwordfile = fopen(passwordfile, "r");

        while ((read_pass = getline(&line_p, &len_p, fp_passwordfile)) != -1)
        {
            passwordfile_username = strtok(line_p, ":"); // username from passwordfile.txt
            passwordfile_hash = strtok(NULL, ":");       // to get the password hash

            // Comparing the hashes of both passwordfile and wordlist
            if (strcmp(wordlist_hash, passwordfile_hash) == 0)
            {
                counter += 1;
                fprintf(fp_wordlist_cracked, "%d. %s : ", counter, passwordfile_username);
                fprintf(fp_wordlist_cracked, "%s", wordlist_password);
            }
        }
    }

    // while ((read_wordlist = getline(&line_w, &len_w, fp_wordlist_cracked)) != -1)
    // {
    //     fprintf(stdout, "%s", line_w);
    // }

    fclose(fp_wordlist_hash);
    fclose(fp_passwordfile);

    clock_t end = clock();
    exec_time += (double)(end - start) / CLOCKS_PER_SEC;

    fprintf(fp_wordlist_cracked, "\nNo. of passwords cracked: %d\n", counter);
    fprintf(fp_wordlist_cracked, "The program got executed in %f seconds\n", exec_time);
    fclose(fp_wordlist_cracked);
}
