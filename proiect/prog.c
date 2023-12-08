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

void cerinteDir(struct stat* st_file,char *path,char* name,int out_fd,int *lines_count)
{
  char str[3000];
  char buff[500];//buffer pt transformarea val din intregi in sir de caractere
  //adaugare nume
  strcpy(str,"nume director: ");
  *lines_count=5;
  strcat(str,name);
  strcat(str,"\n");

  //ID utilizator
  strcat(str,"identificatorul utilizatorului: ");
  sprintf(buff,"%d\n",st_file->st_uid);
  strcat(str,buff);

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

  //scrierea in fisierul de iesire
  if(write(out_fd,str,strlen(str))==-1)
    {
      perror("error write file");
      exit(1);
    }
}
void cerinteSlink(struct stat* st_file,struct stat* st_link,char *path,char* name,int out_fd,int *lines_count)
{
  char str[3000];
  char buff[500];//buffer pt transformarea val din intregi in sir de caractere
  
  //adaugare nume
  strcpy(str,"nume legatura: ");
  *lines_count=6;
  strcat(str,name);
  strcat(str,"\n");
  
  //dimensiunea pentru legatura simbolica
  sprintf(buff,"dimensiune legatura: %ld\n",st_link->st_size);
  strcat(str,buff);

  //dimensiune fisier 
  strcat(str,"dimensiune: ");
  sprintf(buff,"%ld\n",st_file->st_size);
  strcat(str,buff);
    
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

  //scrierea in fisierul de iesire
  if(write(out_fd,str,strlen(str))==-1)
    {
      perror("error write file");
      exit(1);
    }
}
void cerinteBmp(struct stat* st_file,char *path,char* name,int out_fd,int *lines_count)
{
  char str[3000];
  char buff[500];
  
  //adaugare nume
  strcpy(str,"nume fisier: ");
  *lines_count=10;
  strcat(str,name);
  strcat(str,"\n");
  
  //adaugare lungime, inaltime
  int fd;
  fd=open(path,O_RDWR);
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
      perror("error closing file");
      exit(1);
    }

  //dimensiune fisier 
  strcat(str,"dimensiune: ");
  sprintf(buff,"%ld\n",st_file->st_size);
  strcat(str,buff);

  //ID utilizator
  strcat(str,"identificatorul utilizatorului: ");
  sprintf(buff,"%d\n",st_file->st_uid);
  strcat(str,buff);

  //timpul ultimei modificari
  strcat(str,"timpul ultimei modificari: ");
  strcat(str,ctime(&st_file->st_mtime));
      
  
  //contorul de legaturi
  strcat(str,"contorul de legaturi: ");
  sprintf(buff,"%ld\n",st_file->st_nlink);
  strcat(str,buff);

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
  
  //scrierea in fisierul de iesire
  if(write(out_fd,str,strlen(str))==-1)
    {
      perror("error write file");
      exit(1);
    }
}
void cerinteReg(struct stat* st_file,char *path,char* name,int out_fd,int *lines_count,int *pfd)
{
  char str[3000];
  char buff[500];
  //adaugare nume
  strcpy(str,"nume fisier: ");
  *lines_count=8;
  strcat(str,name);
  strcat(str,"\n");

  //dimensiune fisier 
  strcat(str,"dimensiune: ");
  sprintf(buff,"%ld\n",st_file->st_size);
  strcat(str,buff);

  //ID utilizator 
  strcat(str,"identificatorul utilizatorului: ");
  sprintf(buff,"%d\n",st_file->st_uid);
  strcat(str,buff);

  //timpul ultimei modificari
  strcat(str,"timpul ultimei modificari: ");
  strcat(str,ctime(&st_file->st_mtime));
      
  
  //contorul de legaturi
  strcat(str,"contorul de legaturi: ");
  sprintf(buff,"%ld\n",st_file->st_nlink);
  strcat(str,buff);

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

  //scrierea in fisierul de iesire
  if(write(out_fd,str,strlen(str))==-1)
    {
      perror("error write file");
      exit(1);
    }

  //citirea din fisier a liniilor si scrierea informatiei in pipe  
  close(pfd[0]);
  ssize_t n;
  char buffer[1024];
  int fd;
  fd=open(path,O_RDONLY);
  if(fd==-1)
    {
      perror("error.open");
      exit(1);
    }
  while((n=read(fd,buffer,sizeof(buffer)))>0)
    {
      write(pfd[1],buffer,n);
		
    }
  close(pfd[1]);
        
      
}


