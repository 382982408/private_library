#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <zlib.h>
#include <unistd.h>

#define H_num 10
#define LEN_1 500;
#define LEN_2 200;
#define LEN_3 100;

int main(int argc, char *argv[])
{
    //////////////table handel
    int get_check(char d[]);
    int get_index(char d[]);
    int get_sort(char i[],char o[]);
    int get_filter(char i[],char o[],char d[]);
    int get_db_combine(char i[],char o[],char d[],char n[]);
    int get_crush(char d[],char o[],char n[]);
    //////////////
    int get_anno(char i[],char o[],char d[]);
    int get_intersect(char i[],char o[],char d[],char n[]);
    int help(char prog[],char cmd[],char i[],char o[],char d[],char n[]);
    //////////////vcf handel 
	int get_sortvcf(char i[],char o[]);
	int get_indexvcf(char d[]);
	int get_checkvcf(char d[]);
	int get_annovcf(char i[],char o[],char d[],char n[]);
	
    char opt_cmd[100];opt_cmd[0]='\0';
    char opt_i[500];opt_i[0]='\0';
    char opt_o[500];opt_o[0]='\0';
    char opt_d[500];opt_d[0]='\0';
    char opt_n[500];opt_n[0]='\0';
    ////////////////////////
    if (argv[1]!=NULL)
    {
        sscanf(argv[1],"%s",&opt_cmd);
    };
    int opt;
    while ((opt=getopt(argc,argv,"i:d:o:n:"))!=-1)
    {
        switch(opt)
        {
            case 'i':
                sscanf(optarg,"%s",&opt_i);
                break;
            case 'd':
                sscanf(optarg,"%s",&opt_d);
                break;
            case 'o':
                sscanf(optarg,"%s",&opt_o);
                break;
            case 'n':
                sscanf(optarg,"%s",&opt_n);
                break;
            default:
                break;
        };
    };
    help(argv[0],opt_cmd,opt_i,opt_o,opt_d,opt_n);
//    printf("\nwork done ...\n\n");
    return 0;
};
////////////////////////////////
char* strSplit(char* ori, char* des, char* separator, int index) //ori can not change
{
    char *token = NULL;
    char *pStrTemp = (char*)malloc(strlen(ori) + 1);
    memcpy(pStrTemp, ori, strlen(ori) + 1);
    char *p = pStrTemp;
    while((token = strsep(&p, separator)) != NULL)
    {
        if(--index <= 0)
            break;
    }
 
    if(index <= 0 && token != NULL)
        strcpy(des, token);
    else
        strcpy(des, "");
 
    free(pStrTemp);
    
    return des;    
};
////////////////////////
char * strreplace_1(char const * const original, char const * const pattern, char const * const replacement) 
{
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);
  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;
  // find how many times the pattern occurs in the original string
  for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
  {
    patcnt++;
  }
 
  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );
    if (returned != NULL)
    {
      // copy the original string, 
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement 
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }	
}
///////////////////
char *strreplace_2(const char *src, const char *from, const char *to)
{
   size_t size    = strlen(src) + 1;
   size_t fromlen = strlen(from);
   size_t tolen   = strlen(to);
   char *value = malloc(size);
   char *dst = value;
   if ( value != NULL )
   {
      for ( ;; )
      {
         const char *match = strstr(src, from);
         if ( match != NULL )
         {
            size_t count = match - src;
            char *temp;
            size += tolen - fromlen;
            temp = realloc(value, size);
            if ( temp == NULL )
            {
               free(value);
               return NULL;
            }
            dst = temp + (dst - value);
            value = temp;
            memmove(dst, src, count);
            src += count;
            dst += count;
            memmove(dst, to, tolen);
            src += fromlen;
            dst += tolen;
         }else /* No match found. */
         {
            strcpy(dst, src);
            break;
         }
      }
   }
   return value;
}
///////////////////
int strSearch(char *str1,char *str2)
{
	int at,flag=1;
	if (strlen(str2) > strlen(str1))
	{
		at = -1;
	}else if (!strcmp(str1,str2))
	{
		at = 0;
	}else
	{
		int i=0,j=0;
		for (i=0;i < strlen(str1)&&flag;)
		{
			for (j=0;j < strlen(str2) && flag;)
			{
				if (str1[i]!=str2[j])
				{
					i++;
					j=0;
				}else if (str1[i]==str2[j])
				{
					i++;
					j++;
				};
				if (j==strlen(str2))
				{
					flag = 0;
				};
			}
		}
		at = i-j;
	};
	return at;
}
//////////////
/*字符串替换，返回替换后的字符串*/
char *strreplace_3(char str1[],char str2[],char str3[])
{
	char *str;
	int len1,len2,len3,pos;
	len1=strlen(str1);
	len2=strlen(str2);
	len3=strlen(str3);
	pos = strSearch(str1,str2);
	if (pos>=0)
	{
		int i;
		for (i=0;i<pos;i++)
		{
			str[i]=str1[i];
		}
		for (i=0;i<len3;i++)
		{
			str[pos+i]=str3[i];
		}
		int j=pos+len3;
		for (i=pos+len2;i<len1;i++)
		{
			str[j]=str1[i];
			j++;
		}
	}else
	{
		strcpy(str,str1);
	};
	return str;
}
/////////////////////////////////
int strreplace_4(char *sSrc, char *sMatchStr, char *sReplaceStr)
{
        int  StringLen;
        char caNewString[50000];
        char *FindPos = strstr(sSrc, sMatchStr);
        if( (!FindPos) || (!sMatchStr) )
		{
			return -1;
		}
		printf("%s\n%s\n%s\n%s\n",sSrc,sMatchStr,sReplaceStr,FindPos);
        while( FindPos )
        {
                StringLen =strlen(sSrc)-strlen(FindPos);
                strncpy(caNewString, sSrc, StringLen);
                strcat(caNewString, sReplaceStr);
                //strcat(caNewString, FindPos + strlen(sMatchStr));
                strcpy(sSrc, caNewString);
                FindPos = strstr(sSrc, sMatchStr);
        }
        return 0;
}
/////////////////////////////////
char *strreplace_5(char ori[], char str_from[], char str_to[]) //ori can not change
{
	char *token;
	char origin[100000];
	char des[100000];
	origin[0]='\0';
	des[0]='\0';
	if (ori[strlen(ori)-1] != ';')
	{
		sprintf(origin,"%s%s",ori,";");
	}else
	{
		sprintf(origin,"%s",ori);
	};

    char *pStrTemp = (char*)malloc(strlen(origin) + 1);
    memcpy(pStrTemp, origin, strlen(origin) + 1);
    char *p = pStrTemp;
	
    while((token = strsep(&p, str_from)) != NULL)
    {
		if (token[strlen(token)-1] != ';')
		{
			strcat(des,token);
			strcat(des,str_to);
		}else
		{
			strcat(des,token);
		};
    };
	//printf("AAA%s\nBBB%s\n",origin,des);
	if (strlen(des)==0)
	{
		strcpy(des, origin);
	};
    free(pStrTemp);
	char *result=des;
	//printf("AAA\t%s\n",des);
    return result;    
};
/////////////////////////////////
int get_intersect(char i[],char o[],char d[],char n[])
{
    printf("start to intersect...\n\n");
    ////////
    char idx_chr[20];
    sscanf("NC_000001.10","%s",&idx_chr);
    long idx_start=0;
    long idx_end=0;
    float idx_seek=0;
    float idx_diff=5000;
    float seek_tmp=0;
    ////////////
    int idx_tag=1;
    float seek_down=0;
    //////////////////////
    FILE *IN,*ON,*IDX,*OUT;
    char file_idx[500];
    sprintf(file_idx,"%s.idx",d);
    IN=fopen(i,"r");
    ON=fopen(d,"r");
    OUT=fopen(o,"w");
    IDX=fopen(file_idx,"r");

    if (IN==NULL || ON==NULL || IDX==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return 0;
    };
    //////////////////
    char report[5000];
    char LINE_IN[50000];
    char LINE_ON[50000];
    char LINE_IDX[500];
    char check='N';
    fgets(LINE_IDX,sizeof(LINE_IDX),IDX);
    LINE_IDX[strlen(LINE_IDX)-1]=' ';

    while(fgets(LINE_IN,sizeof(LINE_IN),IN)!=NULL)
    {
        if (strstr(LINE_IN,"####"))
        {
            fputs(LINE_IN,OUT);
            continue;
        }else if (strstr(LINE_IN,"NC_0000"))
        {
        }else
        {
            fputs(LINE_IN,OUT);
            continue;
        };
        //////////
        char chr[20];
        long start=0;
        long end=0;

        sscanf(LINE_IN,"%s%d%d",&chr,&start,&end);
        if (strstr(chr,"NC_0000"))
        {
        }else
        {
            fputs(LINE_IN,OUT);
            continue;
        };

        if (strlen(LINE_IN)>=4998)
        {
            fputs(LINE_IN,OUT);
            continue;
        };
        ///////////////////
//        printf("AAA\t%s\t%0.f\t%s\n",LINE_IN,seek_down,chr);
        while (idx_tag==1 && (strcmp(chr,idx_chr)>0 || (strcmp(chr,idx_chr)==0 && start>idx_end)))
        {
            if (idx_seek > seek_down)
            {
                seek_down=idx_seek;
            };
            if (fgets(LINE_IDX,sizeof(LINE_IDX),IDX)!=NULL)
            {
                LINE_IDX[strlen(LINE_IDX)-1]=' ';
                sscanf(LINE_IDX,"%s%d%d%f",&idx_chr,&idx_start,&idx_end,&idx_seek);
            }else
            {
                idx_tag=0;
                break;
            };

            fseek(ON,seek_down,SEEK_SET);
        };
        //////////////////////
        float seek_true=ftell(ON);
        seek_true=seek_true-idx_diff;
        if(seek_true<0)
        {
                seek_true=0;
        };
        fseek(ON,seek_true,SEEK_SET);
        //////////////////////
        int report_tag=0;

//        printf("AAA\t%s\t%0.f\t%s\n",LINE_IN,seek_down,chr);

        while(fgets(LINE_ON,sizeof(LINE_ON),ON)!=NULL)
        {
            char chr_2[20];
            long start_2=0;
            long end_2=0;
            //////////////////////
            if (strstr(LINE_ON,"NC_0000") && (LINE_ON[0]=='N'))
            {
            }else
            {
                continue;
            };
            sscanf(LINE_ON,"%s%d%d",&chr_2,&start_2,&end_2);
            if (strstr(chr_2,"NC_0000"))
            {
            }else
            {
                continue;
            };

            if (strlen(LINE_ON)>=4998)
            {
                continue;
            };

            //////////////////////

//            printf("BBB\t%s\t%f\t%f\n",LINE_ON,seek_down,seek_tmp);
                if (strcmp(chr,chr_2) < 0)
                {
                    break;
                }else if(strcmp(chr,chr_2) > 0)
                {
                    continue;
                }else
                {
//                    printf("BBB\t%s\t%f\t%f\n",LINE_ON,seek_down,seek_tmp);
                    if (end < start_2)
                    {
                        if (report_tag==1)
                        {
                            fseek(ON,seek_tmp,SEEK_SET);
                        };
                        break;
                    }else if (start > end_2)
                    {
                        continue;
                    }else
                    {
                        if (report_tag==0)
                        {
                            sprintf(report,"%s\t####%s\t%s",LINE_IN,n,LINE_ON);
                            fputs(report,OUT);
                            report_tag=1;
                            seek_tmp=ftell(ON);
                        }else
                        {
                            sprintf(report,"\t####%s\t%s",n,LINE_ON);
                            fputs(report,OUT);
                        };
                    };
                };
        };
        //////////////
        if (report_tag==0)
        {
            fputs(LINE_IN,OUT);
        };
    };

    printf("intersecting is ended...\n");

    fclose (IN);
    fclose (ON);
    fclose (IDX);
    fclose (OUT);
    return 0;
};
////////////////////////////////
////////////////////////////////
int get_filter(char i[],char o[],char d[])
{
    printf("start to filter...\n\n");
    //////////////////////
    float idx_diff=5000;
    FILE *IN,*ON,*OUT,*ERR;

    IN=fopen(d,"r");
    ON=fopen(i,"r");
    OUT=fopen(o,"w");

    char o_err[500];
    sprintf(o_err,"%s.err",o);
    ERR=fopen(o_err,"w");

    if (IN==NULL || ON==NULL || OUT==NULL || ERR==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return 0;
    };
    //////////////////
    char report[5000];
    char LINE_IN[5000];
    char LINE_ON[5000];
    while(fgets(LINE_IN,sizeof(LINE_IN),IN)!=NULL)
    {
        char chr[20];
        long start=0;
        long end=0;
        if (strstr(LINE_IN,"NC_0000"))
        {
        }else
        {
            sprintf(report,"error:not_begin_with_NC\t%s",LINE_IN);
            fputs(report,ERR);
            continue;
        };
        //////////
        sscanf(LINE_IN,"%s%d%d",&chr,&start,&end);
        if (strstr(chr,"NC_0000"))
        {
        }else
        {
            sprintf(report,"error:not_begin_with_NC\t%s",LINE_IN);
            fputs(report,ERR);
            continue;
        };

        if (strlen(LINE_IN)>=4998)
        {
            sprintf(report,"error:data_too_long\t%s",LINE_IN);
            fputs(report,ERR);
            continue;
        };
        ///////////////////
//        printf("AAA\t%s\t%0.f\t%s\n",LINE_IN,seek_down,chr);
        //////////////////////
        float seek_true=ftell(ON);
        seek_true=seek_true-idx_diff;
        if(seek_true<0)
        {
                seek_true=0;
        };
        fseek(ON,seek_true,SEEK_SET);
        //////////////////////
        int report_tag=0;
        //////////////////////


        while(fgets(LINE_ON,sizeof(LINE_ON),ON)!=NULL)
        {
            char chr_2[20];
            long start_2=0;
            long end_2=0;
            //////////////////////
            if (strstr(LINE_ON,"NC_0000") && (LINE_ON[0]=='N'))
            {
            }else
            {
                continue;
            };
            sscanf(LINE_ON,"%s%d%d",&chr_2,&start_2,&end_2);
            if (strstr(chr_2,"NC_0000"))
            {
            }else
            {
                continue;
            };

            if (strlen(LINE_ON)>=4998)
            {
                continue;
            };

            //////////////////////

//            printf("BBB\t%s\t%f\t%f\n",LINE_ON,seek_down,seek_tmp);
                if (strcmp(chr,chr_2) < 0)
                {
                    break;
                }else if(strcmp(chr,chr_2) > 0)
                {
                    continue;
                }else
                {
//                    printf("BBB\t%s\t%f\t%f\n",LINE_ON,seek_down,seek_tmp);
                    if (end < start_2)
                    {
                        break;
                    }else if (start > end_2)
                    {
                        continue;
                    }else
                    {
                        report_tag=1;
                        break;
                    };
                };
        };
        //////////////
        if (report_tag==0)
        {
            sprintf(report,"error:not_in_bed\t%s",LINE_IN);
            fputs(report,ERR);
        }else if(report_tag==1)
        {
            fputs(LINE_IN,OUT);
        };
    };

    printf("filter is ended...\n");

    fclose (IN);
    fclose (ON);
    fclose (OUT);
    fclose (ERR);

    return 0;
};
////////////////////////////////
int get_anno(char i[],char o[],char d[])
{
    printf("start to anno...\n\n");
    ////////
    char idx_chr[20];
    sscanf("NC_000001.10","%s",&idx_chr);
    long idx_start=0;
    long idx_end=0;
    float idx_seek=0;
    float idx_diff=5000;
    ////////////
    int idx_tag=1;
    float seek_down=0;
    float seek_tmp=0;
    //////////////////////
    FILE *IN,*ON,*IDX,*OUT;
    char file_idx[500];
    sprintf(file_idx,"%s.idx",d);
    IN=fopen(i,"r");
    ON=fopen(d,"r");
    OUT=fopen(o,"w");
    IDX=fopen(file_idx,"r");

    if (IN==NULL || ON==NULL || IDX==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return 0;
    };
    //////////////////
    char report[50000];
    char LINE_IN[50000];
    char LINE_ON[50000];
    char LINE_IDX[500];
    fgets(LINE_IDX,sizeof(LINE_IDX),IDX);
    LINE_IDX[strlen(LINE_IDX)-1]=' ';

    while(fgets(LINE_IN,sizeof(LINE_IN),IN)!=NULL)
    {
        if (strstr(LINE_IN,"####"))
        {
            fputs(LINE_IN,OUT);
            continue;
        }else if (strstr(LINE_IN,"NC_0000"))
        {
        }else
        {
            fputs(LINE_IN,OUT);
            continue;
        };

        char chr[20];
        long start=0;
        long end=0;
        char ref[500];
        char alt[500];
        char com[500];
        //////////
        sscanf(LINE_IN,"%s%d%d%s%s",chr,&start,&end,ref,alt);
        if (strstr(chr,"NC_0000") && strlen(chr)<=12)
        {
        }else
        {
            fputs(LINE_IN,OUT);
            continue;
        };

        if (strlen(ref) >= 200 || strlen(alt) >= 200 || strlen(LINE_IN)>=4998)
        {
            fputs(LINE_IN,OUT);
            continue;
        };
        sprintf(com,"%s-%s",ref,alt);

        ///////////////////

//        printf("AAA\t%s\t%0.f\t%s\n",LINE_IN,seek_down,chr);

        while (idx_tag==1 && (strcmp(chr,idx_chr)>0 || (strcmp(chr,idx_chr)==0 && start>idx_end)))
        {
            if (idx_seek > seek_down)
            {
                seek_down=idx_seek;
            };
            if (fgets(LINE_IDX,sizeof(LINE_IDX),IDX)!=NULL)
            {
                LINE_IDX[strlen(LINE_IDX)-1]=' ';
                sscanf(LINE_IDX,"%s%d%d%f",&idx_chr,&idx_start,&idx_end,&idx_seek);
            }else
            {
                idx_tag=0;
                break;
            };
            fseek(ON,seek_down,SEEK_SET);
        };
        ///////////////
        float seek_true=ftell(ON);
        seek_true=seek_true-idx_diff;
        if(seek_true<0)
        {
            seek_true=0;
        };
        fseek(ON,seek_true,SEEK_SET);
        //////////////////////
        int report_tag=0;

//        printf("AAA\t%s\t%0.f\n%s\t%d\t%d\n",LINE_IN,seek_down,chr,start,end);
        while(fgets(LINE_ON,sizeof(LINE_ON),ON)!=NULL)
        {
            char chr_2[500];
            long start_2=0;
            long end_2=0;
            char ref_2[500];
            char alt_2[500];
            char com_2[500];
            //////////////////////
            if (strstr(LINE_ON,"####"))
            {
                if (report_tag==1)
                {
                    fputs(LINE_ON,OUT);
                };
                continue;
            }else if (strstr(LINE_ON,"NC_0000") && (LINE_ON[0]=='N'))
            {
                if (report_tag==1)
                {
                    fseek(ON,seek_tmp,SEEK_SET);
                    break;
                };
            }else
            {
                continue;
            };
            sscanf(LINE_ON,"%s%d%d%s%s",chr_2,&start_2,&end_2,ref_2,alt_2);

            if (strstr(chr_2,"NC_0000") && strlen(chr_2)<=12)
            {
            }else
            {
                continue;
            };

            //////////
//            LINE_ON[strlen(LINE_ON)-1]=' ';

            if (strlen(ref_2) >= 200 || strlen(alt_2) >= 200 || strlen(LINE_ON)>=4998)
            {
                continue;
            };
            sprintf(com_2,"%s-%s",ref_2,alt_2);

            //////////////////////
//            printf("BBB\t%s\t%f\t%s\t%d\t%d\n",LINE_ON,seek_down,chr_2,start_2,end_2);

                if (strcmp(chr,chr_2) < 0)
                {
                    break;
                }else if(strcmp(chr,chr_2) > 0)
                {
                    continue;
                }else
                {
                    if (start < start_2)
                    {
                        break;
                    }else if (start > start_2)
                    {
                        continue;
                    }else
                    {
                        if (end < end_2)
                        {
                            break;
                        }else if (end > end_2)
                        {
                            continue;
                        }else
                        {
                            if (strcmp(com,com_2)<0)
                            {
                                break;
                            }else if(strcmp(com,com_2)>0)
                            {
                                continue;
                            }else
                            {
                                fputs(LINE_IN,OUT);
                                report_tag=1;

                                seek_tmp=ftell(ON);
                                continue;
                            };
                        };
                    };
                };
        };
        //////////////
        if (report_tag==0)
        {
            fputs(LINE_IN,OUT);
        };
    };

    printf("annotating is ended...\n");

    fclose (IN);
    fclose (ON);
    fclose (IDX);
    fclose (OUT);
    return 0;
};

