#include <stdio.h>
#include <stdlib.h>

typedef struct _student
{
    int student_id;
    const char *name;
    int class_register[100];
    int score[100];
} Student;
//학생 구조체 선언
typedef struct _snode
{
    struct _student* link;
    struct _snode* next;
    struct _snode* prev;
} Snode; 
//node를 선언해주는 구조체 link 는 학생구조체 나머지는 이전과 다음 node의 주소를 가르킴

typedef struct _class
{
    int class_id;
    const char* class_name;
    int num_registered;
    int avg_score;
    Snode* student_head, * student_tail;
} Class;
//class의 구조체 내부에는 클래스 id, 이름, 몇명이 등록했는가 평균점수, 
// head, tail을 입력해서 student_linked list에서 정보를 가져옴

void init_Snode(Snode **head, Snode **tail, int num_class);
void init_Student(Student* studentarray, int num_student);
void init_Class(Class *c, Snode** student_head, Snode** student_tail, int num_class);
void apply_class(Class *c, Student* studentarray,int num_student, int num_class);
Snode* add_student_to_class(Class* c, Student* studentarray, int s_id);
void print_info(Class* c, int num_class);
//함수 선언
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
    //학생 수와 class 수를 동적으로 받아준다.
    Student *studentarray;
    Snode** student_head, ** student_tail;
    Class* c;
    //구조체에 의해서 각각의 구조체를 선언해준다.
    student_head=(Snode**)malloc(num_class*sizeof(Snode *));
    for (int i=0; i<num_class; i++)
    {
        student_head[i]=(Snode*)malloc(sizeof(Snode));
    }
    //student_head 부분에서  malloc을 이용해 num_class만큼 동적할당을 시켜주고 head의 각 크기는
    //다시 각 num_class만큼 동적할당 시킨 head를 student_head를 동적할당 시켜준다.
    student_tail=(Snode**)malloc(num_class*sizeof(Snode *));
    for (int i=0; i<num_class; i++)
    {
        student_tail[i]=(Snode*)malloc(sizeof(Snode));
    }
    //tail도 head와 마찬가지로 동적할당 시켜준다.

    c=(Class*)malloc(num_class*sizeof(Class));
    studentarray=(Student*)malloc(num_student*sizeof(Student));
    //class 구조체를 담을 c를 동적할당을 이용해 만들어준다.
    //studentarray를 동적할당을 이용해 num_student만큼 만들어준다. 

    init_Snode(student_head,student_tail,num_class);
    init_Class(c, student_head, student_tail, num_class);
    init_Student(studentarray, num_student);
    apply_class(c,studentarray,num_student,num_class);
    print_info(c,num_class);
    //각각 함수에 위에서 정의해준 값을 넣어준다.

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
}//Snode의 초기값을 정해 줄 때 각각 과목 수마다 head와 tail을 정의해준다.
//head 다음은 tail, 전은 head, tail 이전은 head, 다음은 tail.

void init_Student(Student *studentarray,int num_student)
{
    for (int i=0; i<num_student; i++)
    {
        studentarray[i].student_id=i+1;
        printf("name of student: ");
        scanf("%s",&studentarray[i].name);
    }
}//student의 구조체의 num_student만큼 studentarray를 만들어준다.

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
}//과목수 마다 class구조체를 만들어 준다. 각각의 값을 넣어준다.

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
            scanf("%d",&apply); //수강을 하면 1, 아니면 0
            if (apply==1)
            {
                printf("\n score: ");
                scanf("%d",&studentarray[s].score[j]);//점수를 받는 코드
                studentarray[s].class_register[j]=1;
                add_student_to_class(&c[j],studentarray,s);
            }//수강을 할 경우에는 레지스터 값에 1이 들어가고 add_student_to_class에 class 구조체 값과
            //studentarray와 몇 번째 student인지를 넣어준다.
            else
            {
                studentarray[s].class_register[j]=0;
            }//수강을 안할경우에는 studentarray에만 0값이 저장 되고 끝난다.
        }
    }
}

Snode* add_student_to_class(Class *c, Student* studentarray, int s_id)
{
    Snode* node; //구조체를 선언해준다.
    c->num_registered++; //c에 num_registered를 1 추가시켜준다.
    node=(Snode*)malloc(sizeof(Snode)); //node를 동적할당을 시켜준다.
    node->link=&studentarray[s_id];// node의 link 부분은 studentarray[s_id]수강을 듣는 학생의 배열 index를 가르킨다.
    c->student_head->next->prev=node;// head의 다음부분의 그 전을 node로 지정한다.
    node->next=c->student_head->next;//노드의 다음 부부분은 head의 다음부분으로 지정한다.
    node->prev=c->student_head;//노드의 전은 헤드로 지정한다.
    c->student_head->next=node;//헤드의 다음은 노드로 지정한다.
}

void print_info(Class* c, int num_class)
{
    Snode* ptr;

    for(int j=0; j<num_class; j++)
    {
        // ptr=c[j].student_head->next;
        ptr=c[j].student_tail->prev; //tail에서 시작해서 prev로 거꾸로 해준다.
        printf("registered students for class name %s are: \n",&c[j].class_name);
        int k=0;
        while(ptr != c[j].student_head) //student_head가 ptr일때 멈춘다.
        {
            k++;
            printf("(%d-th id=%d name=%s score=%d) \n", 
                    k,ptr->link->student_id,&(ptr->link->name),ptr->link->score[j]); 
            c[j].avg_score+=ptr->link->score[j];
            //내부정보를 linked list를 돌면서 출력
            ptr=ptr->prev; //ptr이 이전 prev를 가르키도록 한다.
            //이럴 경우 학번순으로 나오는 것이 가능하다. (그 전에는 학번 역순으로 출력)
        } 
        c[j].avg_score=c[j].avg_score/c[j].num_registered;//평균 score를 계산한다.
        printf("%s is avg_score: %d \n",&c[j].class_name,c[j].avg_score);//평균 score를 출력한다.
    }
}
