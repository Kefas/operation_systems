#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>


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
    printf("socket");
    break;
  case S_IFLNK:
    printf("symbolic link");
    break;
  case S_IFREG:
    printf("regular file");
    break;
  case S_IFIFO:
    printf("FIFO");
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
  printf("Mode:                     %lo (octal)\n", (unsigned long) sb->st_mode);
  if((sb->st_mode & S_IRUSR) == S_IRUSR) usr += 4;
  if((sb->st_mode & S_IWUSR) == S_IWUSR) usr += 2;
  if((sb->st_mode & S_IXUSR) == S_IXUSR) usr += 1;

  if((sb->st_mode & S_IRGRP) == S_IRGRP) grp += 4;
  if((sb->st_mode & S_IWGRP) == S_IWGRP) grp += 2;
  if((sb->st_mode & S_IXGRP) == S_IXGRP) grp += 1;

  if((sb->st_mode & S_IROTH) == S_IROTH) oth += 4;
  if((sb->st_mode & S_IWOTH) == S_IWOTH) oth += 2;
  if((sb->st_mode & S_IXOTH) == S_IXOTH) oth += 1;
 
  printf("Mode:                     %d%d%d\n",usr,grp,oth);

					
}
/* -------------------------------------------------------------------- */
     
static void print_linkc(const struct stat *sb){
  printf("Link count:               %ld\n", (long) sb->st_nlink);
}
/* -------------------------------------------------------------------- */
     
static void print_owner(const struct stat *sb){
  struct passwd *pwd = getpwuid(uid);
  struct group *grp = getgrgid(gid);
  char uname [100] = pwd->pw_name
  printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb->st_uid, (long) sb->st_gid);
  printf("Ownership:                %c(%ld) %c(%ld)\n", sb->
}
/* -------------------------------------------------------------------- */
     
static void print_size(const struct stat *sb){
  printf("Preferred I/O block size: %ld bytes\n", (long) sb->st_blksize);
  printf("File size:                %lld bytes\n",(long long) sb->st_size);
  printf("Blocks allocated:         %lld\n",(long long) sb->st_blocks);
}
/* -------------------------------------------------------------------------------- */
     
static void print_laststch(const struct stat *sb){
  printf("Last status change:       %s", ctime(&sb->st_ctime));
}
/* -------------------------------------------------------------------------------- */
     
static void print_lastacc(const struct stat *sb){
  printf("Last file access:         %s", ctime(&sb->st_atime));
}
/* -------------------------------------------------------------------------------- */
     
static void print_lastmod(const struct stat *sb){
  printf("Last file modification:   %s", ctime(&sb->st_mtime));
}
/* -------------------------------------------------------------------------------- */
     
static void print_name(const struct stat *sb, char *name){
  char* bname = basename(name);
  printf("Name of the file:         %s\n", bname);
}
/* -----------------------------------------------------------------------*/