//////////////////////////////////////////////////////
int get_annovcf(char i[],char o[],char d[], char n[])
{
    printf("annovcf start...\n\n");
    ////////
    long idx_chr=0;
    long idx_start=0;
	long idx_end=0;
    float idx_seek=0;
    float idx_diff=5000;
    ////////////
    int idx_tag=1;
    float seek_down=0;
    //////////////////////
    FILE *IN,*ON,*IDX,*OUT;
    char file_idx[500];
    sprintf(file_idx,"%s.idx",d);
    IN=fopen(i,"r");
    ON=fopen(d,"r");
    OUT=fopen(o,"w");
    IDX=fopen(file_idx,"r");

    if (IN==NULL || ON==NULL || IDX==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return 0;
    };
    //////////////////
    char report[200000];
    char LINE_IN[100000];
    char LINE_ON[100000];
    char LINE_IDX[50000];
    fgets(LINE_IDX,sizeof(LINE_IDX),IDX);
    LINE_IDX[strlen(LINE_IDX)-1]=' ';

    while(fgets(LINE_IN,sizeof(LINE_IN),IN)!=NULL)
    {
        if(bcmp(LINE_IN,"#",1)==0)
        {
            fputs(LINE_IN,OUT);
            continue;
        };

        long chr=0;
        long end=0;
		char rs[100];
        char ref[500];
        char alt[500];
		char qua[100];
		char filter[500];
		char INFO[50000];
		char other[50000];
		
        char com[1000];
		char rubish_1[50000];
        //////////
        sscanf(LINE_IN,"%s%d%s%s%s%s%s%s%[^\n]",&rubish_1,&end,&rs,ref,alt,&qua,&filter,&INFO,&other);
		if (strlen(rubish_1)>2)
		{
			continue;
		};
		long start=end;
		
		if (strcmp(rubish_1,"X")==0)
		{
			chr=23;
		}else if (strcmp(rubish_1,"Y")==0)
		{
			chr=24;
		}else
		{
			chr=atoi(rubish_1);
		};
		/////////

        if (strlen(ref) >= 500 || strlen(alt) >= 500 || strlen(LINE_IN)>=99998 || end < start)
        {
//          fputs(LINE_IN,OUT);
            continue;
        };
        sprintf(com,"%s-%s",ref,alt);

        ///////////////////

        //printf("AAA\t%s\t%0.f\t%d\n",LINE_IN,seek_down,chr);

        while (idx_tag==1 && (chr > idx_chr|| (chr==idx_chr && start > idx_end)))
        {
            if (idx_seek > seek_down)
            {
                seek_down=idx_seek;
            };
            if (fgets(LINE_IDX,sizeof(LINE_IDX),IDX)!=NULL)
            {
                LINE_IDX[strlen(LINE_IDX)-1]=' ';
				char rubish_idx[20];
                sscanf(LINE_IDX,"%s%d%d%f",&rubish_idx,&idx_start,&idx_end,&idx_seek);
				if (strcmp(rubish_idx,"X")==0)
				{
					idx_chr=23;
				}else if (strcmp(rubish_idx,"Y")==0)
				{
					idx_chr=24;
				}else
				{
					idx_chr=atoi(rubish_idx);
				};
            }else
            {
                idx_tag=0;
                break;
            };
            fseek(ON,seek_down,SEEK_SET);
        };
        ///////////////
        float seek_true=ftell(ON);
        seek_true=seek_true-idx_diff;
        if(seek_true<0)
        {
            seek_true=0;
        };
        fseek(ON,seek_true,SEEK_SET);
        //////////////////////
        int report_tag=0;

        //printf("AAA\t%s\nAAA\t%d\t%d\t%d\t%s\n",LINE_IN,chr,start,end,com);
        while(fgets(LINE_ON,sizeof(LINE_ON),ON)!=NULL)
        {
			//printf ("Acheck\t%d\t%d\n",start,end);
            long chr_2=0;
            long end_2=0;
            char ref_2[50000];
            char alt_2[50000];
            char com_2[50000];
			char rubish_2[50000];
			char info[100000];
            //////////////////////
			if(bcmp(LINE_ON,"#",1)==0)
			{
				continue;
			};
			//printf("BBB\t%s\tBBB\t%d\t%d\t%d\t%s\n",LINE_ON,chr_2,end_2,end_2,com_2);
			/////////
			//printf ("MMM check\t%d\t%d\n",start,end);
			char des[8];
			char *check_chr=strSplit(LINE_ON,des,"\t",1);
			if (strlen(check_chr)>2 || strlen(check_chr)<1)
			{
				continue;
			}else if (sizeof(des) != 8)
			{
				continue;
			};
			//printf ("NNNcheck\t%d\t%d\n",start,end);
			//////////
			sscanf(LINE_ON,"%s%d%*s%s%s%*s%*s%s",&rubish_2,&end_2,&ref_2,&alt_2,&info);
			//printf ("Ccheck\t%d\t%d\n",start,end);			
			long start_2=end_2;
			if (strcmp(rubish_2,"X")==0)
			{
				chr_2=23;
			}else if (strcmp(rubish_2,"Y")==0)
			{
				chr_2=24;
			}else
			{
				chr_2=atoi(rubish_2);
			};
			if (chr_2>0 && chr_2<25 && start_2>0 && start_2<300000000)
			{
			}else
			{
				continue;
			}
            //////////
            if (strlen(ref_2) >= 500 || strlen(alt_2) >= 500 || strlen(LINE_ON)>=49998 || end_2 < start_2)
            {
                continue;
            };
            sprintf(com_2,"%s-%s",ref_2,alt_2);

            //////////////////////
            //printf("CCC\t%s\t%f\t%d\t%d\t%d\t%s\n",LINE_ON,seek_down,chr_2,start_2,end_2,com_2);

                if (chr < chr_2)
                {
                    break;
                }else if(chr > chr_2)
                {
                    continue;
                }else
                {
                    if (start < start_2)
                    {
						//printf("%d < %d \n",start,start_2);
                        break;
                    }else if (start > start_2)
                    {
						//printf("%d > %d \n",start,start_2);
                        continue;
                    }else
                    {
                         if (strcmp(com,com_2)<0)
                         {
                             break;
                         }else if(strcmp(com,com_2)>0)
                         {
                             continue;
                         }else
                         {
							 LINE_IN[strlen(LINE_IN)-1]=';';
							 char str_to[500];
							 char *last;
							 sprintf(str_to,"%s%s%s","_",n,"=");
							 //printf("info\t%s\n",info);
							 last=strreplace_5(info,"=",str_to);
							 //sscanf(LINE_IN,"%s%d%s%s%s%s%s%s%[^\n]",&rubish_1,&end,&rs,ref,alt,&qua,&filter,&INFO,&other);
                             sprintf(report,"%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s;%s\t%s\n",rubish_1,start,rs,ref,alt,qua,filter,INFO,last,other);
							 //printf("%s\n",report);
							 //exit(0);
                             fputs(report,OUT);
                             report_tag=1;
                             break;
                         };
                    };
                };
        };
        //////////////
        if (report_tag==0)
        {
            fputs(LINE_IN,OUT);
        };
    };

    printf("annovcf end...\n");

    fclose (IN);
    fclose (ON);
    fclose (IDX);
    fclose (OUT);
    return 0;
};
//////////////////////////////////////////////////////
////////////////////////////////

