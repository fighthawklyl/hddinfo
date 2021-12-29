/*************************************************************************
> File Name: hddinfo.c
> Author: zhangchen
> Mail:zhangchen@xiongmaitech.com
************************************************************************/

#include<stdio.h>
#include<string.h>

int LibDvr_getSdxInfo(char sdx[16][5])
{
    char line[64];
    char *sdxname;
    FILE *fp;

    fp = fopen("/proc/partitions","r");
    if(fp == NULL)
    {
        perror("open /proc/partitions failure:%s");
		fclose(fp);
        return -1;
    }
    char *buff = NULL;
    int m = 0;
    //int ret = -1;

    do{
        buff = fgets(line,sizeof(line),fp);
        //printf("%s",line);
        if(buff != NULL)
        {
            int n = -1;
            sdxname = strstr(line,"sd");
            if(sdxname)
            {
                while(sdxname[++n] != '\n')
                {
                    sdx[m][n] = sdxname[n];
                    //ret = strncmp(sdx[m],sdxname,3);
                    //printf("%d\n",ret);
                    //sdx[s][4] = '\0';
                }
                sdx[m][n] = '\0';
                printf("getsdxinfo fun--->sdx[%d]:%s\n",m,sdx[m]);
                m++;
            }
        }
    }while(buff != NULL);
    fclose(fp);
    return m;
}

int WriteSdxInfo(void)
{
    int ret = -1;
    int i;
	char sdx[16][5] = {'\0'};
	char hdparm_pathname[64] = {'\0'};
	char hddtemp_pathname[64] = {'\0'};

    ret = LibDvr_getSdxInfo(sdx);
    for( i = 0; i < ret;i++ )
    {
        printf("sdx:%s\n",sdx[i]);
		if(strstr(sdx[i],"sd"))
		{
			sprintf(hdparm_pathname,"hdparm -I /dev/%s > /var/%s_hdparm.txt",sdx[i],sdx[i]);
			printf("hdparm_pathname:%s\n",hdparm_pathname);
			system(hdparm_pathname);
			
			sprintf(hddtemp_pathname,"hddtemp -n /dev/%s > /var/%s_hddtemp.txt",sdx[i],sdx[i]);
			printf("hddtemp_pathname:%s\n",hddtemp_pathname);
			system(hddtemp_pathname);
		}
		else
		{
			printf("get hdparm path name failure\n");
			return -1;
		}			
    }
    return 0;
}

int main(int argc, char *argv[])
{
	//循环检测硬盘信息
	while(1)
	{
		WriteSdxInfo();
		sleep(60);
	}
	return 0;
}



