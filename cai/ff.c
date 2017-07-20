#define _XOPEN_SOURCE 700

#include <stdio.h>

int main ()
{
char* new_str;
char* chemin = "wfew";
				char* str2 = "/shm_bar";
				if((new_str = malloc(strlen(chemin)+strlen(str2)+1)) != NULL){
					new_str[0] = '\0';   // ensures the memory is an empty string
					strcat(new_str,chemin);
					strcat(new_str,str2);
				} 
				printf("dwq %s\n",new_str);

   return 0;
}
