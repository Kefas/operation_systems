#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <pwd.h>
#include <grp.h>


/* -------------------------------------------------------------------- */
     
static void print_type(struct stat *sb);
static void print_ino(const struct stat *sb);
static void print_perms(const struct stat *sb);
static void print_linkc(const struct stat *sb);
static void print_owner(const struct stat *sb);
static void print_size(const struct stat *sb);
static void print_laststch(const struct stat *sb);
static void print_lastacc(const struct stat *sb);
static void print_lastmod(const struct stat *sb);
static void print_name(const struct stat *sb, char *name);
/* -------------------------------------------------------------------- */
     
int  main(int argc, char *argv[])
{
  struct stat sb;
     
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
     
  if (stat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_SUCCESS);
  }
     
  print_type(&sb);
  print_name(&sb, argv[1]);
  print_ino(&sb);
  print_perms(&sb);
  print_linkc(&sb);
  print_owner(&sb);
  print_size(&sb);
  print_laststch(&sb);
  print_lastacc(&sb);
  print_lastmod(&sb);
     
  exit(EXIT_SUCCESS);
}
/* -------------------------------------------------------------------- */
     
static void print_type(struct stat *sb){
  printf("File type:                ");
  switch (sb->st_mode & S_IFMT) {
  case S_IFBLK: 
    printf("block device\n"); 
    break;
  case S_IFCHR:
    printf("character device\n");
    break;
  case S_IFSOCK:
    printf("socket\n");
    break;
  case S_IFLNK:
    printf("symbolic link\n");
    break;
  case S_IFREG:
    printf("regular file\n");
    break;
  case S_IFIFO:
    printf("FIFO\n");
    break;
  default:
    printf("unknown?\n");
    break;
  }
}
/* -------------------------------------------------------------------- */
     
static void print_ino(const struct stat *sb){
  printf("I-node number:            %ld\n", (long) sb->st_ino);
}
/* -------------------------------------------------------------------- */
     
static void print_perms(const struct stat *sb){
  int usr=0,grp=0,oth=0;
  char *perms = (char *) malloc(sizeof(char) * 128);
  strcat(perms, "Your permissions:         ");
  printf("Mode:                     %lo (octal)\n", (unsigned long) sb->st_mode);
  strcat(perms, "read: ");
  if((sb->st_mode & S_IRUSR) == S_IRUSR){
    usr += 4;
    strcat(perms,"yes, ");
  }
  else
    strcat(perms,"no, ");
  strcat(perms, "write: ");
  if((sb->st_mode & S_IWUSR) == S_IWUSR){
    usr += 2;
    strcat(perms,"yes, ");
  }
  else
    strcat(perms,"no, ");
  strcat(perms, "execute: ");
  if((sb->st_mode & S_IXUSR) == S_IXUSR){
    usr += 1;
    strcat(perms,"yes ");
  }
  else
    strcat(perms,"no \n");
  if((sb->st_mode & S_IRGRP) == S_IRGRP) grp += 4;
  if((sb->st_mode & S_IWGRP) == S_IWGRP) grp += 2;
  if((sb->st_mode & S_IXGRP) == S_IXGRP) grp += 1;

  if((sb->st_mode & S_IROTH) == S_IROTH) oth += 4;
  if((sb->st_mode & S_IWOTH) == S_IWOTH) oth += 2;
  if((sb->st_mode & S_IXOTH) == S_IXOTH) oth += 1;
  
 
  printf("Mode:                     %d%d%d\n",usr,grp,oth);
  printf("%s",perms);

					
}
/* -------------------------------------------------------------------- */
     
static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------- */
     
static void print_owner(const struct stat *sb){
  long uid,gid;
  uid = (long)sb->st_uid;
  gid = (long)sb->st_gid;
  struct passwd *pwd = getpwuid(uid);
  struct group *grp = getgrgid(gid);
  char *uname =(char *)malloc(64*sizeof(char));
  char *gname =(char *)malloc(64*sizeof(char));  
  uname = pwd->pw_name;
  gname = grp->gr_name;
  

  printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb->st_uid, (long) sb->st_gid);
  printf("Ownership:                %s(%ld) %s(%ld)\n", uname, uid, gname, gid );


}
/* -------------------------------------------------------------------- */
     
static void print_size(const struct stat *sb){
  printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
  printf("Preferred I/O block size (kb): %f kb\n", (float)sb->st_blksize/1024); 
  printf("Preferred I/O block size (MB): %f Mb\n",(float)sb->st_blksize/(1024*1024));
  printf("File size:                %ld bytes\n",(long)sb->st_size);
 
  printf("File size (kb):           %f kb\n",(float)sb->st_size/1024);
  
  printf("File size (MB):           %f Mb\n",(float)sb->st_size/(1024*1024));

  printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */
     
static void print_laststch(const struct stat *sb){
  time_t now;
  time(&now);
  long long seconds;
  seconds = difftime(now,sb->st_ctime);
  printf("Last status change:       %s", ctime(&sb->st_ctime));
  printf("Last status change:       %d days ago\n",seconds/60/60/24);
}
/* -------------------------------------------------------------------------------- */
     
static void print_lastacc(const struct stat *sb){
time_t now;
  time(&now);
  long long seconds;
  seconds = difftime(now,sb->st_atime);
  printf("Last file access:         %s", ctime(&sb->st_atime));
  printf("Last file access:         %d days ago\n",seconds/60/60/24 );
}
/* -------------------------------------------------------------------------------- */
     
static void print_lastmod(const struct stat *sb){
time_t now;
  time(&now);
  long long seconds;
  seconds = difftime(now,sb->st_atime);
  printf("Last file modification:   %s", ctime(&sb->st_mtime));
  printf("Last file modification:   %d days ago\n",seconds/60/60/24 );
}
/* -------------------------------------------------------------------------------- */
     
static void print_name(const struct stat *sb, char *name){
  char* bname = basename(name);
  char* lname = (char*) malloc(128 * sizeof(char));
  ssize_t n;
  if( S_ISLNK(sb->st_mode)){
    if((n = readlink(name,lname, sizeof(lname)) != -1)){
	if(lname != NULL){
	  lname[n] = '\0';
	  printf("%s links to %s\n",bname,lname);
	}
	else{
	  perror("NULL lname");
	  exit(EXIT_FAILURE);
	}
	  
      }
      else{
	printf("Can not read link from file %s\n",bname);
      }
  }
  else
  printf("Name of the file:         %s\n", bname);
}
/* -----------------------------------------------------------------------*/
