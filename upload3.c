#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#define CHECKEXT 1

#define MAXSIZE 2000000

#define UPLOADIR "/tmp/upload"

#define MAXLINE 512

char szBoundary;
char szFile;

char myFile;

#define BUFFSIZE 16*1024
char szBuff;


int getext(const char *filename)
{
	char *p, *q;
	int len,i,isgood = 0;
	char *goodext = {"gif","jpg","jpeg","png","bmp","tgz","gz","txt","tar.gz"};
	char ext; 

	if (!(p = strchr(filename, '.')))
	return 0;

	++p;

if (!(q = strchr(p, '\0')))
return 0;

len = q - p;
memcpy(ext, p, len);
ext = '\0';

for (i=0;i<=9;i++)
{
if(strcmp(ext,goodext) == 0)
{
isgood = 1;
}
}

return isgood;
}


int main(int argc, char **argv[])
{
int rc = 0;
char *up= UPLOADIR;
char *psz1; char *psz2;
FILE *out = NULL;
long i, total, count;
char *ContentLength;

long InCount;

char *filename;

printf("Content-type: text/html\n\n");
printf("<html>");
printf("<head><title>File Upload</title></head>\n");

printf("<center><h1>File Upload Status</h1></center>\n");

ContentLength = getenv("CONTENT_LENGTH");
InCount = atol(ContentLength);

if (InCount > MAXSIZE)
{
rc=7;
goto Error;
}

memset(szFile, 0, sizeof(szFile));

szBoundary = '\r';
szBoundary = '\n';
szBoundary = '\0';

if (fgets(&szBoundary, sizeof(szBoundary)-2, stdin) == NULL)
{
rc = 1;
goto Error;
}

//strip terminating CR / LF
if ((psz1=strchr(&szBoundary,'\r')) != NULL)
{
*psz1 = '\0';
}

if ((psz1=strchr(&szBoundary,'\n')) != NULL)
{
*psz1 = '\0';
}

if (fgets(szBuff, sizeof(szBuff), stdin) == NULL)
{
rc = 2;
goto Error;
}

if ((psz1=strstr(szBuff, "filename=")) == NULL)
{
rc = 3;
goto Error;
}

psz1+=strlen("filename=");

if ((psz1 = strtok(psz1, " \"")) == NULL)
{
rc = 4;
goto Error;
}

if ((psz2 = strrchr(psz1,'\\')) != NULL)
{
psz1 = psz2+1;
}
if ((psz2 = strrchr(psz1,'/')) != NULL)
{
psz1 = psz2+1;
}

sprintf(szFile, "%s/%s",up,psz1);

sprintf(myFile, "%s", psz1);

filename= myFile;

if (CHECKEXT)
{
if ((getext(filename))== 0)
{
rc = 8;
goto Error;
}
}

if ((out = fopen(szFile, "rb")) != NULL)
{
rc = 5;
goto Error;
}


if ((out = fopen(szFile, "wb+")) == NULL)
{
rc = 6;
goto Error;
}

while (fgets(szBuff, sizeof(szBuff), stdin) != NULL)
{
if (strlen(szBuff) <= 2)
{
break;
}
}

while ((count=fread(szBuff, 1, sizeof(szBuff), stdin)) != 0)
{
if ((i=fwrite(szBuff, 1, count, out)) != count)
{
rc = 7;
goto Error;
}
}

if ((count = ftell(out)) == -1)
{
rc = 8;
goto Error;
}

if (count > 128)
{
count = 128;
}

if (fseek(out, 0-count, SEEK_END) != 0)
{
rc = 9;
goto Error;
}

if ((total = ftell(out)) == -1)
{
rc = 10;
goto Error;
}

count = fread(szBuff, 1, sizeof(szBuff), out);
szBuff = '\0';

rc = 11;
for (i=0; i<=(count-(long)strlen(szBoundary)); i++)
{
if ((szBuff == szBoundary)
&& (strncmp(szBoundary, &szBuff, strlen(szBoundary)) == 0))
{
total+=i;
rc = 0;
break;
}
}

if (rc != 0)
{
goto Error;
}

if (total == 0)
{
rc = 11;
goto Error;
}

fflush(out);

Error: if (out != NULL)
{
fclose(out);
}

switch (rc)
{
case 0: // success
printf("The file <b> %s %d bytes</b> was uploaded sucessfully.",myFile,InCount);
break;

case 5: // file exists
printf ("The file %s already exists and cannot be overwritten.Please try again with a different file.\n", myFile);
break;

case 7: // file too big
printf ("The file <b> %s </b> is too big. Try again.",myFile);
break;

case 8: // file is not an allowed type
printf ("The file <b> %s </b> is not an allowed type. Try again.",myFile);
break;

case 11: // 0 byte file
printf("The file <b>%s </b>contains no data.<br>Please try again with a different file.", myFile);
unlink(szFile);
break;

default: // all other cases
printf("Error %d uploading file<b>%s </b>Please try again.", rc, myFile);
unlink(szFile);
break;
}

if ((psz1=getenv("HTTP_REFERER")) != NULL)
{
printf("<p><A HREF =\"%s\">Back</A>", psz1);
}

printf("</html>\n");

return 0;
}
