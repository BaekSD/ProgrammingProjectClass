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
void insert(char* word, int recursive) {
	int row=0, column=0;
	int i = 0;
	char *newWord;
	Node *newNode, *tmp = NULL;

	searchIndex(&word, &row, &column, 2);

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
			insert(newNode->data, 1);		//재귀호출하여 others에 넣어줌.
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

int main() {
	int i;
	FILE *fp = NULL;
	char buffer[1024], tmp[1024];
	char *word = NULL;

	fp = fopen("voc_sample.txt", "r");

	if(fp == NULL) {
		printf("파일 읽기 실패");
		return -1;
	}

	for(i=0; i<27; i++) {
		list[i] = (LinkedList*)malloc(sizeof(LinkedList));
		list[i]->count=0;
	}

	while(fgets(buffer,1024,fp) != NULL) {
		strcpy(tmp,buffer);

		word = strtok(tmp," \r\n\t");
		while(word != NULL) {
			insert(word,0);

			word = strtok(NULL, " \r\n\t");
		}
	}

	fclose(fp);

	while(1) {
		printf("\n기능을 선택하세요\n");
		printf("[1] Search\t[2] Insert\n");
		printf("[3] Delete\t[4] Quit\n");
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
				insert(tmp,0);
				break;
			case 3:
				printf("삭제할 단어를 입력하세요: ");
				scanf("%s",tmp);
				delete(tmp);
				break;
			case 4:
				printf("종료합니다.\n");
				return 0;
			default:
				printf("올바른 메뉴를 선택하세요.\n");
				break;
		}
	}

	return 0;
}
