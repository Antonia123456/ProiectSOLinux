#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
void citire_director(char *director)
{
DIR *dir;
dir=opendir(director);
if(dir==NULL)
{
perror("error open director");
exit(1);
}

struct dirent *entry;
struct stat file;
char path[300];
while((entry=readdir(dir))!=NULL)
{
if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
{
sprintf(path,"%s/%s",director,entry->d_name);

if(stat(path,&file)==-1)
{
perror("error stat");
exit(1);
}
if(S_ISREG(file.st_mode))
{
if(strstr(path,".bmp"))
{
printf("bmp\n");

int fd;
fd=open(path,O_RDONLY);
if(fd==-1)
{
perror("error.open");
exit(1);
}
int latime,inaltime;
lseek(fd,18,SEEK_SET);
if(read(fd,&latime,sizeof(int))!=sizeof(int))
{
perror("error.close");
if(close(fd)==-1){
perror("error.open");
exit(1);
}
exit(1);
}
if(read(fd,&inaltime,sizeof(int))!=sizeof(int))
{
perror("error.close");
if(close(fd)==-1){
perror("error.open");
exit(1);
}
exit(1);
}
printf("latime %d\n",latime);
printf("inaltime %d\n",inaltime);
if(close(fd)==-1){
perror("error.open");
exit(1);
}
}
else
printf("regular\n");
}
printf("%s\n",path);
}
}

if(closedir(dir)==-1){
perror("error close dir");
exit(1);
}
}
int main(int argc,char*argv[])
{
/*char *file=argv[1];
int fd;
fd=open(file,O_RDONLY);
if(fd==-1)
{
perror("error.open");
exit(1);
}
int latime;
lseek(fd,18,SEEK_SET);
if(read(fd,&latime,sizeof(int))!=sizeof(int))
{
perror("error.close");
if(close(fd)==-1){
perror("error.open");
exit(1);
}
exit(1);
}
printf("latime %d\n",latime);
if(close(fd)==-1){
perror("error.open");
exit(1);
}*/
if(argc!=2)
{
perror("incorect number of args");
exit(1);
}
citire_director(argv[1]);
return 0;
}
//fol structura stat pt fiecare fis, verif cu st_mode daca e regulat,legat simbolica sau director si afisam atr pt fiecare fis, trebuie ignorate cele doua .. si . cu un strcmp prob
//stat trb sa primeasca intreaga cale carte fisier cu sprintf director/numefisier
