#include "stringutils.h"

int kmp_contains(char *string, char *pattern){
    int stringlen = strlen(string);
    int ptlen = strlen(pattern); 
    int j = 0;
    for(int i=0;i<stringlen;i++){
        if(string[i]==pattern[j]){
            j++;
            if(j==ptlen){
                return 1;
            }
        }
        else{
            j = 0;
            if(string[i]==pattern[j]){
                j++;
            }
        }
    }
    return 0;
}