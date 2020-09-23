extern char _free_space[];
extern char _STACK_SIZE[];
extern char _HEAP_SIZE[];
extern char _stack_end[];
extern char _stack[];
extern char _heap[];
extern char _heap_end[];
extern char __bss_end[];
extern char __bss_start[];
extern char __sbss_end[];
extern char __sbss_start[];


char *test1=(char *)&_free_space;
char *test3=(char *)&_STACK_SIZE;
char *test4=(char *)&_HEAP_SIZE;
char *test5=(char *)&_stack_end;
char *test6=(char *)&_stack;
char *test7=(char *)&_heap;
char *test8=(char *)&_heap_end;
char *test9=(char *)&__bss_end;
char *test10=(char *)&__bss_start;
char *test11=(char *)&__sbss_end;
char *test12=(char *)&__sbss_start;




int c1,c2,c3,c4,c5;
int *pointer;

int main()
{
printf("\n\n\n\n\n\nTest Running");
printf("\n %x %x %x %x %x %x %x %x %x %x %x",test1,test3,test4,test5,test6,test7,test8,test9,test10,test11,test12);
printf("\n %x %d %x %x %x %x %x",&pointer,c1,&c1,&c2,&c3,&c4,&c5);
static int b1,b2,b3,b4,b5;
printf("\n %x %x %x %x %x",&b1,&b2,&b3,&b4,&b5);
int a1,a2,a3,a4,a5;
printf("\n %x %x %x %x %x",&a1,&a2,&a3,&a4,&a5);
int **a;
int *b,*c,*d;
int i=0;int n;
//printf("\n Enter Size of Array");
n= 25;
printf("\n size of array %d %x",n,&n);
b=malloc(n*sizeof(int))	;

//printf("\n\n\n\n\n\n\n %x %x %x %x %x %x %x  \n\n\n\n\n",test1,test3,test4,test5,test6,test7,test8);

printf("\n B's address %x %x",b,&b);
for(i=0;i<n;i++)
{
b[i]=i+1;
}
for(i=0;i<n;i++)
{
printf("\n b[%d]'s address is : %x",i,&b[i]);
printf("\n b[%d]'s value is : %d",i,b[i]);
}
free(b);
printf("\nArray B is freed");
printf("\nb=%x",b);
printf("\n size of array %d",n);
c=malloc(n*sizeof(int))	;
printf("\n The previously allocated block from the free list is used to allocate Array C since they are of the same size");

printf("\n C's address %x",c);
for(i=0;i<n;i++)
{
c[i]=i+1;
}
for(i=0;i<n;i++)
{
printf("\n c[%d]'s address is : %x",i,&c[i]);
printf("\n c[%d]'s value is : %d",i,c[i]);
}
free(c);
printf("\nc=%x",c);
int x;
x=30;
printf("\n\n\nx address is %x",&x);
d=malloc(x*sizeof(int));
printf("\n D's address %x",d);
for(i=0;i<x;i++)
{
printf("\nSetting value %d , %x",i+1,&d[i]); 
d[i]=i+1;
}
for(i=0;i<x;i++)
{
printf("\n d[%d]'s address is : %x",i,&d[i]);
printf("\n d[%d]'s value is : %d",i,d[i]);
}
free(d);
printf("\nd=%x",d);

/*{
b=malloc(1024);

//printf("\n %d %x",b[(128*1024)+1],&b[(128*1024)-1]);
free(b);
}*/
//free(c);
}