int get_db_combine(char i[],char o[],char d[], char n[])
{
    printf("database combine anno...\n\n");
    ////////
    char idx_chr[20];
    sscanf("NC_000001.10","%s",&idx_chr);
    long idx_start=0;
    long idx_end=0;
    float idx_seek=0;
    float idx_diff=5000;
    ////////////
    int idx_tag=1;
    float seek_down=0;
    //////////////////////
    FILE *IN,*ON,*IDX,*OUT;
    char file_idx[500];
    sprintf(file_idx,"%s.idx",d);
    IN=fopen(i,"r");
    ON=fopen(d,"r");
    OUT=fopen(o,"w");
    IDX=fopen(file_idx,"r");

    if (IN==NULL || ON==NULL || IDX==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return 0;
    };
    //////////////////
    char report[50000];
    char LINE_IN[50000];
    char LINE_ON[50000];
    char LINE_IDX[500];
    fgets(LINE_IDX,sizeof(LINE_IDX),IDX);
    LINE_IDX[strlen(LINE_IDX)-1]=' ';

    while(fgets(LINE_IN,sizeof(LINE_IN),IN)!=NULL)
    {
        if (strstr(LINE_IN,"####"))
        {
            fputs(LINE_IN,OUT);
            continue;
        }else if (strstr(LINE_IN,"NC_0000"))
        {
        }else
        {
            fputs(LINE_IN,OUT);
            continue;
        };

        char chr[20];
        long start=0;
        long end=0;
        char ref[500];
        char alt[500];
        char com[500];
        //////////
        sscanf(LINE_IN,"%s%d%d%s%s",chr,&start,&end,ref,alt);

        if (strstr(chr,"####"))
        {
            fputs(LINE_IN,OUT);
            continue;
        }else if (strstr(chr,"NC_0000") && strlen(chr)<=12)
        {
        }else
        {
            fputs(LINE_IN,OUT);
            continue;
        };

        if (strlen(ref) >= 200 || strlen(alt) >= 200 || strlen(LINE_IN)>=4998 || end < start)
        {
//            fputs(LINE_IN,OUT);
            continue;
        };
        sprintf(com,"%s-%s",ref,alt);

        ///////////////////

//        printf("AAA\t%s\t%0.f\t%s\n",LINE_IN,seek_down,chr);

        while (idx_tag==1 && (strcmp(chr,idx_chr)>0 || (strcmp(chr,idx_chr)==0 && start>idx_end)))
        {
            if (idx_seek > seek_down)
            {
                seek_down=idx_seek;
            };
            if (fgets(LINE_IDX,sizeof(LINE_IDX),IDX)!=NULL)
            {
                LINE_IDX[strlen(LINE_IDX)-1]=' ';
                sscanf(LINE_IDX,"%s%d%d%f",&idx_chr,&idx_start,&idx_end,&idx_seek);
            }else
            {
                idx_tag=0;
                break;
            };
            fseek(ON,seek_down,SEEK_SET);
        };
        ///////////////
        float seek_true=ftell(ON);
        seek_true=seek_true-idx_diff;
        if(seek_true<0)
        {
            seek_true=0;
        };
        fseek(ON,seek_true,SEEK_SET);
        //////////////////////
        int report_tag=0;

//        printf("AAA\t%s\t%0.f\n%s\t%d\t%d\n",LINE_IN,seek_down,chr,start,end);
        while(fgets(LINE_ON,sizeof(LINE_ON),ON)!=NULL)
        {
            char chr_2[500];
            long start_2=0;
            long end_2=0;
            char ref_2[500];
            char alt_2[500];
            char com_2[500];
            //////////////////////
            if (strstr(LINE_ON,"NC_0000") && (LINE_ON[0]=='N'))
            {
            }else
            {
                continue;
            };
            sscanf(LINE_ON,"%s%d%d%s%s",chr_2,&start_2,&end_2,ref_2,alt_2);

            if (strstr(chr_2,"NC_0000") && strlen(chr_2)<=12)
            {
            }else
            {
                continue;
            };

            //////////
//            LINE_ON[strlen(LINE_ON)-1]=' ';

            if (strlen(ref_2) >= 200 || strlen(alt_2) >= 200 || strlen(LINE_ON)>=4998 || end_2 < start_2)
            {
                continue;
            };
            sprintf(com_2,"%s-%s",ref_2,alt_2);

            //////////////////////
//            printf("BBB\t%s\t%f\t%s\t%d\t%d\n",LINE_ON,seek_down,chr_2,start_2,end_2);

                if (strcmp(chr,chr_2) < 0)
                {
                    break;
                }else if(strcmp(chr,chr_2) > 0)
                {
                    continue;
                }else
                {
                    if (start < start_2)
                    {
                        break;
                    }else if (start > start_2)
                    {
                        continue;
                    }else
                    {
                        if (end < end_2)
                        {
                            break;
                        }else if (end > end_2)
                        {
                            continue;
                        }else
                        {
                            if (strcmp(com,com_2)<0)
                            {
                                break;
                            }else if(strcmp(com,com_2)>0)
                            {
                                continue;
                            }else
                            {
                                sprintf(report,"%s\t####%s\t%s",LINE_IN,n,LINE_ON);
                                fputs(report,OUT);
                                report_tag=1;
                                break;
                            };
                        };
                    };
                };
        };
        //////////////
        if (report_tag==0)
        {
            fputs(LINE_IN,OUT);
        };
    };

    printf("database combine is ended...\n");

    fclose (IN);
    fclose (ON);
    fclose (IDX);
    fclose (OUT);
    return 0;
};
//////////////////////////////////////////////////////
////////////////////////////////

