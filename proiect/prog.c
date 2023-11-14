#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>

void cerinte(char *path,char* name, struct stat *st_file)
{
  char str[3000];
  char buff[100];//buffer pt transformarea val din intregi in sir de caractere
  struct stat st_link;
  if(lstat(path,&st_link) == -1)
    {
      perror("stat error");
      exit(1);
    }
  //adaugare nume
  if(S_ISLNK(st_link.st_mode))
    strcpy(str,"nume legatura: ");
  else
    {
      if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode) && strstr(path,".bmp")))
	strcpy(str,"nume fisier: ");
  
      if(S_ISDIR(st_file->st_mode))
	strcpy(str,"nume director: ");
    }
  strcat(str,name);
  strcat(str,"\n");

  //adaugare lungime si inaltime pentru fisierele bmp
  if(S_ISREG(st_file->st_mode)&& strstr(path,".bmp"))
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
      if(close(fd)==-1)
	{
	  perror("error close poza");
	  exit(1);
	}
    }
  
  //dimensiunea pentru legatura simbolica
  if(S_ISLNK(st_link.st_mode))
    {
      sprintf(buff,"dimensiune legatura: %ld\n",st_link.st_size);
      strcat(str,buff);
    }
  
  //dimensiune fisier pt fisier regulat,bmp sau legatura simbolica 
  if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode)&& strstr(path,".bmp"))|| S_ISLNK(st_file->st_mode) )
    {
      strcat(str,"dimensiune: ");
      sprintf(buff,"%ld\n",st_file->st_size);
      strcat(str,buff);
    }
  
  //ID utilizator pt fisier regulat,bmp sau director
  if(!(S_ISLNK(st_link.st_mode)))
    if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode)&& strstr(path,".bmp")) || S_ISDIR(st_file->st_mode))
      {
	strcat(str,"identificatorul utilizatorului: ");
	sprintf(buff,"%d\n",st_file->st_uid);
	strcat(str,buff);
      }
  
  //timpul ultimei modificari pt fisier bmp sau regulat
  if(!(S_ISLNK(st_link.st_mode)))
    if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode)&& strstr(path,".bmp")))
      {
	strcat(str,"timpul ultimei modificari: ");
	strcat(str,ctime(&st_file->st_mtime));
      }
  
  //contorul de legaturi pt fisier bmp sau regulat
  if(!(S_ISLNK(st_link.st_mode)))//ca sa nu intre si pt legatura simbolica
    if(S_ISREG(st_file->st_mode) || (S_ISREG(st_file->st_mode)&& strstr(path,".bmp")))
      {
	strcat(str,"contorul de legaturi: ");
	sprintf(buff,"%ld\n",st_file->st_nlink);
	strcat(str,buff);
      }
  
  //drepturi de acces user
  //drepturi de acces grup
  //drepturi de acces altii
  strcat(str,"drepturi de acces user: ");
  sprintf(buff,"%s%s%s\n", st_file->st_mode & S_IRUSR ? "R" : "-",st_file->st_mode & S_IWUSR ? "W" : "-",st_file->st_mode & S_IXUSR ? "X" : "-");
  strcat(str,buff);
  strcat(str,"drepturi de acces grup: ");
  sprintf(buff,"%s%s%s\n", st_file->st_mode & S_IRGRP ? "R" : "-",st_file->st_mode & S_IWGRP ? "W" : "-",st_file->st_mode & S_IXGRP ? "X" : "-");
  strcat(str,buff);
  strcat(str,"drepturi de acces altii: ");
  sprintf(buff,"%s%s%s\n",st_file->st_mode & S_IROTH ? "R" : "-",st_file->st_mode & S_IWOTH ? "W" : "-",st_file->st_mode & S_IXOTH ? "X" : "-");
  strcat(str,buff);
  strcat(str,"\n");

  
  char *output="statistica.txt";
  struct stat st_output;
  if(stat(output,&st_output)==-1){
    perror("stat error");
    exit(1);
  }
  int file_fd1;
  file_fd1=open(output,O_WRONLY | O_APPEND);
  if(file_fd1==-1)
    {
      perror("error open file");
      exit(1);
    }
  if(write(file_fd1,str,strlen(str))==-1)
    {
      perror("error write file");
      exit(1);
    }
  if(close(file_fd1)==-1)
    {
      perror("error close output file");
      exit(1);
    }
}
void citire_director(char *director)
{
  DIR *dir;
  dir=opendir(director);
  if(dir==NULL)
    {
      perror("error open director");
      exit(1);
    }

  struct dirent *entry=NULL;
  
  while((entry=readdir(dir))!=NULL)
    {
      if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
	{
	  struct stat file;
	  char path[300]={};
	  sprintf(path,"%s/%s",director,entry->d_name);
	  
	  if(stat(path,&file)==-1)
	    {
	      perror("error stat");
	      exit(1);
	    }
	  if(S_ISDIR(file.st_mode))
	    {
	      cerinte(path, entry->d_name, &file);
	      citire_director(path);
	    }
	  else
	      cerinte(path,entry->d_name,&file);
	}
    }

  if(closedir(dir)==-1){
    perror("error close dir");
    exit(1);
  }
}

int main(int argc,char*argv[])
{
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
