//BAJENARU CATALIN
//GRUPA 311CB
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define INF 1234567890 
typedef struct nod
{
	struct nod *st, *dr;
	int h, val;
	char nume[11];
}*AVL;

typedef struct arb
{
	struct arb *st, *dr;
	int val;
	char nume[5];
}*ARB;

int makenumber(char *str)
{
	int x = 0, i = 0, sign = 1;
	if(str[0] == '-')
	{
		sign = -1;
		i++;
	}
	while(str[i] >= '0' && str[i] <= '9')
	{
		x = x * 10 + str[i++] - '0';
	}
	return x * sign;
}

int maxh(AVL n)
{
	int a = 0, b = 0;
	if(n->st)
		a = n->st->h;
	if(n->dr)
		b = n->dr->h;
	return a > b ? a : b;
}

AVL rotdr(AVL n)
{
	AVL aux = n->st;
	n->st = aux->dr;
	aux->dr = n;
	n->h = maxh(n) + 1;
	aux->h = maxh(aux) + 1;
	return aux;
}

AVL rotst(AVL n)
{
	AVL aux = n->dr;
	n->dr = aux->st;
	aux->st = n;
	n->h = maxh(n) + 1;
	aux->h = maxh(aux) + 1;
	return aux;
}

AVL inserAVL(AVL n, char nume[11], int val)
{
	if(n == NULL)
	{
		n = (AVL)malloc(sizeof(struct nod));
		if(!n)
			return NULL;
		n->val = val;
		strcpy(n->nume, nume);
		n->h = 1;
		n->st = n->dr = NULL;
		return n;
	}
	if(strcmp(nume, n->nume) < 0)
		n->st = inserAVL(n->st, nume, val);
	else
		n->dr = inserAVL(n->dr, nume, val);
	int a = 0, b = 0, c = 0, d = 0;
	if(n->st)
		a = n->st->h;
	if(n->dr)
		b = n->dr->h;
		n->h = (a > b ? a : b) + 1;//ii calculeaza inaltimea
		if(a > b + 1)
		{//balanseaza arborele
			if(n->st->st)
				c = n->st->st->h;
			if(n->st->dr)
				d = n->st->dr->h;
			if(d > c)
				n->st = rotst(n->st);
			n = rotdr(n);
		}
		else
			if(b > a + 1)
			{
				if(n->dr->dr)
					c = n->dr->dr->h;
				if(n->dr->st)
					d = n->dr->st->h;
				if(d > c)
					n->dr = rotdr(n->dr);
				n = rotst(n);
			}
			return n;
}

int findinAVL(AVL n, char nume[11])
{
	if(!n)
		return INF;
	if(strcmp(nume, n->nume) == 0)
		return n->val;
	if(strcmp(nume, n->nume) < 0)
		return findinAVL(n->st, nume);
	else
		return findinAVL(n->dr, nume);
}

void eliberarb(ARB n)
{
	if(!n)
		return;
	eliberarb(n->st);
	eliberarb(n->dr);
	free(n);
}

void eliberavl(AVL n)
{
	if(!n)
		return;
	eliberavl(n->st);
	eliberavl(n->dr);
	free(n);
}

void buildarb(ARB *tree, char s[1111], AVL rad)
{	
		ARB stack[strlen(s)/2], cel;
		(*tree) = malloc(sizeof(struct arb));
		if(!*tree)
			return;
		(*tree)->st = (*tree)->dr = NULL;
		strcpy((*tree)->nume, "sqrt");
		int i = 0;
		stack[i] = (*tree);
	char *it;
	it = strtok(s, " ");
	while(it)
	{
		cel = malloc(sizeof(struct arb));
		if(!cel)
		{
			eliberarb(*tree);
			break;
		}
		cel->st = cel->dr = NULL;
		if(it[0] == '+' || strcmp(it, "-") == 0 || it[0] == '*' || it[0] == '/'
		 || it[0] == '^' || strcmp(it, "sqrt") == 0)
		{
			strcpy(cel->nume, it);
			cel->val = INF-1;
			if(!stack[i]->st)
				{
					stack[i]->st = cel;
					if(strcmp(stack[i]->nume, "sqrt") == 0)
						stack[i] = cel;
					else
					stack[++i] = cel;
				}
				else
				{
					stack[i]->dr = cel;
					stack[i] = cel;
				}
		}
		else
				{
					if(it[strlen(it) - 1] == '\n')
						it[strlen(it) - 1] = 0;
					if((it[0] >= '0' && it[0] <= '9') || it[0] == '-')
						cel->val = makenumber(it);
					else
						{
							cel->val = findinAVL(rad, it);
							if(cel->val == INF)
							{
								if(strlen(it) == 0)
									{//iful asta e special pt testu 5 unde
										//e un spatiu-n plus la finalul primei linii
										free(cel);
										break;
									}
								strcpy(cel->nume, it);
							}
						}
					if(!stack[i]->st)
						{
							stack[i]->st = cel;
							if(strcmp(stack[i]->nume, "sqrt") == 0)
								i--;
						}
						else
						{
							stack[i]->dr = cel;
							i--;
						}
				}
		it = strtok(NULL, " ");
	}
	cel = (*tree)->st;
	free(*tree);
	*tree = cel;
}

int calcarb(ARB n, int *ok, FILE *out)
{
		if(n->val == INF)
	{
		*ok = 2;
		fprintf(out, "Variable %s undeclared\n", n->nume);
		return 0;
	}
	if(!n->st)
		return n->val;
	int a = calcarb(n->st, ok, out);
	if(*ok)
		return 0;
	if(strcmp(n->nume, "sqrt") == 0)
		{
			if(a < 0)
			{
				*ok = 1;
				return 0;
			}
			return ((int)sqrt(a));
		}
	int b = calcarb(n->dr, ok, out);
		if(*ok)
			return 0;
	if(strcmp(n->nume, "+") == 0)
		return a + b;
	if(strcmp(n->nume, "-") == 0)
		return a - b;
	if(strcmp(n->nume, "*") == 0)
		return a * b;
	if(strcmp(n->nume, "^") == 0)
		return (int)pow(a, b);
	if(strcmp(n->nume, "/") == 0)
		{
			if(b == 0)
				{
					*ok = 1;
			   	    return 0;
				}
			return a / b;
		}
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 3)
		return 0;
	FILE *in = fopen(argv[1], "rt");
	FILE *out = fopen(argv[2], "w");
	int n, ok, x;
	AVL rad = NULL;
	ARB tree = NULL;
	char s[1111], *aux, nume[11];
	fscanf(in, "%d", &n);
	fgets(s, 1110, in);
	while(n--)
	{
		
		fgets(s, 1110, in);
		aux = strtok(s, " ");
		strcpy(nume, aux);
		aux = strtok(NULL, " ");
		aux = strtok(NULL, " ");
		rad = inserAVL(rad, nume, makenumber(aux));	
	}
	fscanf(in, "%d", &n);
	fgets(s, 1110, in);
	while(n--)
	{	
		ok = 0;
		fgets(s, 1110, in);
		buildarb(&tree, s, rad);
		x = calcarb(tree, &ok, out);
		if(ok == 1)
			fprintf(out, "Invalid expression\n");
		else
			if(ok == 0)
				fprintf(out, "%d\n", x);
		eliberarb(tree);
		tree = NULL;
	}
eliberavl(rad);
	return 0;
}