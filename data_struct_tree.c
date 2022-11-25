#include<stdio.h>
#include<stdlib.h>

typedef struct _node
{
    int operand; //피연산자
    int optr; //연산자
    int op_flag; //1 if operator (operan와 optr구분 1이면 optr)
    struct _node* left; //이진나무기 때문에 자손 노드 두개 만들기
    struct _node* right; //이진나무기 때문에 자손 노드 두개 만들기
} Node;

typedef struct _stack
{
    int top; //스택이 여러개 사용되기 때문에 top을 묶어 혼동의 우려 줄임
    Node *ptr_stack[10]; //노드의 포인터를 저장하기위한 스택
} TreeStack; 

typedef struct _fix
{
    int top;
    char op_stack[10];
} OpStack; //연산자를 저장하는 스택 캐릭터 저장 +,-,*,/

Node* head,*tail; //tree를 위한 포인터 선언

void init_tree(void); //트리 초기 설계
Node* make_parse_tree(TreeStack* s, char* p); //트리 만들기
void my_push2(TreeStack* s, Node *t);//푸쉬 스택 tree
void my_push1(OpStack* s, char c);//푸쉬 스택 optr
char my_pop1(OpStack* s);//연산자
Node* my_pop2(TreeStack* s);//트리 스택
int is_operator(int k);//연산자냐
int precedence(int op);//값이냐
void postfix(OpStack *s, char *dest, char* src);//후위표기법 바꾸는거 저장
void traverse(Node* t);//부모를 마지막 방문 하는 traverse
void visit(Node* t); //출력을 하고 계산을 함

int main()
{
    char src[100], dst[100];
    OpStack stack1;
    TreeStack stack2;

    stack1.top=-1; //init
    stack2.top=-1;

    init_tree();

    printf(" Enter infix formula \n"); //중위표기법 입력
    gets(src); //소스에 저장
    printf(" Infix formula : %s \n", src);
    postfix(&stack1, dst, src); //후위표기법으로 바꿈
    printf( "Postfix formula : %s \n",dst);
    head->right=make_parse_tree(&stack2,dst);//후위표기법으로 표시된 식을 스택2를 이용해 루트노드 받기 
    printf(" traverse of tree \n");
    traverse(head->right); //루트노드 주소 주기 traverse실행
    printf(" \n");
}

void init_tree()
{
    head=(Node*)malloc(sizeof(Node));
    tail=(Node*)malloc(sizeof(Node));
    head->left=tail;
    head->right=tail;
    tail->left=tail;
    tail->right=tail;
}//헤드와 테일 연결

void my_push1(OpStack* s,char c)
{
    s->op_stack[++(s->top)]=c;
}//탑 증가 후 스택에 c 저장

char my_pop1(OpStack* s)
{
    return s->op_stack[(s->top)--];
}//스택을 받고 값을 리턴하고 탑을 감소

void my_push2(TreeStack *s, Node * t)
{
    s->ptr_stack[++(s->top)]=t;
}//탑 증가 후 t 넣어줌

Node* my_pop2(TreeStack* s)
{
    return s->ptr_stack[(s->top)--];
}//값을 리턴하고 탑 감소

int is_operator(int k)
{
    return(k=='+' || k=='-' || k=='*' || k=='/');
}//연산자가 맞다면 1을 리턴

int precedence(int op)
{
    if(op=='(') return 0;
    if(op=='+' || op=='-') return 1;
    if(op=='*' || op=='/') return 2;
    else return 3; 
}//우선순위를 표시해 리턴

void postfix(OpStack* s, char* dst, char* src)
{
    while(*src) //0을 나오면 빠져나오지만 그 전까지 모든 문자열 읽어오기
    {
        if(*src=='(')
        {
            my_push1(s,*src); src++;
        } //캐릭터 값에 따라 push를 함
        else if(*src==')')
        {
            while(s->op_stack[s->top] != '(')
            {
                *dst++ = my_pop1(s); *dst++=' ';
            }
            my_pop1(s); src++;
        }//)일 때 pop을 실행한다 ( 나올때까지
        else if (is_operator(*src))
        {
            while(s->top>=0 && precedence(s->op_stack[s->top])>=precedence(*src))
            {
                *dst++ = my_pop1(s); *dst++ = ' ';
            }
            my_push1(s, *src); src++;
        }//연산자일 때 앞에값이 없거나 연산자 다시 만날때까지 실행
        else if(*src>='0' && *src<='9')
        {
            do {
                *dst++=*src++;
            } while(*src>='0' && *src<='9');
            *dst++= ' ';
        }//숫자가 있을 때 소스에 있는 숫자를 dst로 카피
        else
        {
            src++;
        } // 다 아닐때 src 증가 시켜서 다음 문자열 받기
    }
    while(s->top>=0){
        *dst++=my_pop1(s);
        *dst++=' ';
    } //top이 0보다 크거나 같으면 dst증가 s는 팝을 실행
    dst--;//dst감소
    *dst=0; //dst0으로 변경 
}

Node* make_parse_tree(TreeStack* s,char* p)
{
    Node* t;
    int i;
    while (*p)
    {
        while(*p==' ')
        {
            p++;
        }
        t=(Node*)malloc(sizeof(Node));
        if (*p>= '0' && *p<='9')
        {
            i=0;
            do{
                i=i*10 + *p - '0'; //이전에 들어온 숫자가 10의 자리로 판단 따라서 10을 곱한 후 지금 들어온 문자를 더해줌 
                p++;//'9'-'0'을 빼면 9가 나옴 아스키코드 이용 연속된 숫자일 때 사용 문자를 숫자로 변경
            } while(*p>='0' && *p<='9');
            t->operand=i; //*p; 방금 읽은 문자를 오퍼랜드에 저장
            t->op_flag=0; //operand로 모드 알려줌
            t->left=tail;
            t->right=tail;
        }
        else if (is_operator(*p)) //연산자이면
        {
            t->optr=*p;//optr에 저장
            t->op_flag=1; //operator 모드로 바꿔줌
            t->right=my_pop2(s); //왼쪽 오른쪽 자식 pop수행
            t->left=my_pop2(s);
        }
        my_push2(s,t);
        p++;
    }
    return my_pop2(s); //마지막은 루트노드 이기 때문에 루트노드 빠져나옴
}

void traverse(Node* t) //재귀호출을 사용한 traverse
{
    if(t!=tail)
    {
        traverse(t->left);
        traverse(t->right);
        visit(t); //부모노드를 visit함
    }
}

void visit(Node *t)
{
    int r;

    if (t->op_flag==1) //만약 1이면 연산자가 있으므로 연산 수행
    {
        if(t->optr=='+')
        {
            r=t->left->operand+t->right->operand;
        }
        else if (t->optr=='-')
        {
            r=t->left->operand-t->right->operand;
        }
        else if(t->optr=='*')
        {
            r=t->left->operand*t->right->operand;
        }
        else if(t->optr=='/')
        {
            r=t->left->operand/t->right->operand;
        }
        t->operand=r; //오퍼랜드에 연산이 끝나면 그 값을 저장
        printf(" %c (%d) ", t->optr,r);//따라서 두개 다 출력
    }
    else //flag==0일 때 그냥 숫자 바로 출력
    {
        printf(" %d ",t->operand);
    }
}