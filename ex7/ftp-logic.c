#include<stdio.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>




#define BUFFER_SIZE 1024
bool found = false;
char *dest_path;

void ls_recursive(char *bpath, char *file);
void print_file_contents(char *file_path);


int main()
{
    char buffer[30] = "hello.txt";
    char path[100] = "/home/yash";

    // Allocate memory for dest_path
    dest_path = (char *)malloc(2 * 1000);  // Assuming a maximum path length of 1000 characters

    if (dest_path == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    ls_recursive(path, buffer);

    // Check if the file was found
    if (found) {
        printf("Path to file: %s\n", dest_path);
        print_file_contents(dest_path);
    } else {
        printf("File not found.\n");
    }

    // Don't forget to free the allocated memory
    free(dest_path);



    return 0;
}


void ls_recursive(char *bpath, char *file) {
  char path[1000];
  struct dirent *dir_pointer;
  DIR *dir_stream = opendir(bpath);
  if (!dir_stream)
    return;

  while ((dir_pointer = readdir(dir_stream)) != NULL) {
    //printf("Name : %s type : %d\n", dir_pointer->d_name, dir_pointer->d_type);
    if (strcmp(dir_pointer->d_name, ".") != 0 &&
        strcmp(dir_pointer->d_name, "..") != 0) {
      if (strcmp(file, dir_pointer->d_name) == 0) {
        // printf("%s\n", dir_pointer ->d_name);
        // printf("%s\n",bpath);
        strcpy(dest_path, bpath);
        strcat(dest_path, "/");
        strcat(dest_path, dir_pointer->d_name);
        found = true;
        return;
      }
      strcpy(path, bpath);
      strcat(path, "/");
      strcat(path, dir_pointer->d_name);

      ls_recursive(path, file);
    }
  }

  closedir(dir_stream);
}

void print_file_contents(char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}