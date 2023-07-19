#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_LINE 1024

int main(int argc, char *argv[]) {

    // checks if the no arguments are given then print out stdin reversed into stdout
    if (argc == 1) {
        int lineCount = 0;
        int charCount = 0;
        char character;
        char **lines;

        // Dynamically allocating memory to lines for reading the contents of the stdin and making an error exit in case it fails
        if ((lines = malloc(sizeof(char *) * BUFFER_LINE)) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        } 

        // Reading stdin character by character until the end of the file or error occurs
        while ((character = fgetc(stdin)) != EOF) {
            if (ferror(stdin)) {
                fprintf(stderr, "error: while reading stdin\n");
                exit(1);
            }

            // Dynamically allocating memory for characters and making an error exit in case it fails
            if (charCount == 0) {
                if ((lines[lineCount] = malloc(BUFFER_LINE)) == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    exit(1);
                } 
            }

            lines[lineCount][charCount] = character; // Storing character in a dynamically allocated 2D Array
            charCount++; // Add character count

            // if character hits linebreak character then reallocate memory to next row and reset character count to 0
            if (character == '\n') {
                lines[lineCount] = realloc(lines[lineCount], charCount + 1);
                lines[lineCount][charCount] = '\0';
                lineCount++;
                charCount = 0;

                // This checks if lineCount has reached allocated Buffer then its reallocates more memory for the lines if it has reached it
                if (lineCount % BUFFER_LINE == 0) {
                    int newBuffer = lineCount + BUFFER_LINE;
                    lines = realloc(lines, sizeof(char *) * newBuffer);
                }
            }

            // This checks if charCount has reached allocated Buffer then its reallocates more memory for the characters if it has reached it
            else if (charCount % BUFFER_LINE == 0) {
                int newBuffer = charCount + BUFFER_LINE;
                lines[lineCount] = realloc(lines[lineCount], newBuffer);
            }
        }

        // Dynamically reallocates memory for null pointer and makes sure the last line is properly null terminated
        if (charCount != 0) {
            lines[lineCount] = realloc(lines[lineCount], charCount + 1);
            lines[lineCount][charCount] = '\0';
            lineCount++;
        }
        lines = realloc(lines, sizeof(char *) * lineCount);

         // Prints all the lines in reverse order to the stdout
        for (int i = lineCount-1; i > -1; i--) {
            fprintf(stdout, "%s", lines[i]);
        }

        // Freeing all the dynamically stored memory in 2D array
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        free(lines); // Frees lines array
    }
    
    // checks if the input file argument is given then print out the file reversed to standart output (stdout)
    else if (argc == 2) {
        FILE *fileRead = fopen(argv[1], "r"); // Opens file in read

        // Checks if file was succesfully opened and gives error exit in case it fails
        if (fileRead == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        int lineCount = 0;
        int charCount = 0;
        char character;
        char **lines;

        // Dynamically allocating memory to read the contents of the input file and making an error exit in case it fails
        if ((lines = malloc(sizeof(char *) * BUFFER_LINE)) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        // Reading file character by character until the end of the file or error occurs
        while ((character = fgetc(fileRead)) != EOF) {
            if (ferror(fileRead)) {
                fprintf(stderr, "error: while reading file\n");
                exit(1);
            }  

            // Dynamically allocating memory for characters and making an error exit in case it fails
            if (charCount == 0) {
                if ((lines[lineCount] = malloc(BUFFER_LINE)) == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    exit(1);
                } 
            }

            lines[lineCount][charCount] = character; // Storing character in a dynamically allocated 2D Array
            charCount++; // Add character count

            // if character hits linebreak character then reallocate memory to next row and reset character count to 0
            if (character == '\n') { 
                lines[lineCount] = realloc(lines[lineCount], charCount + 1);
                lines[lineCount][charCount] = '\0';
                lineCount++;
                charCount = 0;

                // This checks if lineCount has reached allocated Buffer then its reallocates more memory for the lines if it has reached it
                if (lineCount % BUFFER_LINE == 0) {
                    int newBuffer = lineCount + BUFFER_LINE;
                    lines = realloc(lines, sizeof(char *) * newBuffer);
                } 
            }

            // This checks if charCount has reached allocated Buffer then its reallocates more memory for the characters if it has reached it
            else if (charCount % BUFFER_LINE == 0) {
                int newBuffer = charCount + BUFFER_LINE;
                lines[lineCount] = realloc(lines[lineCount], newBuffer);
            } 
        }

        // Dynamically reallocates memory for null pointer and makes sure the last line is properly null terminated
        if (charCount != 0) {
            lines[lineCount] = realloc(lines[lineCount], charCount + 1);
            lines[lineCount][charCount] = '\0';
            lineCount++;
        } 
        lines = realloc(lines, sizeof(char *) * lineCount);

        // Prints all the lines in reverse order to the stdout
        for (int i = lineCount-1; i > -1; i--) {
            fprintf(stdout, "%s", lines[i]);
        } 

        // Freeing all the dynamically stored memory in 2D array
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        free(lines); // Frees lines array
        fclose(fileRead); // Closes the input file
    }

    // checks if the input and output file arguments are given then print out the input file contents in reversed order to the output file
    else if (argc == 3) {

        // checking if the input file and output file differ
        struct stat file1, file2;
        lstat(argv[1], &file1);
        lstat(argv[2], &file2);
        
        // cheking if inode numbers are the same meaning that they do not differ and are hardlinked
        if (file1.st_ino == file2.st_ino) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        // Checks if input file was opened succesfully and gives error exit in case it fails
        FILE *fileRead = fopen(argv[1], "r"); // Opens input file in read
        if (fileRead == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        // Checks if output file was opened succesfully and gives error exit in case it fails
        FILE *fileWrite = fopen(argv[2], "w"); // Opens output file in write
        if (fileWrite == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
        int lineCount = 0;
        int charCount = 0;
        char character;
        char **lines;

        // Dynamically allocates memory to lines and makes error exit in case it fails
        if ((lines = malloc(sizeof(char *) * BUFFER_LINE)) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        // Reading file character by character until the end of the file or error occurs 
        while ((character = fgetc(fileRead)) != EOF) {
            if (ferror(fileRead)) {
                fprintf(stderr, "error: while reading file\n");
                exit(1);
            }

            // Dynamically allocating memory for characters and making an error exit in case it fails
            if (charCount == 0) {
                if ((lines[lineCount] = malloc(BUFFER_LINE)) == NULL) {
                    fprintf(stderr, "malloc failed\n");
                    exit(1);
                }
            }

            lines[lineCount][charCount] = character; // Stores character in dynamically allocated 2D Array
            charCount++; // Add character count

            // Checks if character hits linebreak then reallocate memory for null pointer and go the next line
            if (character == '\n') {
                lines[lineCount] = realloc(lines[lineCount], charCount + 1);
                lines[lineCount][charCount] = '\0';
                lineCount++;
                charCount = 0;

                // This checks if lineCount has reached allocated Buffer then its reallocates more memory for the lines if it has reached it
                if (lineCount % BUFFER_LINE == 0) {
                    int newBuffer = lineCount + BUFFER_LINE;
                    lines = realloc(lines, sizeof(char *) * newBuffer);
                }
            }

            // This checks if charCount has reached allocated Buffer then its reallocates more memory for the characters if it has reached it
            else if (charCount % BUFFER_LINE == 0) {
                int newBuffer = charCount + BUFFER_LINE;
                lines[lineCount] = realloc(lines[lineCount], newBuffer);
            }
        }

        // Dynamically reallocates memory for null pointer and makes sure the last line is properly null terminated
        if (charCount != 0) {
            lines[lineCount] = realloc(lines[lineCount], charCount + 1);
            lines[lineCount][charCount] = '\0';
            lineCount++;
        }
        lines = realloc(lines, sizeof(char *) * lineCount);

        // Prints all the lines in reverse order to the stdout
        for (int i = lineCount-1; i > -1; i--) {
            fprintf(fileWrite, "%s", lines[i]);
        }

        // Freeing all the dynamically stored memory in 2D array
        for (int i = 0; i < lineCount; i++) {
            free(lines[i]);
        }
        free(lines); // Frees lines array
        fclose(fileRead); // Closes input file
        fclose(fileWrite); // Closes output file
    }

    // if user gives too many or too few arguments print to the standart output (stdout) instructions on how to use the program and exit with return code 1
    else {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    return 0;
}

