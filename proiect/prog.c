#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<sys/wait.h>
#include<sys/types.h>

void cerinte(char *path,char* name,int out_fd,int *lines_count)
{
  char str[3000];
  char buff[200];//buffer pt transformarea val din intregi in sir de caractere
  struct stat st_file;
  if(stat(path,&st_file) == -1)
    {
      perror("stat file error");
      exit(1);
    }
  struct stat st_link;
  if(lstat(path,&st_link) == -1)
    {
      perror("stat link error");
      exit(1);
    }
  //adaugare nume
  if(S_ISLNK(st_link.st_mode))
    {
      strcpy(str,"nume legatura: ");
      *lines_count=6;
    }
  else
    {
      if(S_ISREG(st_file.st_mode) || (S_ISREG(st_file.st_mode) && strstr(path,".bmp")))
	strcpy(str,"nume fisier: ");
  
      if(S_ISDIR(st_file.st_mode))
	{
	  strcpy(str,"nume director: ");
	  *lines_count=5;
	}
    }
  strcat(str,name);
  strcat(str,"\n");

  //adaugare lungime si inaltime pentru fisierele bmp
  if(S_ISREG(st_file.st_mode)&& strstr(path,".bmp"))
    {
      int fd;
      fd=open(path,O_RDONLY);
      if(fd==-1)
	{
	  perror("error.open");
	  exit(1);
	}
      int lungime,inaltime;
      lseek(fd,18,SEEK_SET);
      if(read(fd,&inaltime,sizeof(int))!=sizeof(int))
	{
	  perror("error.read");
	  if(close(fd)==-1){
	    perror("error.close");
	    exit(1);
	  }
	  exit(1);
	}
      if(read(fd,&lungime,sizeof(int))!=sizeof(int))
	{
	  perror("error.read");
	  if(close(fd)==-1){
	    perror("error.close");
	    exit(1);
	  }
	  exit(1);
	}
      
      strcat(str,"inaltime: ");
      sprintf(buff,"%d\n",inaltime);
      strcat(str,buff);
      strcat(str,"lungime: ");
      sprintf(buff,"%d\n",lungime);
      strcat(str,buff);

      *lines_count=10;

      
    }
  
  //dimensiunea pentru legatura simbolica
  if(S_ISLNK(st_link.st_mode))
    {
      sprintf(buff,"dimensiune legatura: %ld\n",st_link.st_size);
      strcat(str,buff);
    }
  
  //dimensiune fisier pt fisier regulat,bmp sau legatura simbolica 
  if(S_ISREG(st_file.st_mode) || (S_ISREG(st_file.st_mode)&& strstr(path,".bmp"))|| S_ISLNK(st_file.st_mode) )
    {
      strcat(str,"dimensiune: ");
      sprintf(buff,"%ld\n",st_file.st_size);
      strcat(str,buff);
    }
  
  //ID utilizator pt fisier regulat,bmp sau director
  if(!(S_ISLNK(st_link.st_mode)))
    if(S_ISREG(st_file.st_mode) || (S_ISREG(st_file.st_mode)&& strstr(path,".bmp")) || S_ISDIR(st_file.st_mode))
      {
	strcat(str,"identificatorul utilizatorului: ");
	sprintf(buff,"%d\n",st_file.st_uid);
	strcat(str,buff);
      }
  
  //timpul ultimei modificari pt fisier bmp sau regulat
  if(!(S_ISLNK(st_link.st_mode)))
    if(S_ISREG(st_file.st_mode) || (S_ISREG(st_file.st_mode)&& strstr(path,".bmp")))
      {
	strcat(str,"timpul ultimei modificari: ");
	strcat(str,ctime(&st_file.st_mtime));
      }
  
  //contorul de legaturi pt fisier bmp sau regulat
  if(!(S_ISLNK(st_link.st_mode)))//ca sa nu intre si pt legatura simbolica
    if(S_ISREG(st_file.st_mode) || (S_ISREG(st_file.st_mode)&& strstr(path,".bmp")))
      {
	strcat(str,"contorul de legaturi: ");
	sprintf(buff,"%ld\n",st_file.st_nlink);
	strcat(str,buff);
      }
  
  //line counterul pt fisiere regulate dar nu bmp
  if(!(S_ISLNK(st_link.st_mode)))
    if(S_ISREG(st_file.st_mode) && !(S_ISREG(st_file.st_mode)&& strstr(path,".bmp")))
      *lines_count=8;
      
  //drepturi de acces user
  //drepturi de acces grup
  //drepturi de acces altii
  strcat(str,"drepturi de acces user: ");
  sprintf(buff,"%s%s%s\n", st_file.st_mode & S_IRUSR ? "R" : "-",st_file.st_mode & S_IWUSR ? "W" : "-",st_file.st_mode & S_IXUSR ? "X" : "-");
  strcat(str,buff);
  strcat(str,"drepturi de acces grup: ");
  sprintf(buff,"%s%s%s\n", st_file.st_mode & S_IRGRP ? "R" : "-",st_file.st_mode & S_IWGRP ? "W" : "-",st_file.st_mode & S_IXGRP ? "X" : "-");
  strcat(str,buff);
  strcat(str,"drepturi de acces altii: ");
  sprintf(buff,"%s%s%s\n",st_file.st_mode & S_IROTH ? "R" : "-",st_file.st_mode & S_IWOTH ? "W" : "-",st_file.st_mode & S_IXOTH ? "X" : "-");
  strcat(str,buff);
  strcat(str,"\n");

  
 
  if(write(out_fd,str,strlen(str))==-1)
    {
      perror("error write file");
      exit(1);
    }
  
}

