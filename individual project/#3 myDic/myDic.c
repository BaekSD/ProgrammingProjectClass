#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node {		//각 데이터(문자열)을 저장하는 변수 data와 다음 노드를 가리키는 next변수
	char* data;
	struct node *next;
} Node;

typedef struct linkedList {	//처음 노드를 가리키는 변수 first와 단어의 갯수를 저장하는 count변수
	Node *first;
	int count;
} LinkedList;

LinkedList *list[27];		//알파벳 갯수 26 + others = 총 27개의 배열 선언

void wordToLower(char* word) {	//단어를 소문자로 변환해주는 함수
	int i;

	for(i=0; i<strlen(word); i++)
		if(isupper(word[i]))
			word[i] = tolower(word[i]);
}

//word가 있는 인덱스를 검색, 반환.
//해당 단어가 있으면 word에 NULL을 넣어서 반환, 해당 인덱스 역시 반환
//없으면 word는 그대로 반환, 해당 인덱스는 insert될 인덱스 반환
//이렇게 하면 search, insert, delete 작업에 모두 사용가능해짐
//mode 1: search, 2: insert, 3: delete
void searchIndex(char** word, int* row, int* column, int mode) {
	int i_row, i_col;
	Node *tmp = NULL;

	wordToLower(*word);

	i_row = *word[0]-'a';
	i_col = 0;

	if(i_row < 0 || i_row > 26) {
		i_row = 26;		//범위를 벗어나면 (알파벳이 아니면) 26번 인덱스로 조정
	}

	while(1) {
		if(list[i_row]->count <= i_col) {	//해당 리스트에 있는 모든 문자열 점검했을 경우
			if(i_row != 26 && (list[i_row]->count == 5 || mode == 1 || mode == 3)) {
				i_row = 26;			//others가 아닐때, count가 5일 경우 others를 탐색
				i_col = 0;			//count가 5가 아니더라도 search모드거나 delete모드인경우 others 탐색
				continue;
			} else {			//해당하지 않는 경우 현재 행과 열을 반환
				*row = i_row;
				*column = i_col;
				return;
			}
		}

		if(i_col == 0) {		//0번째 열인 경우 tmp가 첫번째 노드를 가리키도록 함.
			tmp = list[i_row]->first;
		}

		if(strcmp(*word, tmp->data) == 0) {		//일치하는 경우 word를 NULL로 설정하고 행과 열을 반환
			*word = NULL;
			*row = i_row;
			*column = i_col;
			return;
		} else if(strcmp(*word, tmp->data) < 0 && mode == 2) {		//지나쳤고, insert모드인 경우 행과 열을 반환.
			*row = i_row;		//insert모드가 아닌 경우는 정확히 일치하는 것을 찾아야하기 때문.
			*column = i_col;
			return;
		} else {			//아직 안지나친 경우 다음 노드를 가리키게 함.
			tmp = tmp->next;
			i_col++;
		}
	}
}

void search(char* word) {	//search하는 함수.
	int row=0, column=0;

	searchIndex(&word, &row, &column, 1);	//인덱스를 찾아와서 처리

	printf("SEARCH: ");

	if(word != NULL) {		//word가 NULL이 아니라는 것은 해당 단어가 없다는 뜻(이라고 제가 정해뒀습니다.)
		printf("-1 -1\n");
	} else {
		printf("%d %d\n",row,column);
	}
}

