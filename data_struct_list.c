#include <stdio.h>
#include <stdlib.h>

typedef struct _student
{
    int student_id;
    char *name;
    int class_register[5];
} Student;

typedef struct _snode
{
    struct _student* link;
    struct _snode* next;
    struct _snode* prev;
} Snode;

typedef struct _classes
{
    int class_id;
    char *class_name;
    int num_registered;
    int avg_score;
    Snode* student_head, *student_tail;
} Classes;

void init_Snode(Snode **head, Snode **tail, int num_class);
void init_Student(Student* studentarray);
void init_Class(Classes *classes, Snode** student_head, Snode** student_tail);
void apply_class(Classes *classes, Student* studentarray, int num_student,int num_class);
Snode* add_student_to_class(Classes *classes, Student* studentarray, int s_id);
void print_info(Classes *classes, int num_class);

int main()
{
    int num_student=2;
    int num_class=2;
    Student *studentarray;
    Snode** student_head, **student_tail;
    Classes* classes;

    student_head = (Snode**)malloc(num_class*sizeof(Snode *));
    for (int i=0; i<num_class; i++)
        student_head[i]=(Snode*)malloc(sizeof(Snode));

    student_tail=(Snode**)malloc(num_class*sizeof(Snode*));
    for (int i=0; i<num_class; i++)
        student_tail[i]=(Snode*)malloc(sizeof(Snode));
    
    classes=(Classes*)malloc(num_class*sizeof(Classes));
    studentarray=(Student*)malloc(num_student*sizeof(Student));

    init_Snode(student_head, student_tail, num_class);
    init_Class(classes, student_head, student_tail);
    init_Student(studentarray);
    apply_class(classes,studentarray,num_student,num_class);
    print_info(classes,num_class);

    return 0;
}

void init_Snode(Snode** head, Snode** tail, int num_class)
{
    for(int i=0; i<num_class; i++)
    {
        head[i]->next=tail[i];
        head[i]->prev=head[i];
        tail[i]->next=tail[i];
        tail[i]->prev=head[i];
    }
}

void init_Student(Student* studentarray)
{
    studentarray[0].student_id=1;
    studentarray[0].name="kim";
    studentarray[1].student_id=2;
    studentarray[1].name="lee";
}

void init_Class(Classes *classes, Snode** student_head, Snode **student_tail)
{
    classes[0].class_id=0;
    classes[0].class_name="math";
    classes[0].num_registered=0;
    classes[0].avg_score=0;
    classes[0].student_head=student_head[0];
    classes[0].student_tail=student_tail[0];

    classes[1].class_id=1;
    classes[1].class_name="eng";
    classes[1].num_registered=0;
    classes[1].avg_score=0;
    classes[1].student_head=student_head[1];
    classes[1].student_tail=student_tail[1];
}

void apply_class(Classes* classes ,Student *studentarray, int num_student,int num_class){
    int apply;
    for(int s=0; s<num_student; s++)
    {
        printf(" \n Apply classes for student with id %d ",s);
        printf(" Input 1 if applying 0 otherwise \n");
        for (int c=0; c<num_class; c++)
        {
            printf("\n class %s : ",classes[c].class_name);
            scanf("%d",&apply);
            if(apply==1)
            {
                studentarray[s].class_register[c]=1;
                add_student_to_class(&classes[c],studentarray,s);
            }
            else
                studentarray[s].class_register[c]=0;           
        }
    }
}

Snode *add_student_to_class(Classes* classes, Student *studentarray, int s_id)
{
    Snode* node;
    classes->num_registered++;
    classes->student_head;
    node=(Snode*)malloc(sizeof(Snode));
    node->link =&studentarray[s_id];
    classes->student_head->next->prev=node;
    node->next=classes->student_head->next;
    node->prev=classes->student_head;
    classes->student_head->next=node;
}

void print_info(Classes *classes, int num_class)
{
    Snode* ptr;

    for (int c=0; c<num_class; c++)
    {
        ptr=classes[c].student_head->next;
        printf(" registered students for class id %d are :\n",c);
        int k=0;
        while(ptr != classes[c].student_tail)
        {
            k++;
            printf(" ( %d-th id=%d name= %s ) \n", k, ptr->link->student_id, ptr->link->name);
            ptr=ptr->next;
        }

    }
}