void writeStatistic(int st_fd,int childPid,int lines)
{
  char buff[200];
  sprintf(buff,"Procesul fiu %d are %d linii\n",childPid,lines);
  if(write(st_fd,buff,strlen(buff))==-1)
    {
      perror("error write file");
      exit(1);
    }
}


void citire_director(const char *director,const char *iesire)
{
  DIR *dir;
  struct dirent *entry=NULL;
  
  dir=opendir(director);
  if(dir==NULL)
    {
      perror("error open director");
      exit(1);
    }

  struct stat file;
  if(stat(iesire,&file)==-1)
      if(!S_ISDIR(file.st_mode))
	{
	  perror("error stat output director");
	  exit(1);
	}
  
    
  char statistici[300];
  sprintf(statistici,"%s/statistica.txt",director);
  
  int st_fd=open(statistici,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  
  while((entry=readdir(dir))!=NULL)
    {
      if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
	{
	  
	  char path[300];
	  sprintf(path,"%s/%s",director,entry->d_name);

	  pid_t childPid=fork();
	  if(childPid ==-1)
	    {
	      perror("error child process creation");
	      exit(1);
	    }
	  if(childPid ==0)
	    {
	      int count=0;
	      char outpath[300];
	      sprintf(outpath,"%s/%s_statistica.txt",iesire,entry->d_name);

	      int out_fd=open(outpath,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	      if(out_fd==-1)
		{
		  perror("error open output file");
		  exit(1);
		}
	  
	      //apelam cerintele pt fiecare fisier/dir/legatura simbolica
	      cerinte(path,entry->d_name,out_fd, &count);
	    

	      if(close(out_fd)==-1)
		{
		  perror("error close output file");
		  exit(1);
		}
	      exit(count);
	    }
	  else
	    {
	      int status=0;
	      waitpid(childPid, &status,0);
	      if(WIFEXITED(status))
		printf("S-a incheiat procesul cu pid-ul %d si codul %d\n",childPid,WEXITSTATUS(status));
	      else
		printf("process with pid %d didnt't ended correctly\n",childPid);
	  
	      writeStatistic(st_fd,childPid,WEXITSTATUS(status));
	    }
	    
	}
    }
	
      if(close(st_fd)==-1)
	{
	  perror("error close statistic");
	  exit(1);
	}
      if(closedir(dir)==-1)
	{
	  perror("error close dir");
	  exit(1);
	}
	
    
}

int main(int argc,char*argv[])
{
  if(argc!=3)
    {
      perror("incorect number of args");
      exit(1);
    }
  citire_director(argv[1],argv[2]);
  return 0;
}
//fol structura stat pt fiecare fis, verif cu st_mode daca e regulat,legat simbolica sau director si afisam atr pt fiecare fis, trebuie ignorate cele doua .. si . cu un strcmp prob
//stat trb sa primeasca intreaga cale carte fisier cu sprintf director/numefisier
