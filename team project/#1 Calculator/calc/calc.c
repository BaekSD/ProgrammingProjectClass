#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int getValid(int x);
void getToken(char* input);
void calc(char *input);
double expr();
double term();
double factor();

double reg[26];	//register save

int count;	//alpha save
double var;	//getToken에서 num값 저장후 아래 함수들에서 계산

enum {
    PLUS, MULTI, MINUS, DIV, NUMBER, LP, RP, REZ, SIN, COS, NO, EXP, LOG, HP	//save token
}token;

int getValid(int x)	{       //문자의 끝을 확인하는 함수
    static int num = 0;
    if (x==1)
        num=1;
    else if(x==0)
        num=0;
    return num;
}

void getToken(char* input)	{	//문자열을 문자단위로 하나씩 읽어가면서 token, var 저장
    double t;
    static char *p;
    if(input != NULL) {
        p = input;
    }
    switch(*p)	{	//case 마다 token 저장
        case '+':
            token = PLUS;
            break;
        case '*':
            token = MULTI;
            break;
        case '-':
            if(*(p+1)=='>')	{	//-> 일때 다음 문자로 넘어감
                token = NO;
                p++;
            }
            else
                token = MINUS;
            break;
        case '>':		// ->일때 다음 문자로 넘어감
            token = NO;
            break;
        case '/':
            token = DIV;
            break;
        case '(':
            token = LP;
            break;
        case ')':
            token = RP;
            break;
        case 's':
            if(*(p+1) == 'i')	{
                token = SIN;
                p += 2;		//sin 을 만났을때 다음 문자를 ( 로
            }
            else if(*(p-2)=='>')	{
                token = REZ;	//register 저장을 해줄땐 token = REZ
                count = *p - 'a';
            }
            else		{	//register 호출할때 token = HP
                token = HP;
                count = *p - 'a';
            }
            break;
        case 'c':
            if(*(p+1) == 'o')	{	//sin 과 동일
                token = COS;
                p += 2;
            }
            else if(*(p-2)=='>')	{
                token = REZ;
                count = *p - 'a';
            }
            else	{
                token = HP;
                count = *p - 'a';
            }
            break;
        case 'e':
            if(*(p+1) == 'x')	{	//sin과 동일
                token = EXP;
                p += 2;
            }
            else if(*(p-2)=='>')	{
                token = REZ;
                count = *p - 'a';
            }
            else	{
                token = HP;
                count = *p - 'a';
            }
            break;
        case 'l':
            if(*(p+1) == 'o')	{	//sin과 동일
                token = LOG;
                p += 2;
            }
            else if(*(p-2)=='>')	{
                token = REZ;
                count = *p - 'a';
            }
            else	{
                token = HP;
                count = *p - 'a';
            }
            break;
        case '[':		// [ 을 만났을때 다음 문자로
            p++;
            getToken(NULL);
            break;
        case ']':
            getToken(NULL);
            break;
        default:
            if('0' <= *p && *p <= '9')	{	// number calculate
                for(var = 0.0; '0'<= *p && *p <= '9'; p++)	{
                    var *= 10.0;
                    var += *p-'0';	//문자를 아스키코드로 표현
                }
                if(*p == '.' )	{	//소숫점일때
                    p++;
                    if('0'<=*p && *p<='9'){
                        for(t=1.0;'0'<=*p && *p<='9'; p++)	{
                            t /= 10.0;
                            var += t*(*p-'0');
                        }
                    }
                }
                token = NUMBER;
                return;
            }
            else if( *p == '\0' )	{	//문자의 끝일때 EOF 출력후 종료
                getValid(1);
                return ;
            }
            if(*p - 'a' >= 0 && *p - 'a' <= 25)	{	//a~z를 만났을때 register
                if(*(p-2)=='>')
                    token = REZ;
                else
                    token = HP;
                count = *p - 'a';
            }
    }
    p++;
}

/*
 expr(), term(), factor()이 나눠진이유는
 우선순위를 나눠 구하기 위해서 이고,
 뒤쪽으로 갈수록 높은 우선순위이다.
 */
void calc(char *input)	{
    double r=0;
    getValid(0);
    getToken(input);	// get token
    r = expr();	// start calculate
    while(r==-9999)	{	//register저장이 반복되었을때
        if(r == -9999)	// register 저장시 값을 0으로 초기화하기 때문에 0일시에 다음식 계산
            if(getValid(100)==1) {
                printf("this is not valid\n");
                return;
            }
            else
                r = expr();
    }
    printf(" = %lf\n",r);
}

double expr()	{
    double r=0;
    r = term();	//우선순위를 위해서 넣어놓은 식
    while(token == PLUS || token == MINUS)	{
        if(token == PLUS)	{	//PLUS 일때 다음 토큰을 구함
            getToken(NULL);
            r += term();		//PLUS 연산
        }
        else if(token == MINUS)	{
            getToken(NULL);
            r -= term();
        }
    }
    if(token == REZ)	{	//register 저장할때 reg[count]에 값 저장후 다음토큰 구함
        reg[count] = r;
        getToken(NULL);
        return -9999;
    }
    return r;
}

double term()	{
    double r=0;
    r = factor();	//우선순위를 위해서 넣어놓은 식
    while(token == MULTI || token == DIV)	{
        if( token == MULTI )	{
            getToken(NULL);
            r *= factor();
        }
        else if(token == DIV)	{
            getToken(NULL);
            r /= factor();
        }
    }
    return r;
}

double factor()	{
    double r=0;
    if(token == NUMBER)	{	//token == NUMBER일때
        r = var;		//r에 var 저장
        getToken(NULL);
    }
    else if(token == HP) {
        r = reg[count];
        getToken(NULL);
    }
    else if(token == LP)	{
        getToken(NULL);
        r = expr();		// (을 만나면 식 계산
        if(token == RP)	{
            getToken(NULL);	// )일때 다음토큰
        }
    }
    if(token == NO)	{		// 다음토큰구함
        getToken(NULL);
    }
    if(token == SIN)	{	//sin 계산
        getToken(NULL);
        r = factor();
        r = sin(r);
    }
    if(token == COS)	{
        getToken(NULL);
        r = factor();
        r = cos(r);
    }
    if(token == EXP)	{
        getToken(NULL);
        r = factor();
        r = exp(r);
    }
    if(token == LOG)	{
        getToken(NULL);
        r = factor();
        r = log(r);
    }
    return r;
}

int main() {
    char* str;

    str = (char*)malloc(sizeof(char));
    
    while(1) {
        gets(str);
        calc(str);
    }
}
