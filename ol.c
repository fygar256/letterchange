/*
	英単語の一部をn文字置換したリストを与える
	Usage: ol [-d=dicfile] <a word> [n]
	usage ex.    0: ol rain	# nを省略すると1文字置換
	usage ex.    1: ol rain 2	# 2文字置換指定

	to compile: ol.c -o ol
*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>

static	char defaultdic[]="EIJIRO-1446.TXT";
FILE	*so;
FILE	*fp;

void wayout() {
	fclose(so);
	fclose(fp);
	system("sort ol.tmp |uniq && rm ol.tmp");
	exit(0);
}

int ffgetc(FILE *fp) {
	int c;
	c=fgetc(fp);
	if (c==EOF) wayout();
	return(c);
}

char *strtolower(char *s) {
	char	*t=s;
	while(*s!='\0')
		*s++=tolower(*s);
	return(t);
}
char *getword(FILE *fp) {
static	char word[1024];
	int idx=0,c;

	c=ffgetc(fp);
	if (c==0x81) ffgetc(fp);
		else ungetc(c,fp);

	while(1) {
		c=ffgetc(fp);
		if (!isalpha(c)) break;
		word[idx++]=c;
		}
		word[idx]='\0';
		ungetc(c,fp);
		while(ffgetc(fp)!='\n');
		return(strtolower(word));
}

int	main(int argc,char *argv[])
{
	char *dicword,word[1024];
	char *dicfile=defaultdic;
	int	l,n;

	if (argc>=2&&strncmp(*(argv+1),"-d=",3)==0)
			dicfile=&((*(++argv))[3]);

	n=1;
	switch(argc) {
	case 2:
		sscanf(*(++argv),"%s",word);
		break;
	case 3:
		sscanf(*(++argv),"%s",word);
		sscanf(*(++argv),"%d",&n);
		break;
	default:
		exit(1);
		}
		
	strtolower(word);
	l=strlen(word);

	fp=fopen(dicfile,"r");
	so=fopen("ol.tmp","w");
	if (fp!=NULL) {
		while((dicword=getword(fp))!=NULL) {
			if (strlen(dicword)==l) {
				int	i,d;
				for(i=0,d=0;i<l;i++)
					if (dicword[i]!=word[i]) d++;
				if (d==n) fprintf(so,"%s\n",dicword);
				}
			}
		}
	wayout();
}
