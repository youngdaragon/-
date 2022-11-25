#include <stdio.h>
#include <stdlib.h>

typedef struct _student
{
    int student_id;
    const char *name;
    int class_register[100];
    int score[100];
} Student;
//�л� ����ü ����
typedef struct _snode
{
    struct _student* link;
    struct _snode* next;
    struct _snode* prev;
} Snode; 
//node�� �������ִ� ����ü link �� �л�����ü �������� ������ ���� node�� �ּҸ� ����Ŵ

typedef struct _class
{
    int class_id;
    const char* class_name;
    int num_registered;
    int avg_score;
    Snode* student_head, * student_tail;
} Class;
//class�� ����ü ���ο��� Ŭ���� id, �̸�, ����� ����ߴ°� �������, 
// head, tail�� �Է��ؼ� student_linked list���� ������ ������

void init_Snode(Snode **head, Snode **tail, int num_class);
void init_Student(Student* studentarray, int num_student);
void init_Class(Class *c, Snode** student_head, Snode** student_tail, int num_class);
void apply_class(Class *c, Student* studentarray,int num_student, int num_class);
Snode* add_student_to_class(Class* c, Student* studentarray, int s_id);
void print_info(Class* c, int num_class);
//�Լ� ����
int main()
{
    // int num_student=2;
    // int num_class=2;
    int num_student;
    printf("number of student: ");
    scanf("%d",&num_student);
    int num_class;
    printf("number of class: ");
    scanf("%d",&num_class);
    //�л� ���� class ���� �������� �޾��ش�.
    Student *studentarray;
    Snode** student_head, ** student_tail;
    Class* c;
    //����ü�� ���ؼ� ������ ����ü�� �������ش�.
    student_head=(Snode**)malloc(num_class*sizeof(Snode *));
    for (int i=0; i<num_class; i++)
    {
        student_head[i]=(Snode*)malloc(sizeof(Snode));
    }
    //student_head �κп���  malloc�� �̿��� num_class��ŭ �����Ҵ��� �����ְ� head�� �� ũ���
    //�ٽ� �� num_class��ŭ �����Ҵ� ��Ų head�� student_head�� �����Ҵ� �����ش�.
    student_tail=(Snode**)malloc(num_class*sizeof(Snode *));
    for (int i=0; i<num_class; i++)
    {
        student_tail[i]=(Snode*)malloc(sizeof(Snode));
    }
    //tail�� head�� ���������� �����Ҵ� �����ش�.

    c=(Class*)malloc(num_class*sizeof(Class));
    studentarray=(Student*)malloc(num_student*sizeof(Student));
    //class ����ü�� ���� c�� �����Ҵ��� �̿��� ������ش�.
    //studentarray�� �����Ҵ��� �̿��� num_student��ŭ ������ش�. 

    init_Snode(student_head,student_tail,num_class);
    init_Class(c, student_head, student_tail, num_class);
    init_Student(studentarray, num_student);
    apply_class(c,studentarray,num_student,num_class);
    print_info(c,num_class);
    //���� �Լ��� ������ �������� ���� �־��ش�.

    return 0;
}

void init_Snode(Snode** head,Snode** tail,int num_class)
{
    for(int i=0; i<num_class; i++)
    {
        head[i] ->next =tail[i];
        head[i]->prev=head[i];
        tail[i]->next=tail[i];
        tail[i]->prev=head[i];
    }
}//Snode�� �ʱⰪ�� ���� �� �� ���� ���� ������ head�� tail�� �������ش�.
//head ������ tail, ���� head, tail ������ head, ������ tail.

void init_Student(Student *studentarray,int num_student)
{
    for (int i=0; i<num_student; i++)
    {
        studentarray[i].student_id=i+1;
        printf("name of student: ");
        scanf("%s",&studentarray[i].name);
    }
}//student�� ����ü�� num_student��ŭ studentarray�� ������ش�.

void init_Class(Class *c, Snode** student_head, Snode **student_tail, int num_class)
{
    for (int i=0; i<num_class; i++)
    {c[i].class_id=i+1;
    printf("class of name: ");
    scanf("%s",&c[i].class_name);
    c[i].num_registered=0;
    c[i].avg_score=0;
    c[i].student_head=student_head[i];
    c[i].student_tail=student_tail[i];}
}//����� ���� class����ü�� ����� �ش�. ������ ���� �־��ش�.

void apply_class(Class* c, Student *studentarray, int num_student, int num_class)
{
    int apply;
    for(int s=0; s<num_student; s++)
    {
        printf("\n Apply classes student name : %s.",&studentarray[s].name);
        printf("input 1 if applying 0 otherwise \n");
        for (int j=0; j<num_class; j++)
        {
            printf("\n class %s : ",&c[j].class_name);
            scanf("%d",&apply); //������ �ϸ� 1, �ƴϸ� 0
            if (apply==1)
            {
                printf("\n score: ");
                scanf("%d",&studentarray[s].score[j]);//������ �޴� �ڵ�
                studentarray[s].class_register[j]=1;
                add_student_to_class(&c[j],studentarray,s);
            }//������ �� ��쿡�� �������� ���� 1�� ���� add_student_to_class�� class ����ü ����
            //studentarray�� �� ��° student������ �־��ش�.
            else
            {
                studentarray[s].class_register[j]=0;
            }//������ ���Ұ�쿡�� studentarray���� 0���� ���� �ǰ� ������.
        }
    }
}

Snode* add_student_to_class(Class *c, Student* studentarray, int s_id)
{
    Snode* node; //����ü�� �������ش�.
    c->num_registered++; //c�� num_registered�� 1 �߰������ش�.
    node=(Snode*)malloc(sizeof(Snode)); //node�� �����Ҵ��� �����ش�.
    node->link=&studentarray[s_id];// node�� link �κ��� studentarray[s_id]������ ��� �л��� �迭 index�� ����Ų��.
    c->student_head->next->prev=node;// head�� �����κ��� �� ���� node�� �����Ѵ�.
    node->next=c->student_head->next;//����� ���� �κκ��� head�� �����κ����� �����Ѵ�.
    node->prev=c->student_head;//����� ���� ���� �����Ѵ�.
    c->student_head->next=node;//����� ������ ���� �����Ѵ�.
}

void print_info(Class* c, int num_class)
{
    Snode* ptr;

    for(int j=0; j<num_class; j++)
    {
        // ptr=c[j].student_head->next;
        ptr=c[j].student_tail->prev; //tail���� �����ؼ� prev�� �Ųٷ� ���ش�.
        printf("registered students for class name %s are: \n",&c[j].class_name);
        int k=0;
        while(ptr != c[j].student_head) //student_head�� ptr�϶� �����.
        {
            k++;
            printf("(%d-th id=%d name=%s score=%d) \n", 
                    k,ptr->link->student_id,&(ptr->link->name),ptr->link->score[j]); 
            c[j].avg_score+=ptr->link->score[j];
            //���������� linked list�� ���鼭 ���
            ptr=ptr->prev; //ptr�� ���� prev�� ����Ű���� �Ѵ�.
            //�̷� ��� �й������� ������ ���� �����ϴ�. (�� ������ �й� �������� ���)
        } 
        c[j].avg_score=c[j].avg_score/c[j].num_registered;//��� score�� ����Ѵ�.
        printf("%s is avg_score: %d \n",&c[j].class_name,c[j].avg_score);//��� score�� ����Ѵ�.
    }
}
