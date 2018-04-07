#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int isValid();

int main(void) {
	int i;
	char str[50];
	while (1) {
		scanf("%s", str);
		i = isValid(str);
		printf("%d\n", i);
	}
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