int get_crush(char d[],char o[], char n[])
{
    printf("database crush start...\n\n");

    //////////////////////
    FILE *IN,*OUT;
    IN=fopen(d,"r");
    OUT=fopen(o,"w");

    if (IN==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return 0;
    };
    //////////////////
    char LINE_IN[50000];
    char crush_tag[500];
    sprintf(crush_tag,"####%s",n);

    while(fgets(LINE_IN,sizeof(LINE_IN),IN)!=NULL)
    {
        if (strstr(LINE_IN,crush_tag))
        {
            ////next;
        }else
        {
            fputs(LINE_IN,OUT);
        };
    };
    printf("database crush ended...\n");
    fclose (IN);
    fclose (OUT);
    return 0;
};
//////////////////////////////////////////////////////
int get_check(char d[])
{
    printf("start to check the database...\n");
    char buff_chr[20];
    sscanf("NC_000001.10","%s",&buff_chr);
    long buff_start=0;
    long buff_end=0;
    char buff_com[500];
    char check='N';

    FILE *ON;
    ON=fopen(d,"r");
    if (ON==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return -1;
    };
    char LINE[50000];
    while ((fgets(LINE,sizeof(LINE),ON))!=NULL)
    {
        if (strstr(LINE,"NC_0000"))
        {
        }else
        {
            continue;
        };

        char chr[20];
        long start=0;
        long end=0;
        char ref[500];
        char alt[500];
        char com_1[500];
//        printf("AAA\t%s\n",LINE);
        //////////
        LINE[strlen(LINE)-1]=' ';
        sscanf(LINE,"%s%d%d%s%s",&chr,&start,&end,&ref,&alt);
        if (chr[0]!=check || strlen(chr)>12)
        {
            continue;
        };


        if (strlen(ref)>=200 || strlen(alt)>=200 || strlen(LINE)>=4998)
        {
            printf("\n\tattention:length_bg_than_200bp ! ignore it !!!\n\t%s\n\t%s\n\t%s\n",ref,alt,LINE);
            continue;
        }else if(end < start)
        {
            printf("\n\terror:\tDatabase format is invalid...\n");
            printf("\n\tend lt start:\t%s\t%d\t%d\t%s\n",chr,start,end,com_1);
            exit(0);
        };

        sprintf(com_1,"%s-%s",ref,alt);
        //////////////
        if(strcmp(ref,"-")==0 && ((end-start)!=1))
        {
            printf("\n\t%s\t%d\t%d\t%s\t%s\n",chr,start,end,ref,alt);
            goto CASE_LAST;                            
        };
        ////////////////
        int wrong=0;
        char reason[500];

        if (strcmp(chr,buff_chr) > 0)
        {
            wrong=0;
        }else if(strcmp(chr,buff_chr) < 0)
        {
            wrong=1;
            sprintf(reason,"%s < %s",chr,buff_chr);
        }else
        {
            if(start < buff_start)
            {
                wrong=1;
                sprintf(reason,"%d<%d:start",start,buff_start);
            }else if (start > buff_start)
            {
                wrong=0;
            }else
            {
                if (end < buff_end)
                {
                    wrong=1;
                    sprintf(reason,"%d<%d:end",end,buff_end);
                }else if (end > buff_end)
                {
                    wrong=0;
                }else
                {
                    if (strcmp(com_1,buff_com)<0)
                    {
                        wrong=1;
                        sprintf(reason,"%s < %s",com_1,buff_com);
                    }else if(strcmp(com_1,buff_com)>0)
                    {
                        wrong=0;
                    }else
                    {
                        wrong=0;
                    };
                };
            };
        };
        /////////////////
        if (wrong==1)
        {
            printf("\n\terror:\tDatabase format is invalid...\n");
            printf("\n\tbefore:\t%s\t%d\t%d\t%s\n",buff_chr,buff_start,buff_end,buff_com);
            printf("\n\tbehind:\t%s\t%d\t%d\t%s\n",chr,start,end,com_1);
            printf("\n\tReason:%s\n",reason);
            goto CASE_LAST;
        }else
        {
            sscanf(chr,"%s",&buff_chr);
            buff_start=start;
            buff_end=end;
            sscanf(com_1,"%s",&buff_com);
        };
    };
    ////////////////
    CASE_LAST:printf("checking is ended ...\n\n");
    return 0;
};
////////////////////////////
//////////////////////////////////////////////////////
int get_checkvcf(char d[])
{
    printf("checkvcf start...\n");
    long buff_chr=0;
    long buff_start=0;
    char buff_com[500];
    char check='N';

    FILE *ON;
    ON=fopen(d,"r");
    if (ON==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return -1;
    };
    char LINE[50000];
    while ((fgets(LINE,sizeof(LINE),ON))!=NULL)
    {
        if (bcmp(LINE,"#",1)==0)
        {
			continue;
        };
        long chr=0;
        long start=0;
        char ref[500];
        char alt[500];
        char com_1[500];
//        printf("AAA\t%s\n",LINE);
        //////////
        LINE[strlen(LINE)-1]=' ';
		char rubish_1[50];
        sscanf(LINE,"%s%d%*s%s%s",&rubish_1,&start,&ref,&alt);
        if (strlen(rubish_1)>2)
        {
            continue;
        };
		
		if (bcmp(rubish_1,"X",1)==0)
		{
			chr=23;
		}else if (bcmp(rubish_1,"Y",1)==0)
		{
			chr=24;
		}else
		{
			chr=atoi(rubish_1);
		};

        if (strlen(ref)>=500 || strlen(alt)>=500 || strlen(LINE)>=49998)
        {
            printf("\n\tattention:length_bg_than_500bp ! ignore it !!!\n\t%s\n\t%s\n\t%s\n",ref,alt,LINE);
            continue;
        };
        sprintf(com_1,"%s-%s",ref,alt);
        ////////////////
        int wrong=0;
        char reason[500];

        if(chr > buff_chr)
        {
            wrong=0;
        }else if(chr < buff_chr)
        {
            wrong=1;
            sprintf(reason,"%d < %d",chr,buff_chr);
        }else
        {
            if(start < buff_start)
            {
                wrong=1;
                sprintf(reason,"%d<%d:start",start,buff_start);
            }else if (start > buff_start)
            {
                wrong=0;
            }else
            {
                if (strcmp(com_1,buff_com)<0)
                {
                    wrong=1;
                    sprintf(reason,"%s < %s",com_1,buff_com);
                }else if(strcmp(com_1,buff_com)>0)
                {
                    wrong=0;
                }else
                {
                    wrong=0;
                };
            };
        };
        /////////////////
        if (wrong==1)
        {
            printf("\n\terror:\tDatabase format is invalid...\n");
            printf("\n\tbefore:\t%d\t%d\t%s\n",buff_chr,buff_start,buff_com);
            printf("\n\tbehind:\t%d\t%d\t%s\n",chr,start,com_1);
            printf("\n\tReason:%s\n",reason);
            goto CASE_LAST;
        }else
        {
			buff_chr=chr;
            buff_start=start;
            sscanf(com_1,"%s",&buff_com);
        };
    };
    ////////////////
    CASE_LAST:printf("checkvcf end ...\n\n");
    return 0;
};
////////////////////////////////////////
int get_sort(char i[],char o[])
{
    printf("start to sort the database...\n\n");

    char program[]="sort -k1,1 -k2,2n -k3,3n -k4,4 -k5,5";
    char command[1000];
    sprintf(command,"%s %s > %s\n",program,i,o);
    system(command);

    printf("sorting is ended ...\n\n");
    return 0;
};
////////////////////////////////////////
int get_sortvcf(char i[],char o[])
{
    printf("sortvcf start...\n\n");
	FILE *IN,*OUT,*HEAD,*OUT22;
	char head_file[100];
	char content_file[100];
	char content22_file[100];
	char tmp1_file[100];
	char tmp2_file[100];
	sprintf(head_file,"%s.header",o);
	sprintf(content_file,"%s.content",o);
	sprintf(content22_file,"%s.content22",o);
	sprintf(tmp1_file,"%s.tmp1",o);
	sprintf(tmp2_file,"%s.tmp2",o);
	
	IN=fopen(i,"r");
	HEAD=fopen(head_file,"w");
	OUT=fopen(content_file,"w");
	OUT22=fopen(content22_file,"w");
	
    if (IN==NULL || OUT==NULL || HEAD==NULL || OUT22==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return -1;
    };
	char LINE[50000];
    while ((fgets(LINE,sizeof(LINE),IN))!=NULL)
    {
        if (bcmp(LINE,"#",1)==0)
        {
            fputs(LINE,HEAD);
        }else if (bcmp(LINE,"X",1)==0)
		{
			fputs(LINE,OUT22);
		}else if (bcmp(LINE,"Y",1)==0)
		{
			fputs(LINE,OUT22);
		}else
		{
			fputs(LINE,OUT);
		};
	};
    fclose (IN);
    fclose (OUT);
	fclose (HEAD);
	fclose (OUT22);
	//////////////////
	char program[]="sort -k1,1n -k2,2n -k4,4 -k5,5";
	char program2[]="sort -k1,1 -k2,2n -k4,4 -k5,5";
    char command[1000];
    sprintf(command,"%s %s > %s ",program,content_file,tmp1_file);
	sprintf(command,"%s && %s %s > %s ",command,program2,content22_file,tmp2_file);
	sprintf(command,"%s && cat %s %s %s > %s ",command,head_file,tmp1_file,tmp2_file,o);
	sprintf(command,"%s && rm %s %s %s %s %s \n",command,head_file,content_file,content22_file,tmp1_file,tmp2_file);
//	printf("%s\n",command);
    system(command);

    printf("sortvcf end ...\n\n");
    return 0;
};
////////////////////////////////////////
int get_index(char d[])
{
    printf("start to index the database ...\n\n");
    FILE *IN,*OUT;
    char out_file[100];
    sprintf(out_file,"%s.idx",d);

    IN=fopen(d,"r");
    OUT=fopen(out_file,"w");

    if (IN==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return -1;
    };
    char LINE[50000];
    char buff_chr[20];buff_chr[0]='\0';
    long buff_start=0;
    long buff_end=0;
    float seek=0;
    char report[1000];

    while(fgets(LINE,sizeof(LINE),IN)!=NULL)
    {
        if (strstr(LINE,"NC_0000"))
        {
        }else
        {
            continue;
        };

        char tmp_chr[20];
        long tmp_start=0;
        long tmp_end=0;
        sscanf(LINE,"%s%d%d",&tmp_chr,&tmp_start,&tmp_end);

        if (strstr(tmp_chr,"NC_0000") && strlen(tmp_chr)<=12)
        {
        }else
        {
            continue;
        };


        if (strlen(LINE)>=4998)
        {
            printf("%s\n\terror:length_bg_than_5000 bits ! ignore it !!!\n",LINE);
            continue;
        };

        if (buff_chr==NULL || strlen(buff_chr)<1)
        {
            sscanf(tmp_chr,"%s",&buff_chr);
            buff_start=tmp_start;
            seek=ftell(IN);
            sprintf(report,"%s\t%d\t%d\t%.0f\n",tmp_chr,tmp_start,tmp_end,seek);
            fputs(report,OUT);
        }else
        {
            if (strcmp(tmp_chr,buff_chr)==0)
            {
                if ((tmp_start - buff_start) > 100000)
                {
                    buff_start=tmp_start;
                    seek=ftell(IN);
                    sprintf(report,"%s\t%d\t%d\t%.0f\n",tmp_chr,tmp_start,tmp_end,seek);
                    fputs(report,OUT);
                };
            }else if(strcmp(tmp_chr,buff_chr)<0)
            {
                printf("\n\t the database file is error !!!\n\n");
                exit(0);
            }else if(strcmp(tmp_chr,buff_chr)>0)
            {
                sscanf(tmp_chr,"%s",&buff_chr);
                buff_start=tmp_start;
                seek=ftell(IN);
                sprintf(report,"%s\t%d\t%d\t%.0f\n",tmp_chr,tmp_start,tmp_end,seek);
                fputs(report,OUT);
            };
        };
    };
    fclose (IN);
    fclose (OUT);
    printf("indexing is ended ...\n");
    return(0);
};
//////////////
int get_indexvcf(char d[])
{
    printf("indexvcf start...\n\n");
    FILE *IN,*OUT;
    char out_file[100];
    sprintf(out_file,"%s.idx",d);

    IN=fopen(d,"r");
    OUT=fopen(out_file,"w");

    if (IN==NULL || OUT==NULL)
    {
        printf("Attention:\tmust be something wrong in this files, please check them !\n");
        return -1;
    };
    char LINE[50000];
    long buff_chr=0;
    long buff_start=0;
    long buff_end=0;
    float seek=0;
    char report[1000];

    while(fgets(LINE,sizeof(LINE),IN)!=NULL)
    {
	
		if (bcmp(LINE,"#",1)==0)
		{
			continue;
		};
		
        long tmp_chr=0;
        long tmp_start=0;
		char rubish_1[10];
        sscanf(LINE,"%s%d",&rubish_1,&tmp_start);
		if (strcmp(rubish_1,"X")==0)
		{
			tmp_chr=23;
		}else if (strcmp(rubish_1,"Y")==0)
		{
			tmp_chr=24;
		}else
		{
			tmp_chr=atoi(rubish_1);
		};
		long tmp_end=tmp_start;
		
        if (strlen(LINE)>=49999)
        {
            printf("%s\n\terror:length_bg_than_5000 bits ! ignore it !!!\n",LINE);
            continue;
        };

        if (buff_chr==0)
        {
			buff_chr=tmp_chr;
            buff_start=tmp_start;
            seek=ftell(IN);
            sprintf(report,"%s\t%d\t%d\t%.0f\n",rubish_1,tmp_start,tmp_end,seek);
            fputs(report,OUT);
        }else
        {
            if (tmp_chr==buff_chr)
            {
                if ((tmp_start - buff_start) > 100000)
                {
                    buff_start=tmp_start;
                    seek=ftell(IN);
                    sprintf(report,"%s\t%d\t%d\t%.0f\n",rubish_1,tmp_start,tmp_end,seek);
                    fputs(report,OUT);
                };
            }else if(tmp_chr < buff_chr)
            {
                printf("\n\t the database file is error !!!\n\n");
                exit(0);
            }else if(tmp_chr > buff_chr)
            {
				buff_chr=tmp_chr;
                buff_start=tmp_start;
                seek=ftell(IN);
                sprintf(report,"%s\t%d\t%d\t%.0f\n",rubish_1,tmp_start,tmp_end,seek);
                fputs(report,OUT);
            };
        };
    };
    fclose (IN);
    fclose (OUT);
    printf("indexvcf end...\n");
    return(0);
};
//////////////
int help(char prog[],char cmd[],char i[],char o[],char d[],char n[])
{
    if (strlen(cmd)>0 && strcmp(cmd,"check")==0)
    {
        if (strlen(d)<1)
        {
            printf("Usage:\tannotools check -d [database]\n\n");
            return -1;
        }else
        {
            get_check(d);
            return 0;
        };
    }else if (strlen(cmd)>0 && strcmp(cmd,"sort")==0)
    {
        if (strlen(i)<1 || strlen(o)<1)
        {
            printf("Usage:\tannotools sort -i [database] -o [db_sorted]\n\n");
            return -1;
        }else
        {
            get_sort(i,o);
            return 0;
        };
    }else if (strlen(cmd)>0 && strcmp(cmd,"index")==0)
    {
        if (strlen(d)<1)
        {
            printf("Usage:\tannotools index -d [database]\n\n");
            return -1;
        }else
        {
            get_index(d);
            return 0;
        };
    }else if(strlen(cmd)>0 && strcmp(cmd,"intersect")==0)
    {
        if (strlen(d)<1|| strlen(i)<1 || strlen(o)<1 || strlen(n)<1)
        {
            printf("Usage:\tannotools intersect -i [bed] -d [database] -o [outfile] -n [dadabase_name]\n");
            printf("\t-i\tOnly with the following format:\n");
            printf("\t\tchr1\t69270\t69270\n");
            printf("\t\tchr1\t69270\t69271\n");
            printf("\t\tchr1\t69271\t69271\n");
            printf("\t-o\tOut file\n");
            printf("\t-d\tDatabase that needs to be annotated, file format refer to [-i]\n");
            printf("\t-n\tThe name of additional database\n");
            return -1;
        }else
        {
            get_intersect(i,o,d,n);
            return 0;
        };
    }else if(strlen(cmd)>0 && strcmp(cmd,"filter")==0)
    {
        if (strlen(d)<1|| strlen(i)<1 || strlen(o)<1)
        {
            printf("Usage:\tannotools filter -i [bed] -d [database] -o [outfile]\n");
            printf("\t-i\tOnly with the following format:\n");
            printf("\t\tchr1\t69270\t69270\n");
            printf("\t\tchr1\t69270\t69271\n");
            printf("\t\tchr1\t69271\t69271\n");
            printf("\t-d\tDatabase that needs to be annotated, file format refer to [-i]\n");
            printf("\t-o\tOut file\n");
            return -1;
        }else
        {
            get_filter(i,o,d);
            return 0;
        };
    }else if(strlen(cmd)>0 && strcmp(cmd,"combine")==0)
    {
        if (strlen(d)<1|| strlen(i)<1 || strlen(o)<1 || strlen(n)<1)
        {
            printf("Usage:\tannotools combine -i [database] -d [additional database] -o [outfile] -n [database_name]\n");
            printf("\t-i\tIntegrated database\n");
            printf("\t-d\tAdditional database\n");
            printf("\t-n\tThe name of additional database\n");
            printf("\t-o\tOut file\n");
            return -1;
        }else
        {
            get_db_combine(i,o,d,n);
            return 0;
        };
    }else if(strlen(cmd)>0 && strcmp(cmd,"crush")==0)
    {
        if (strlen(d)<1 || strlen(o)<1 || strlen(n)<1)
        {
            printf("Usage:\tannotools crush -d [database] -o [outfile] -n [database to be shredded]\n");
            printf("\t-d\tIntegrated database\n");
            printf("\t-n\tThe name of database to be shredded\n");
            printf("\t-o\tOut file\n");
            return -1;
        }else
        {
            get_crush(d,o,n);
            return 0;
        };
    }else if(strlen(cmd)>0 && strcmp(cmd,"anno")==0)
    {
        if (strlen(d)<1|| strlen(i)<1 || strlen(o)<1)
        {
            printf("Usage:\tannotools anno -i [bed] -d [database] -o [outfile]\n");
            printf("\t-i\tOnly with the following format:\n");
            printf("\t\tchr1\t69270\t69270\tA\tG\n");
            printf("\t\tchr1\t69270\t69271\t-\tG\n");
            printf("\t\tchr1\t69270\t69270\tA\t-\n");
            printf("\t-d\tDatabase that needs to be annotated, file format refer to [-i]\n");
            printf("\t-o\tOut file\n");
            return -1;
        }else
        {
            get_anno(i,o,d);
            return 0;
        };
    }else if(strlen(cmd)>0 && strcmp(cmd,"annovcf")==0)
    {
        if (strlen(d)<1|| strlen(i)<1 || strlen(o)<1 || strlen(n)<1)
        {
            printf("Usage:\tannotools annovcf -i [input.vcf] -d [database.vcf] -o [out.vcf] -n [database_name]\n");
            printf("\t-i\tinput vcf path\n");
            printf("\t-d\tdatabase vcf path\n");
            printf("\t-n\tname of the additional database\n");
            printf("\t-o\tout put vcf path\n");
            return -1;
        }else
        {
            get_annovcf(i,o,d,n);
            return 0;
        };		
    }else if (strlen(cmd)>0 && strcmp(cmd,"indexvcf")==0)
    {
        if (strlen(d)<1)
        {
            printf("Usage:\tannotools indexvcf -d [database]\n\n");
            return -1;
        }else
        {
            get_indexvcf(d);
            return 0;
        };
    }else if (strlen(cmd)>0 && strcmp(cmd,"checkvcf")==0)
    {
        if (strlen(d)<1)
        {
            printf("Usage:\tannotools checkvcf -d [database]\n\n");
            return -1;
        }else
        {
            get_checkvcf(d);
            return 0;
        };
    }else if (strlen(cmd)>0 && strcmp(cmd,"sortvcf")==0)
    {
        if (strlen(i)<1 || strlen(o)<1)
        {
            printf("Usage:\tannotools sortvcf -i [database] -o [db_sorted]\n\n");
            return -1;
        }else
        {
            get_sortvcf(i,o);
            return 0;
        };
    }else
    {
        printf("Usage:\tannotools <subcommand> [options]\nversion:v0.5.1\n");

		printf("\tbed format processing package:\n");
        printf("\t\t[check]\t\t\tto check whether the database meets the standard format.\n");
        printf("\t\t[sort]\t\t\tto sort the database.\n");
        printf("\t\t[index]\t\t\tto index the database.\n");
        printf("\t\t[filter]\t\tto filter the data not in the bed.\n");
        printf("\t\t[combine]\t\tto combine the databases.\n");
        printf("\t\t[crush]\t\t\tto smash the trash in the database.\n");
        printf("\t\t[intersect]\t\tto anno the database by finding intersection.\n");
        printf("\t\t[anno]\t\t\tto anno the database by finding the mutation.\n\n");
		
		printf("\tvcf format processing package:\n");
		printf("\t\t[annovcf]\t\tto anno the vcf by finding the mutation. vcf database should be sort by annotools.\n");
		printf("\t\t[indexvcf]\t\tto index the vcf file.\n");
		printf("\t\t[checkvcf]\t\tto check whether the vcf meets the standard format.\n");
		printf("\t\t[sortvcf]\t\tto sort the vcf database.\n");
		
        return -1;
    };
    return 1;
};

