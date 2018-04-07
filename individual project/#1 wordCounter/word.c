#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

char* regular;              //입력받은 정규표현식을 저장
char *regular_sep[20];		//regular expression을 *을 기준으로 나누어 저장할 공간, 최대 20개라고 가정
int regular_sep_count;
int count_star=0;           //*의 갯수. 0개이면 문자랑 같은지만 비교하면됨.

int compareWord(char word_tmp[]) {
    int i,j,word_len;
    
    if(regular_sep_count == 0)
        return 1;
    else if(count_star == 0) {
        if(strcmp(regular, word_tmp) == 0)
            return 1;
    }
    else if(regular[0] != '*' && strstr(word_tmp, regular_sep[0]) != word_tmp){ return 0; }
    //첫 문자가 *이 아닌경우, *로 분리한 첫 문자열이 처음에 나오는지 검사
    //마지막 문자가 *이 아닌경우, *로 분리한 마지막 문자열이 마지막에 나오는지 검사
    else if(regular[strlen(regular)-1] != '*' &&
            strstr(word_tmp+strlen(word_tmp)-strlen(regular_sep[regular_sep_count-1]),
                   regular_sep[regular_sep_count-1]) == NULL){ return 0; }
    else {
        i=0;
        word_len = strlen(word_tmp);
        j=word_len;
        while(regular_sep[i] != NULL) {		//분리된 문자열을 하나씩 선택해서 단어에 있는지 반복
            if(strstr(word_tmp+word_len-j,regular_sep[i]) == NULL)		//word_tmp + j의 뒤에서부터 분리된문자열이 있는지 비교
                break;
            j=strlen(strstr(word_tmp+word_len-j,regular_sep[i]))-strlen(regular_sep[i]);
            //분리된 문자열의 최초 인덱스를 알기위해 j에 다음에 검사할 문자열의 (단어의 길이-최초 인덱스)를 구함
            //regular_sep[i]를 뺀 이유는 문자열의 중복 방지를 위해
            i++;
            if(i==regular_sep_count)
                return 1;
        }
    }
    return 0;
}

int countLine(char buffer[]) {          //라인별로 일치하는 문자열의 갯수를 구해서 반환해주는 메소드
    char line_tmp[100];
    char *word_tmp;
    int cnt=0;
    
    strcpy(line_tmp,buffer);//읽어온 한줄을 line_tmp에 저장
    word_tmp = strtok(line_tmp," \n");	//word_tmp에 단어별로 분리
    while(word_tmp != NULL) {
        cnt += compareWord(word_tmp);
        word_tmp = strtok(NULL, " \n");			//문장의 다음 단어를 탐색하기위해 토큰 분리
    }
    
    return cnt;
}

int main(int argc, char* argv[]) {
    int i;
    
    FILE *fp = NULL;
    char buffer[100];
    char sep_tmp[50];
    
    if(argc != 2) {
        printf("인수의 갯수가 맞지 않습니다.\n");
        return 1;
    }
    
    regular = argv[1];
    
    fp = fopen("/Users/baek/Desktop/pp/wordCounter/sample.txt", "r");
    
    if(fp == NULL) {
        printf("파일 열기 실패\n");
        return 1;
    }
    
    if(regular[0] == '"')                       //맨 앞의 " 제거
        regular = strchr(regular+1, regular[1]);
    
    if(regular[strlen(regular)-1] == '"')		//맨 뒤의 " 제거
        regular[strlen(regular)-1] = '\0';
    
    for(i=0; i<strlen(regular); i++)
        if(regular[i] == '*')
            count_star++;
    
    strcpy(sep_tmp,regular);
    regular_sep[0] = strtok(sep_tmp, "*\n");		//*을 기준으로 입력받은 문자열 분리
    
    regular_sep_count=1;
    while(regular_sep[regular_sep_count-1] != NULL) {//*을 기준으로 입력받은 문자열 분리
        regular_sep[regular_sep_count] = strtok(NULL, "*\n");
        regular_sep_count++;
    }
    regular_sep_count--;
    
    
    while(fgets(buffer, 100, fp) != NULL) {		//파일에서 한줄씩 읽기
        printf("%d ", countLine(buffer));         //몇개의 문자열이 일치했는지 출력
    }
    printf("\n");
    fclose(fp);
    return 0;
}