//insert하는 함수. recursive는 0~25번(알파벳)이 5개가 꽉 찬 경우 1로 설정, 재귀 호출해서 others로 밀어내기위한 매개변수
void insert(char* word, int index, int recursive) {
	int row=0, column=0;
	int i = 0;
	char *newWord;
	Node *newNode, *tmp = NULL;

	searchIndex(&word, &row, &column, 2);

	if(index != -1)
		row = index;

	if(word == NULL) {			//NULL일 경우 이미 해당 단어가 있다는 뜻. 출력만 실행
		printf("INSERT: %d %d @ %d\n",row,column,list[row]->count);
	} else {
		newNode = (Node*)malloc(sizeof(Node));
		newWord = (char*)malloc(sizeof(char)*strlen(word));
		strcpy(newWord, word);
		newNode->data = newWord;		//새로 추가할 노드를 동적할당으로 생성

		if(list[row]->count == 0) {		//해당 리스트에 노드가 없으면 첫번째 노드로 연결
			list[row]->first = newNode;
		} else {
			tmp = list[row]->first;
			if(column == 0) {		//0번 열에 저장해야하는 경우
				list[row]->first = newNode;
				newNode->next = tmp;
			} else {			//1번 이상의 열에 저장해야하는 경우, 반복문으로 다음노드로 계속 찾아감
				for(i=0; i<column-1; i++)
					tmp = tmp->next;
				newNode->next = tmp->next;
				tmp->next = newNode;
			}
		}
		if(row != 26 && list[row]->count == 5) {	//others가 아니고, 5칸이 꽉 차 있던 경우
			for(; i<4; i++)				//마지막 노드로 이동해서
				tmp = tmp->next;
			newNode = tmp->next;
			tmp->next = NULL;
			insert(newNode->data, -1, 1);		//재귀호출하여 others에 넣어줌.
			free(newNode);
		} else			//others이거나 5칸이 안되는 경우 count 증가
			list[row]->count++;
		if(!recursive)
			printf("INSERT: %d %d @ %d\n",row,column,list[row]->count);
	}
}

void delete(char* word) {		//delete를 위한 함수.
	int row=0, column=0;
	int i;
	Node *delNode, *tmp;

	searchIndex(&word, &row, &column, 3);

	printf("DELETE: ");

	if(word != NULL) {		//NULL이 아닌 경우, 해당 단어 없다는 뜻.
		printf("-1 -1 @ -1\n");
	} else {			//삭제가능한 경우
		tmp = list[row]->first;
		if(column == 0) {			//0번 열을 삭제해야하는 경우
			list[row]->first = tmp->next;	//두번째 노드를 첫 노드로 설정해주고
			free(tmp);			//삭제할 노드를 동적할당 해제해준다.
		} else {
			for(i=0; i<column-1; i++)	//0번이 아닌경우 그만큼 이동
			tmp = tmp->next;
			delNode = tmp->next;
			tmp->next = delNode->next;	//끊어지지 않게 연결해주고
			free(delNode);			//동적할당 해제
		}
		list[row]->count--;			//count 감소
		printf("%d %d @ %d\n",row,column,list[row]->count);
	}
}

void readASCII() {		//프로그램 요구사항에는 없었지만, 아스키로 저장한 파일을 읽어오는 함수
	int i=0;
	FILE *fp = NULL;
	char buffer[1024], tmp[1024];
	char *word = NULL;

	fp = fopen("myDic.txt", "r");

	if(fp == NULL)
		return;

	while(fgets(buffer, 1024, fp) != NULL) {
		strcpy(tmp, buffer);

		word = strtok(tmp, " \r\n\t");
		while(word != NULL) {
			insert(word, i, 0);			//파일에 저장된 상태 그대로 넣어야하므로 해당 index인 i를 매개변수로 전달
			word = strtok(NULL, " \r\n\t");
		}

		i++;
	}

	fclose(fp);
}

void writeASCII() {		//아스키 값으로 단어장 저장하는 함수
	int i,j;		//i는 행, j는 열을 위한, for문에서 사용되는 변수
	FILE *fp = NULL;
	Node *tmp;

	fp = fopen("myDic.txt", "w");

	if(fp == NULL) {
		printf("파일 저장 오류\n");
		return;
	}

	for(i=0; i<27; i++) {
		tmp = list[i]->first;
		for(j=0; j<list[i]->count; j++) {	//해당 행의 갯수만큼 돌면서
			fputs(tmp->data, fp);		//fputs를 이용해 문자열을 입력
			fputs(" ", fp);			//문자열간의 구분을 위해 공백 추가
			tmp = tmp->next;
		}

		fputs("\r\n", fp);			//한 행 모두 추가 후 개행
	}

	fclose(fp);
}