void citire_director(const char *director,const char *iesire,char * caracter)
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


  int suma=0;
  
  while((entry=readdir(dir))!=NULL)
    {
      if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
	{
	  
	  char path[300];
	  sprintf(path,"%s/%s",director,entry->d_name);

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

	  //daca e legatura simbolica
	  if(S_ISLNK(st_link.st_mode))
	    {
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
	  
		  //apelam cerintele
		  cerinteSlink(&st_file,&st_link,path,entry->d_name,out_fd, &count);
	    

		  if(close(out_fd)==-1)
		    {
		      perror("error close output file");
		      exit(1);
		    }
		  exit(count);
		}
	    }
	  else
	    {
	      //daca e director
	      if(S_ISDIR(st_file.st_mode))
		{
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
	  
		      //apelam cerintele
		      cerinteDir(&st_file,path,entry->d_name,out_fd, &count);
	    

		      if(close(out_fd)==-1)
			{
			  perror("error close output file");
			  exit(1);
			}
		      exit(count);
		    }
		}
	      else
		//daca e fisier bmp
		if(S_ISREG(st_file.st_mode) && strstr(path,".bmp"))
		  {
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
	  
			//apelam cerintele
			cerinteBmp(&st_file,path,entry->d_name,out_fd, &count);
	    

			if(close(out_fd)==-1)
			  {
			    perror("error close output file");
			    exit(1);
			  }
			exit(count);
		      }
		    
		    //transformarea in alb negru a imaginii
		    
		    int fd;
		    fd=open(path,O_RDWR);
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

		   
		    //in procesul parinte o sa facem alt proces copil pt transformarea bmp-ului
		    pid_t childPid1 = fork();

		    if (childPid1 == -1) {
		      perror("error child process creation ");
		      close(fd);
		      return;
		    }
		    if (childPid1 == 0)
		      {
			int pixels=lungime*inaltime;
			unsigned char bmpHeader[54];
			if (read(fd,bmpHeader,sizeof(bmpHeader))!= sizeof(bmpHeader))
			  {
			    perror("error reading bmp header");
			    exit(1);
			  }
			for(int i=0;i<pixels;i++)
			  {
			    unsigned char p[3];
			    if(read(fd,p,sizeof(p))!=sizeof(p))
			      {
				perror("error reading pixels");
				exit(1);
			      }
			    unsigned char gray;
			    gray=0.299*p[0]+0.587*p[1]+0.114*p[2];
	      
			    lseek(fd,-3,SEEK_CUR);
	      
			    write(fd,&gray,sizeof(gray));
			    write(fd,&gray,sizeof(gray));
			    write(fd,&gray,sizeof(gray));
			  }
	  
			if(close(fd)==-1)
			  {
			    perror("error closing file");
			    exit(1);
			  }
			exit(0);
		      }
		    //
		    
		  }
		else
		  //daca e fisier regulat, nebmp
		  if(S_ISREG(st_file.st_mode) && !(strstr(path,".bmp")))
		    {
		      
		      int pfd[2];
  
		      if(pipe(pfd)<0)
			{
			  perror("Eroare la crearea pipe-ului\n");
			  exit(1);
			}
		      
		      
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
	  
			  //apelam cerintele
			  cerinteReg(&st_file,path,entry->d_name,out_fd, &count,pfd);
	    

			  if(close(out_fd)==-1)
			    {
			      perror("error close output file");
			      exit(1);
			    }
			  exit(count);
			}

		      //cerinta sapt 9
		      //in cazul procesului parinte, daca fisierul e normal,nu bmp, aplicam bash-ul pe datele citite din pipe si calculam suma liniilor corecte
		      
		      int pfd2[2];
		      if(pipe(pfd2)<0)
			{
			  printf("Eroare la crearea pipe-ului\n");
			  exit(1);
			}
		      pid_t childPid1=fork();
		      if(childPid1 ==-1)
			{
			  perror("error child process creation");
			  exit(1);
			}

		      if(childPid1 ==0)
			{
			  close(pfd[1]);
			  close(pfd2[0]);
			  
			  dup2(pfd[0],0);
			  
			  close(pfd[0]);
			  dup2(pfd2[1],1);
			  close(pfd2[1]);
			  execlp("bash","bash","bashfile.sh",caracter,NULL);

			  perror("execlp error\n");
			  exit(1);
			}
		      close(pfd2[1]);
		      close(pfd[1]);
		      close(pfd[0]);
		      int nr=0;
		      FILE *stream;
		      stream=fdopen(pfd2[0],"r");
		      // deschide un stream (FILE *) pentru capatul de citire 
		      while((fscanf(stream,"%d",&nr))!=EOF)
			suma=suma+nr;
		      close(pfd2[0]);
		    }
		      
		    
	      
	    }
	    
	}
    }

  
  //rularea in paralel a proceselor
  int status,pid;
  while((pid=wait(&status))!=-1)
    {
      if (WIFEXITED(status))
	{
	  printf("S-a incheiat procesul cu pid-ul %d si codul %d\n",pid, WEXITSTATUS(status));
	  
	}
      else 
        printf("Process with pid %d didn't end correctly\n",pid);
      
    }
  
  printf("Au fost identificate in total %d propozitii corecte care contin caracterul %s\n",suma,caracter);
 
  if(closedir(dir)==-1)
    {
      perror("error close dir");
      exit(1);
    }
	
    
}

int main(int argc,char*argv[])
{
  if(argc!=4)
    {
      perror("incorect number of args");
      exit(1);
    }
  citire_director(argv[1],argv[2],argv[3]);

  
  return 0;
}
