#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BLANK ' '

void rm_blank(char *str);
void rm_CAL(char *str, char *word);
char* input(void);

int isValid();

int getValid(int x);
void getToken(char* input);
void calc(char *input);
double expr();
double term();
double factor();

double reg[26];	//register save
int count;	//alpha save
double var;	//getToken에서 num값 저장후 아래 함수들에서 계산

enum
{
	PLUS, MULTI, MINUS, DIV, NUMBER, LP, RP, REZ, SIN, COS, NO, EXP, LOG, HP	//save token
}token;

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

int isValid(char *input)
{
	char fuc[4][5] = { "exp(","cos(","sin(","log(" };
	int i = 0, k, m, isNum = 4, l, open_count = 0, close_count = 0, saving_alpha[26] = { 0 }, isPut = 0;

	if (*(input + i) == '+' || *(input + i) == '-' || *(input + i) == '*' || *(input + i) == '/' || *(input + i) == ')')
		return 0;
	if (strcmp(input, "") == 0)
		return 0;

	for (i = 0; *(input + i) != '\0'; i++)
	{
		if (!isdigit(*(input + i))&&*(input+i)!='.')								//input이 숫자가 아니라면
		{
			if (*(input + i) != '+' && *(input + i) != '-' && *(input + i) != '/' && *(input + i) != '*' && *(input + i) != '(' && *(input + i) != ')')			//연산기호,괄호가 아니라면
			{
				for (k = 0; k < 6; k++)
				{
					if (k == 5)
						return 0;
					if (k == 4)
					{
						if (*(input + i) == '['&&islower(*(input+i+1))&&*(input + i + 2) == ']')				//[x] 비교
						{
							if (*(input + i + 3) != '+'&& *(input + i + 3) != '-'&&*(input + i + 3) != '/'&&*(input + i + 3) != '*'&&*(input + i + 3) != '\0'&&*(input + i + 3) != ')')
								return 0;
							if (saving_alpha[*(input + i + 1)-97]==0)
								return 0;
							i += 2;
							break;
						}
						else
							continue;
					}
					if (strncmp(input + i, fuc[k], 4))				//exp(,cos(,sin(,log( 비교
						continue;
					else
					{
						i += 3;
						break;
					}
				}
			}
			if (strncmp(input + i, "->[", 3) == 0 && islower(*(input + i + 3)) && *(input + i + 4) == ']')
			{
				saving_alpha[*(input + i + 3) - 97] = 1;
				i += 4;
			}
		}
	}
	for (l = 0; *(input + l) != '\0'; l++)
	{
		if (close_count > open_count)																			// )가 (보다 많으면 fail
			return 0;
		if (isNum == 0)
		{
			if (isdigit(*(input + l)) == 0 && *(input + l) != '('&&*(input + l) != ')'&&*(input + l) != 'c'&&*(input + l) != 's'&&*(input + l) != 'l'&&*(input + l) != 'e'&&*(input + l) != '[')						//앞에가 숫자가 아닌데 다음이 기호이고 )가 아니고 (가 아니면 fail
				return 0;
			else
			{
				if (strncmp(input + l, "(+", 2) == 0 || strncmp(input + l, "(-", 2) == 0)					//(+나 (-판별
				{
					if (*(input + l + 2) == ')')
						return 0;
					l += 1;
					if (*(input + l + 1) == '[')
					{
						l += 3;
						isNum = 1;
						open_count += 1;
						continue;
					}
					if (*(input + l+1) == 'e' || *(input + l+1) == 's' || *(input + l+1) == 'l' || *(input + l+1) == 'c')
					{
						l += 3;
						isNum = 3;
						open_count += 1;
						continue;
					}
					while (isdigit(*(input + l+1)) != 0 || *(input + l+1) == '.')
						l++;
					if (*(input + l+1) != ')' && *(input + l + 1) != '(')
						return 0;
					else if (*(input + l + 1) == ')')
					{
						close_count++;
						l++;
					}
					isNum = 2;
					open_count += 1;
					continue;
				}
				if (*(input + l) == 'e' || *(input + l) == 's' || *(input + l) == 'l' || *(input + l) == 'c')
				{
					l += 2;
					isNum = 3;
				}
				else if (*(input + l) == '[')
				{
					l += 2;
					isNum = 1;
				}
				else if (*(input + l) == '(')
				{
					open_count += 1;
					isNum = 2;
				}
				else
					isNum = 1;
			}
		}
		else if (isNum == 1)																						//앞에가 숫자나 [x]일 때 그러므로 다음엔 )나 연산부호가 와야함
		{
			if (*(input + l) == '(' || *(input + l) == 'e' || *(input + l) == 'c' || *(input + l) == '[' || *(input + l) == 's'&& *(input + l) == 'l')																		//숫자 다음 (나 cos등이 바로올때 fail
				return 0;
			else if (*(input + l) == ')')																	//숫자 다음 )이 올 때
			{
				close_count += 1;
				isNum = 2;
			}
			else if (open_count == close_count&&strncmp(input + l, "->", 2) == 0)
			{
				l += 4;
				isNum = 4;
			}
			else if (isdigit(*(input + l)) || *(input + l) == '.')
			{
				if (*(input + l) == '.')
					if (!isdigit(*(input + l + 1)))
						return 0;
					isNum = 1;
			}
			else
			{
				if (*(input + l + 1) == '\0')
					return 0;
				isNum = 0;
			}																								//숫자 다음 연산기호가 올 때
		}
		else if (isNum == 2)																						//앞에가 괄호기호 일때
		{
			if (*(input + l - 1) == '('||strncmp(input + l-2, "(+", 2) == 0 || strncmp(input + l-2, "(-", 2)==0)																	//앞에가 (일 때
			{
				if (strncmp(input +l, "(+", 2) == 0 || strncmp(input + l, "(-", 2) == 0)					//(+나 (-판별
				{
					if (*(input + l + 2) == ')')
						return 0;
					l += 1;
					if (*(input + l+1) == '[')
					{
						l += 3;
						isNum = 1;
						open_count += 1;
						continue;
					}
					if (*(input + l + 1) == 'e' || *(input + l + 1) == 's' || *(input + l + 1) == 'l' || *(input + l + 1) == 'c')
					{
						l += 3;
						isNum = 3;
						open_count += 1;
						continue;
					}
					while (isdigit(*(input + l+1)) != 0 || *(input + l+1) == '.')
						l++;
					if (*(input + l+1) != ')' && *(input + l + 1) != '(')
						return 0;
					else if(*(input+l+1)==')')
					{
						close_count++;
						l++;
					}
					isNum = 2;
					open_count += 1;
					continue;
				}
				if (*(input + l) == '*' || *(input + l) == '/' || *(input + l) == ')')
					return 0;
				else if (*(input + l) == '(')
				{
					open_count += 1;
					isNum = 2;
				}
				else if (*(input + l) == '[')
				{
					l += 2;
					isNum = 1;
				}
				else if (isdigit(*(input + l)))
				{
					isNum = 1;
				}
				else
				{
					l += 2;
					isNum = 3;
				}
			}
			else																							//앞에가 )일 때
			{
				if (open_count == close_count&&strncmp(input + l, "->", 2) == 0)
				{
					l += 4;
					isNum = 4;
					continue;
				}
				if (*(input + l) == '+' || *(input + l) == '-' || *(input + l) == '*' || *(input + l) == '/')
				{
					isNum = 0;
				}
				else if (*(input + l) == ')')
				{
					close_count += 1;
					isNum = 2;
				}
				else
					return 0;
			}
		}
		else if (isNum == 3)
		{
			if (strncmp(input + l, "(+", 2) == 0 || strncmp(input + l, "(-", 2) == 0)					//(+나 (-판별
			{
				if (*(input + l + 2) == ')')
					return 0;
				l += 1;
				if (*(input + l + 1) == '[')
				{
					l += 3;
					isNum = 1;
					open_count += 1;
					continue;
				}
				if (*(input + l + 1) == 'e' || *(input + l + 1) == 's' || *(input + l + 1) == 'l' || *(input + l + 1) == 'c')
				{
					l += 3;
					isNum = 3;
					open_count += 1;
					continue;
				}
				while (isdigit(*(input + l+1)) != 0 || *(input + l+1) == '.')
					l++;
				if (*(input + l + 1) != ')' && *(input + l + 1) != '(')
					return 0;
				else if (*(input + l + 1) == ')')
				{
					close_count++;
					l++;
				}
				isNum = 2;
				open_count += 1;
				continue;
			}
			else
			{
				isNum = 2;
				open_count += 1;
				continue;
			}
		}
		else if (isNum == 4)																						//맨 앞에 오는 수
		{
			if (strncmp(input + l, "(+", 2) == 0 || strncmp(input + l, "(-", 2) == 0)					//(+나 (-판별
			{
				if (*(input + l + 2) == ')')
					return 0;
				l += 1;
				if (*(input + l + 1) == '[')
				{
					l += 3;
					isNum = 1;
					open_count += 1;
					continue;
				}
				if (*(input + l + 1) == 'e' || *(input + l + 1) == 's' || *(input + l + 1) == 'l' || *(input + l + 1) == 'c')
				{
					l += 3;
					isNum = 3;
					open_count += 1;
					continue;
				}
				while (isdigit(*(input + l+1)) != 0 || *(input + l+1) == '.')
					l++;
				if (*(input + l+1) != ')'&& *(input + l + 1) != '(')
					return 0;
				else if (*(input + l + 1) == ')')
				{
					close_count++;
					l++;
				}
				isNum = 2;
				open_count += 1;
				continue;
			}
			if (*(input + l) != '(' && isdigit(*(input + l)) == 0 && *(input + l) != 'e'&&*(input + l) != 'c'&& *(input + l) != '['&& *(input + l) != 's'&& *(input + l) != 'l')							//맨 처음에 (나 숫자가 아니면 fail
			{
				return 0;
			}
			else if (*(input + l) == 'e' || *(input + l) == 's' || *(input + l) == 'l' || *(input + l) == 'c')
			{
				l += 2;
				isNum = 3;
			}
			else if (*(input + l) == '[')
			{
				l += 2;
				isNum = 1;
			}
			else if (*(input + l) == '(')
			{
				open_count += 1;
				isNum = 2;
			}
			else
				isNum = 1;
		}
	}
	if (open_count != close_count)
		return 0;
	return 1;
}

int getValid(int x)		//문자의 끝을 확인하는 함수
{
	static int num = 0;
	if (x==1)
		num=1;
	else if(x==0)
		num=0;
	return num;
}

void getToken(char* input)	//문자열을 문자단위로 하나씩 읽어가면서 token, var 저장
{
	double t;
	static char *p;
	if(input != NULL) {
		p = input;
	}
	switch(*p)	//case 마다 token 저장
	{
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
void calc(char *input)
{
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

double expr()
{
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

double term()
{
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

double factor()
{
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
	int i;

	while(1) {
		for(i=0; i<26; i++)
			reg[i] = 0;
		str = input();
		if(strcmp(str,"EXIT") == 0) {
			return 0;
		}
		if(isValid(str))
			calc(str);
		else
			printf("this is not valid\n");
	}

	return 0;
}