void readBin() {					//바이너리 파일을 읽는 함수
	FILE *fp = NULL;
	int index=0, count=0, len=0, i;
	char word[101];

	fp = fopen("myDic.bin", "rb");

	if(fp == NULL)
		return;

	while(!feof(fp)) {
		fread(&index, sizeof(int), 1, fp);			//처음 int크기만큼의 값은 행의 값
		fread(&count, sizeof(int), 1, fp);			//그 다음 int크기만큼의 값은 해당 행의 문자열 갯수
		if(feof(fp))
			break;
		for(i=0; i<count; i++) {				//문자열 갯수만큼 반복
			fread(&len, sizeof(int), 1, fp);		//문자열의 길이가 int로 저장되있음
			fread(word, sizeof(char), len, fp);		//문자열의 길이만큼 char형으로 읽기
			insert(word, index, 0);				//index값을 매개변수로 넘겨줌. 해당 순서대로 저장하기 위함.
		}
	}

	fclose(fp);
}

void writeBin() {			//바이너리로 저장하는 함수
	FILE *fp = NULL;
	int i,j, isWrited = 0;		//isWrited는 입력된 것이 하나도 없을경우 파일 삭제를 위함
	int len;
	char buffer[101];
	Node *tmp;

	fp = fopen("myDic.bin", "wb");

	if(fp == NULL) {
		printf("파일 저장 오류\n");
		return;
	}

	for(i=0; i<27; i++) {
		if(list[i]->count != 0) {		//해당 행의 문자열 갯수가 0이면 패스
			if(isWrited == 0)
				isWrited = 1;

			fwrite(&i, sizeof(int), 1, fp);				//우선 해당 행의 인덱스를 int의 크기로 저장
			fwrite(&(list[i]->count), sizeof(int), 1, fp);		//그리고 해당 행의 문자열 갯수를 저장

			tmp = list[i]->first;
			for(j=0; j<list[i]->count; j++) {
				len = strlen(tmp->data);
				fwrite(&len, sizeof(int), 1, fp);		//문자열의 길이를 저장
				fwrite(tmp->data, sizeof(char), len, fp);	//문자열을 저장
				tmp = tmp->next;
			}
		}
	}

	fclose(fp);

	if(!isWrited) {			//만약 쓰인 것이 없다면, 해당 파일을 제거
		remove("myDic.bin");
	}
}

int main() {
	int i;
	char tmp[101];			//최대 문자열의 길이는 100!

	for(i=0; i<27; i++) {
		list[i] = (LinkedList*)malloc(sizeof(LinkedList));
		list[i]->count=0;
	}

	readBin();		//처음에 binary 파일에서 읽어온다. 있는지 여부는 해당 함수에서 검사.

	while(1) {
		printf("\n기능을 선택하세요\n");
		printf("[1] Search\t[2] Insert\t[3] Delete\n");
		printf("[4] save in ASCII\t[5] save in binary\n");
		printf("[6] quit\n");
		printf(": ");
		scanf("%d",&i);

		switch(i) {
			case 1:
				printf("찾을 단어를 입력하세요: ");
				scanf("%s",tmp);
				search(tmp);
				break;
			case 2:
				printf("삽입할 단어를 입력하세요: ");
				scanf("%s",tmp);
				insert(tmp,-1,0);
				break;
			case 3:
				printf("삭제할 단어를 입력하세요: ");
				scanf("%s",tmp);
				delete(tmp);
				break;
			case 4:
				writeASCII();
				break;
			case 5:
				writeBin();
				break;
			case 6:
				printf("종료합니다.\n");
				return 0;
			default:
				printf("올바른 메뉴를 선택하세요.\n");
				break;
		}
	}

	return 0;
}
