#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//dynamic allocation 사용시 the others 에 100개제한 없음

typedef struct ListNode Node;
typedef struct LinkedList list;
void InsertTheOthers(list** List,char* inswrd);

typedef struct ListNode{
    char* word;
    Node* next;
} Node;

typedef struct LinkedList{
    Node header;
    int count;
} list;

void displayList(list* List) {
    Node node = List->header;
    node = *node.next;
    int i = 0;
    
    for (i = 0; i < List->count; i++) {
        printf("%s    ", node.word);
        
        node = *node.next;
        if (&node == NULL) {
            printf("\n");
            return;
        }
    }
}

list* createList(){
    list* ret = NULL;
    
    ret = (list*)malloc(sizeof(list));
    if (ret != NULL){
        memset(ret, 0, sizeof(list));
    }
    else{
        printf("Error createList() memory allocation\n");
        return NULL;
    }
    
    return ret;
}

int addElement(list* List,int position, char* element){
    int ret = -1;
    int i = 0;
    Node* prev = NULL;
    Node* newNode = NULL;
    if (List != NULL){
        if (position >= 0 && position <= List->count){
            newNode = (Node*)malloc(sizeof(Node));
            if (newNode != NULL){
                newNode->word = element;
                newNode->next = NULL;
                
                prev = &(List->header);
                for (i = 0; i < position; i++){
                    prev = prev->next;
                }
                
                newNode->next = prev->next;
                prev->next = newNode;
                
                List->count++;
                
                ret = 1;
            }
            else{
                printf("Error addElement() newNode allocation\n");
                return ret;
            }
        }
        else {
            printf("Error addElement() position error\n");
            return ret;
        }
    }
    return ret;
}
void removeElement(list* List, int position){
    int i = 0;
    char strcp[100];
    Node* node = NULL;
    Node* delnode = NULL;
    if (List != NULL){
        if (position >= 0 && position < List->count){
            node = &(List->header);
            for (i = 0; i < position; i++){
                node = node->next;
            }
            delnode = node->next;
            node->next = delnode->next;
            
            strcpy(strcp, delnode->word);
            free(delnode);
            
            List->count--;
        }
        else{
            printf("Error removeElement() position error\n");
        }
    }
    
    return;
}

void deleteList(list* List){
    if (List != NULL){
        if (List->count > 0){
            removeElement(List, 0);
        }
        free(List);
    }
}

void charChange(char* str){             //대문자 소문자 변환
    int length = strlen(str);
    int i = 0;
    for (i = 0; i <length; i++){
        if (str[i] >= 65 && str[i] <= 90){
            str[i] += 32;
        }
    }
    
    return;
}

void displayNode(list* List){
    Node node = List->header;
    int i = 0;
    if(List->count != 0){
        node = *node.next;
        for(i;i<List->count ; i++){
            printf("%s   ",node.word);
            if(i == List->count-1) { break; }
            node = *node.next;
        }
    }
}

void Search(list** List){               //소문자로 바뀐 문자열이 들어와야 함.
    char firstchar[1024];
    int row;
    int i;
    Node node;
    char* srcwrd = (char*)malloc(sizeof(char)*1024);
    
    scanf("%s",srcwrd);
    charChange(srcwrd);
    sprintf(firstchar,"%d",srcwrd[0]);
    row = atoi(firstchar) -97;
    
    
    node = List[row]->header;
    if(List[row]->count!=0){
        node = *node.next;
        for(i = 0 ; i < List[row]->count; i++){
            if(strcmp(srcwrd,node.word)==0){
                printf("SEARCH: %d %d\n",row,i);
                return;
            }
            if(i != ((List[row]->count-1))){
                node = *node.next;
            }
        }
    }
    node = List[26]->header;
    
    if(List[26]->count != 0){
        node = *node.next;
        for(i =0 ; i < List[26]->count; i++){
            if(strcmp(srcwrd,node.word)==0){
                printf("SEARCH: %d %d\n",26,i);
                return;
            }
            if(i != ((List[26]->count-1))){
                node = *node.next;
            }
        }
    }
    printf("SEARCH: -1 -1\n");
    return;
}

