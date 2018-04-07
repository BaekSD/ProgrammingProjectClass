#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLANK ' '

void rm_blank(char *str);
void rm_CAL(char *str, char *word);
char* input(void);

void rm_blank(char *str)
{
    char *cpy = (char*)malloc(500 * sizeof(char));
    int i, k = 0;
    
    //str 길이만큼 돌려서 공백이 아니면 cpy에 복사
    for(i = 0; i < strlen(str); i++) {
        if(*(str + i) != BLANK) {
            *(cpy + k) = *(str + i);
            k++;
        }
    }
    //공백 없앤 다음에 다시 str에 복사
    strcpy(str, cpy);
}

char* input(void)
{
    char *input = NULL;
    char *line = NULL;
    char *check = NULL;
    char *check_exit = NULL;
    char *exit = "EXIT";
    
    input = (char*)malloc(200 * sizeof(char));
    line = (char*)malloc(500 * sizeof(char));
    
    while(1)
    {
        //엔터 치기 전까지 input 받기
        gets(input);
        
        //input 받은 문자열의 공백 제거
        rm_blank(input);
        
        strcat(line, input);
        
        check_exit = strstr(line, "EXIT");
        check = strstr(line, "CAL");
        if(check_exit != NULL && check != NULL) {
            if(check_exit < check)
                return exit;
            else if(check_exit > check)
                break;
        }
        
        if(check_exit != NULL)
            return exit;
        
        //input에 CAL 있는지 확인하고 CAL 있으면 break
        if(check != NULL)
            break;
    }
    
    rm_CAL(line, check);
    
    
    //line을 반환
    return line;
}

void rm_CAL(char *str, char *word)
{
    char *cpy = NULL;
    int length = strlen(str) - strlen(word);
    int i, k = 0;
    
    cpy = (char*)malloc(100 * sizeof(char));
    
    for(i = 0; i < length; i++) {
        *(cpy + k) = *(str + i);
        k++;
    }
    *(cpy + k) = '\0';
    strcpy(str, cpy);
}

int main() {
    char* str;
    
    while(1) {
        str = input();
        printf("%s\n",str);
    }
}