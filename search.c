#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <curl/curl.h>
/* For older cURL versions you will also need 
#include <curl/types.h>
#include <curl/easy.h>
*/
#include <string.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void write_webpage_to_file(char *outfilename, char *url) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

void get_links_from_html(char *outfilename, char **links) {
    FILE *fp;

    int i, j;
    int scount = 0;
    char buff[255];
    char search[255], search2[255];

    fp = fopen(outfilename, "r");

    while(fgets(buff, 255, (FILE*)fp)) {
        for (i=0;i<255;i++) {
            if (buff[i]=='<') {
                i++;
                if (buff[i]=='a') {
                    while(buff[i]!='=') i++; i+=2;
                    if (buff[i]=='n') {
                        j = 0;
                        while (buff[i]!='"') {
                            search2[j] = buff[i];
                            i++; j++;
                        }
                        strcat(search, "http://web.eecs.utk.edu/~bvz/cs365/");
                        strcat(search, search2);
                        links[scount] = malloc(sizeof(char)*255);
                        strcpy(links[scount], search); scount++;
                        for(j=0;j<255;j++) {
                            search2[j]=0;
                        }
                        for(j=0;j<255;j++) {
                            search[j]=0;
                        }

                    }
                }
            }
        }
    }
}

int get_num_links(char **searches) {
    int numlinks = 0;
    while(searches[numlinks]!=0) {
        numlinks++;
    }
    return numlinks;
}

void get_filenames(char **filenames, char **searches) {
    int numlinks=0;
    while(searches[numlinks]!=0) {
        numlinks++;
    }

    int i, j;
    int flength = 0;
    char filename[255];
    for(i=0;i<255;i++) filename[i]=0;
    for (j=0;j<numlinks;j++) {
        for (i=strlen(searches[j])-1;i>=0;i--) {
            filename[i] = searches[j][i];
        }
        char tmp;
        for (i=0;i<strlen(filename)/2;i++) {
            tmp = filename[i];
            filename[i] = filename[strlen(filename)-1-i];
            filename[strlen(filename)-1-i] = tmp;
        }
        for (i=0;i<strlen(filename)-1;i++) {
            if(filename[i]=='/') filename[i] = 0;
            flength = i;
        }
        for (i=flength;i<255;i++) {
            filename[i]=0;
        }
        for (i=0;i<flength/2;i++) {
            tmp = filename[i];
            filename[i] = filename[flength-1-i];
            filename[flength-1-i] = tmp;
        }
        filenames[j] = malloc(sizeof(char)*255);
        strcpy(filenames[j], filename);
        for (i=0;i<255;i++) {
            filename[i]=0;
        }
    }
}

void write_links_to_files(char **filenames, char **searches) {
    DIR *dir = opendir("files");
    if (!dir) system("mkdir files");
    char *outfilename = malloc(sizeof(char)*255);
    int numlinks = get_num_links(searches);
    int i, j;
    for (i=0;i<numlinks;i++) {
        strcat(outfilename, "files/");
        strcat(outfilename, filenames[i]);
        printf("%s\n", outfilename);
        write_webpage_to_file(outfilename, searches[i]);
        for (j=0; j<255;j++) outfilename[j] = 0;
    }
}

int main(void) {
    char cmd[255];
    //write_webpage_to_file(outfilename, "http://web.eecs.utk.edu/~bvz/cs365/lecture_notes.html");

    char **searches = malloc(sizeof(char *)*255);
    get_links_from_html("output.html", searches);

    char **filenames = malloc(sizeof(char *)*255);
    get_filenames(filenames, searches);

    write_links_to_files(filenames, searches);

    int i, j, k;
    FILE *fp;
    char buf[10000];
    char outfile[255];
    char *word_searched = "finalize";
    char *tmp = malloc(sizeof(char)*strlen(word_searched));
    for (i=0;i<get_num_links(searches);i++) {
        strcat(outfile, "files/");
        strcat(outfile, filenames[i]);
        fp = fopen(outfile, "r");
        while (fgets(buf, 10000, (FILE*)fp)) {
            for(j=0;j<10000;j++) {
                if (buf[j]==word_searched[0]) {
                    printf("%s\n", word_searched);
                }
            }
        }
    }

    return 0;
}