void Insert(list** List, char *inswrd){
    int n;
    int i = 0;
    char firstchar[100];
    int row;
    char* strcp=(char*)malloc(sizeof(char)*100);
    Node node;
    Node node2;
    char strcp2[100];
    int flag = 0;
    charChange(inswrd);
    sprintf(firstchar, "%d", inswrd[0]);
    row = atoi(firstchar) - 97;
    
    node = List[row]->header;
    if (List[row]->count == 0){
        addElement(List[row], 0, inswrd);
        printf("INSERT: %d 0 @ %d\n", row, List[row]->count);
        return;
    }
    node = *node.next;
    
    for (i = 0; (i < List[row]->count); i++){
        if (strcmp(inswrd, node.word) < 0){
            n = i;
            addElement(List[row], i, inswrd);
            printf("INSERT: %d %d @ %d\n",row,i,List[row]->count);
            flag = 1;
            break;
        }
        if(strcmp(inswrd,node.word)==0){
            printf("INSERT: %d %d @ %d\n",row,i,List[row]->count);
            return;
        }
        if(i != List[row]->count-1){
            node = *node.next;
        }
    }
    if (flag == 0){
        n = List[row]->count;
        addElement(List[row], List[row]->count, inswrd);
        printf("INSERT: %d %d @ %d\n",row,List[row]->count-1,List[row]->count);
    }
    
    node = List[row]->header;
    flag=0;
    if(List[row]->count >= 6) {
        for (i = 0; i < 6; i++) {
            node = *node.next;
        }
        strcp=(char*)malloc(sizeof(char)*100);
        strcpy(strcp, node.word);
        printf("Delete Node\n");
        removeElement(List[row], 5);
        if(List[26]->count == 0 ){
            addElement(List[26],0,strcp);
            printf("0INSERT: %d %d @ %d\n",26,0,List[26]->count);
            return;
        }
        node = List[26]->header;
        node = *node.next;
        for(i =0 ;i<List[26]->count;i++){
            if(strcmp(strcp,node.word)<0){
                addElement(List[26],i,strcp);
                printf("INSERT: %d %d @ %d\n",26,i,List[26]->count);
                return;
            }
            else if(strcmp(strcp,node.word)==0){
                printf("INSERT: %d %d @ %d\n",26,i,List[26]->count);
                return;
            }
            if(i == List[26]->count-1){ break; }
            node = *node.next;
        }
        addElement(List[26],List[26]->count,strcp);
        printf("INSERT: %d %d @ %d\n",26,List[26]->count-1,List[26]->count);
    }
}

void Delete(list** List, char *delwrd){
    int row,i;
    char firstword[100];
    Node node;
    sprintf(firstword,"%d",delwrd[0]);
    row = atoi(firstword)-97;
    
    node = List[row]->header;
    if(List[row]->count!=0){
        node = *node.next;
        for(i = 0 ; i < List[row]->count; i++){
            if(strcmp(delwrd,node.word)==0){
                printf("DELETE: %d %d @ %d\n",row,i,List[row]->count-1);
                removeElement(List[row],i);
                return;
            }
            if(i != ((List[row]->count-1))){
                node = *node.next;
            }
        }
    }
    node = List[26]->header;
    
    if(List[26]->count != 0){
        node = *node.next;
        for(i =0 ; i < List[26]->count; i++){
            if(strcmp(delwrd,node.word)==0){
                printf("DELETE: %d %d @ %d\n",26,i,List[26]->count-1);
                removeElement(List[26],i);
                return;
            }
            if(i != ((List[26]->count-1))){
                node = *node.next;
            }
        }
    }
    printf("DELETE: -1 -1 @ -1\n");
    
    return;
    
}

int main(){
    FILE *fp;
    list* alpha[27];
    char buf[1024];
    char* buf2;
    char str[100];
    int menu = 3;
    int i = 0;
    for (i = 0; i < 27; i++){
        alpha[i] = createList();
    }
    
    fp = fopen("voc_sample.txt", "r");
    if (fp == NULL) { printf("file open Error\n"); return -1; }
    while (!feof(fp)){
        fgets(buf, 1024, fp);
        buf2 = strtok(buf, " \t\n\r.,");
        while (buf2 != NULL){
            printf("%s\n",buf2);
            Insert(alpha, buf2);
            buf2 = strtok(NULL, " \n\r.,");
        }
    }
    
    while (menu != 4){
        printf("1. Search\n2.Insert\n3.Delete\n4.Quit\n");
        printf("Select Menu : ");
        scanf("%d", &menu);
        
        switch (menu) {
            case 1:
                printf("찾을 단어를 입력하세요 : ");
                Search(alpha);
                
                break;
                
            case 2:
                printf("추가할 단어를 입력하세요 : ");
                scanf("%s",str);
                Insert(alpha,str);
                
                break;
                
            case 3:
                printf("삭제할 단어를 입력하세요 : ");
                scanf("%s",str);
                Delete(alpha,str);
                break;
                
            default:
                printf("================\n");
                printf("종료합니다\n");
                printf("================");
        }
        
    }
    
    for(i =0 ; i <27;i++){
        deleteList(alpha[i]);
    }
    return 0;
}