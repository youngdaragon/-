#include<stdio.h>
#include<stdlib.h>

typedef struct _node
{
    int operand; //�ǿ�����
    int optr; //������
    int op_flag; //1 if operator (operan�� optr���� 1�̸� optr)
    struct _node* left; //���������� ������ �ڼ� ��� �ΰ� �����
    struct _node* right; //���������� ������ �ڼ� ��� �ΰ� �����
} Node;

typedef struct _stack
{
    int top; //������ ������ ���Ǳ� ������ top�� ���� ȥ���� ��� ����
    Node *ptr_stack[10]; //����� �����͸� �����ϱ����� ����
} TreeStack; 

typedef struct _fix
{
    int top;
    char op_stack[10];
} OpStack; //�����ڸ� �����ϴ� ���� ĳ���� ���� +,-,*,/

Node* head,*tail; //tree�� ���� ������ ����

void init_tree(void); //Ʈ�� �ʱ� ����
Node* make_parse_tree(TreeStack* s, char* p); //Ʈ�� �����
void my_push2(TreeStack* s, Node *t);//Ǫ�� ���� tree
void my_push1(OpStack* s, char c);//Ǫ�� ���� optr
char my_pop1(OpStack* s);//������
Node* my_pop2(TreeStack* s);//Ʈ�� ����
int is_operator(int k);//�����ڳ�
int precedence(int op);//���̳�
void postfix(OpStack *s, char *dest, char* src);//����ǥ��� �ٲٴ°� ����
void traverse(Node* t);//�θ� ������ �湮 �ϴ� traverse
void visit(Node* t); //����� �ϰ� ����� ��

int main()
{
    char src[100], dst[100];
    OpStack stack1;
    TreeStack stack2;

    stack1.top=-1; //init
    stack2.top=-1;

    init_tree();

    printf(" Enter infix formula \n"); //����ǥ��� �Է�
    gets(src); //�ҽ��� ����
    printf(" Infix formula : %s \n", src);
    postfix(&stack1, dst, src); //����ǥ������� �ٲ�
    printf( "Postfix formula : %s \n",dst);
    head->right=make_parse_tree(&stack2,dst);//����ǥ������� ǥ�õ� ���� ����2�� �̿��� ��Ʈ��� �ޱ� 
    printf(" traverse of tree \n");
    traverse(head->right); //��Ʈ��� �ּ� �ֱ� traverse����
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
}//���� ���� ����

void my_push1(OpStack* s,char c)
{
    s->op_stack[++(s->top)]=c;
}//ž ���� �� ���ÿ� c ����

char my_pop1(OpStack* s)
{
    return s->op_stack[(s->top)--];
}//������ �ް� ���� �����ϰ� ž�� ����

void my_push2(TreeStack *s, Node * t)
{
    s->ptr_stack[++(s->top)]=t;
}//ž ���� �� t �־���

Node* my_pop2(TreeStack* s)
{
    return s->ptr_stack[(s->top)--];
}//���� �����ϰ� ž ����

int is_operator(int k)
{
    return(k=='+' || k=='-' || k=='*' || k=='/');
}//�����ڰ� �´ٸ� 1�� ����

int precedence(int op)
{
    if(op=='(') return 0;
    if(op=='+' || op=='-') return 1;
    if(op=='*' || op=='/') return 2;
    else return 3; 
}//�켱������ ǥ���� ����

void postfix(OpStack* s, char* dst, char* src)
{
    while(*src) //0�� ������ ������������ �� ������ ��� ���ڿ� �о����
    {
        if(*src=='(')
        {
            my_push1(s,*src); src++;
        } //ĳ���� ���� ���� push�� ��
        else if(*src==')')
        {
            while(s->op_stack[s->top] != '(')
            {
                *dst++ = my_pop1(s); *dst++=' ';
            }
            my_pop1(s); src++;
        }//)�� �� pop�� �����Ѵ� ( ���ö�����
        else if (is_operator(*src))
        {
            while(s->top>=0 && precedence(s->op_stack[s->top])>=precedence(*src))
            {
                *dst++ = my_pop1(s); *dst++ = ' ';
            }
            my_push1(s, *src); src++;
        }//�������� �� �տ����� ���ų� ������ �ٽ� ���������� ����
        else if(*src>='0' && *src<='9')
        {
            do {
                *dst++=*src++;
            } while(*src>='0' && *src<='9');
            *dst++= ' ';
        }//���ڰ� ���� �� �ҽ��� �ִ� ���ڸ� dst�� ī��
        else
        {
            src++;
        } // �� �ƴҶ� src ���� ���Ѽ� ���� ���ڿ� �ޱ�
    }
    while(s->top>=0){
        *dst++=my_pop1(s);
        *dst++=' ';
    } //top�� 0���� ũ�ų� ������ dst���� s�� ���� ����
    dst--;//dst����
    *dst=0; //dst0���� ���� 
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
                i=i*10 + *p - '0'; //������ ���� ���ڰ� 10�� �ڸ��� �Ǵ� ���� 10�� ���� �� ���� ���� ���ڸ� ������ 
                p++;//'9'-'0'�� ���� 9�� ���� �ƽ�Ű�ڵ� �̿� ���ӵ� ������ �� ��� ���ڸ� ���ڷ� ����
            } while(*p>='0' && *p<='9');
            t->operand=i; //*p; ��� ���� ���ڸ� ���۷��忡 ����
            t->op_flag=0; //operand�� ��� �˷���
            t->left=tail;
            t->right=tail;
        }
        else if (is_operator(*p)) //�������̸�
        {
            t->optr=*p;//optr�� ����
            t->op_flag=1; //operator ���� �ٲ���
            t->right=my_pop2(s); //���� ������ �ڽ� pop����
            t->left=my_pop2(s);
        }
        my_push2(s,t);
        p++;
    }
    return my_pop2(s); //�������� ��Ʈ��� �̱� ������ ��Ʈ��� ��������
}

void traverse(Node* t) //���ȣ���� ����� traverse
{
    if(t!=tail)
    {
        traverse(t->left);
        traverse(t->right);
        visit(t); //�θ��带 visit��
    }
}

void visit(Node *t)
{
    int r;

    if (t->op_flag==1) //���� 1�̸� �����ڰ� �����Ƿ� ���� ����
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
        t->operand=r; //���۷��忡 ������ ������ �� ���� ����
        printf(" %c (%d) ", t->optr,r);//���� �ΰ� �� ���
    }
    else //flag==0�� �� �׳� ���� �ٷ� ���
    {
        printf(" %d ",t->operand);
    }
}