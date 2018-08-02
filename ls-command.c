#include "ls-command.h"
int isFile(const char* name)
{
	DIR* directory = opendir(name);

	if(directory != NULL)
	{
		closedir(directory);
		return 0;
	}

	if(errno == ENOTDIR)
	{
		return 1;
	}

	return -1;
}
int file_select(const struct direct *entry){
	if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
		return 0;
	else
		return 1;
}
void lsCmd(char** k,int ll, char typeBitString[]){
	int i = ll;
	bool redirect = false, done = false, op=false;
	int fd0, childPid= -1;
	if(strcmp(typeBitString,"0100")==0||strcmp(typeBitString,"0110")==0){
		done = true;
		char* file = (char*)malloc(sizeof(char)*1000);
		file = "none\0";
		int cnt=0,fd0;
		for(;cnt<(i-1);cnt++){
			if(strstr(k[cnt],">")||strstr(k[cnt],">>")){
				if(cnt==(i-2)&&((strcmp(k[cnt],">")==0)||(strcmp(k[cnt],">>")==0)))
					printf("Output file not specified\n");
				else{
					if(strstr(k[cnt],">>")){
						if(strlen(k[cnt])>2)
							file = strtok(k[cnt],">>");
						else
							file=k[cnt+1];
					}
					else if(strstr(k[cnt],">")){
						if(strlen(k[cnt])>1)
							file = strtok(k[cnt],">");
						else
							file=k[cnt+1];
					}
					ll = cnt+1;
					break;
				}
			}
		}
		if(strcmp(file,"none")!=0){
			op = true;
			if(strcmp(typeBitString,"0100")==0)
				fd0=open(file, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd0=open(file, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			redirect = true;
			childPid = fork();
			if (childPid==0)
				dup2(fd0,1);

		}
	}
	else if(strcmp(typeBitString,"1100")==0||strcmp(typeBitString,"1110")==0){
		done = true;
		char* inputFile = (char*)malloc(sizeof(char)*1000);
		char* outputFile = (char*)malloc(sizeof(char)*1000);
		outputFile = "none\0";
		inputFile = "none\0";
		int cnt=0,fd0,fd1;
		for(cnt=0;cnt<(i-1);cnt++){
			if(strstr(k[cnt],">")||strstr(k[cnt],">>")){
				if(cnt==(i-2)&&((strcmp(k[cnt],">")==0)||(strcmp(k[cnt],">>")==0)))
					printf("Output file not specified\n");
				else{
					if(strstr(k[cnt],">>")){
						if(strlen(k[cnt])>2)
							outputFile = strtok(k[cnt],">>");
						else
							outputFile=k[cnt+1];
					}
					else if(strstr(k[cnt],">")){
						if(strlen(k[cnt])>1)
							outputFile = strtok(k[cnt],">");
						else
							outputFile=k[cnt+1];
					}
					ll = cnt+1;
					break;
				}
			}
		}
		for(cnt=0;cnt<(i-1);cnt++){
			if(strstr(k[cnt],"<")){
				if(cnt==(i-2)&&(strcmp(k[cnt],"<")==0))
					printf("Input file not specified\n");
				else{
					if(strlen(k[cnt])>1)
						inputFile = strtok(k[cnt],"<");
					else
						inputFile=k[cnt+1];
					ll = cnt+1;
					break;
				}
			}
		}
		if((strcmp(outputFile,"none")!=0)||(strcmp(inputFile,"none")!=0)){
			if(strcmp(outputFile,"none")==0){
				printf("Invalid command. Exiting...\n");
				return;
			}
			if(strstr(outputFile,">>"))
				strtok(outputFile,">>");
			else if(strstr(outputFile,">"))
				strtok(outputFile,">");
			op = true;
			if(strcmp(typeBitString,"1100")==0)
				fd0=open(outputFile, O_CREAT|O_WRONLY|O_TRUNC, 0777 );
			else
				fd0=open(outputFile, O_APPEND|O_WRONLY|O_CREAT, 0777 );
			if(strcmp(inputFile,"none")!=0){
				redirect = true;
				fd1=open(inputFile, O_RDONLY);
				char* buffer = (char*) malloc(sizeof(char)*1024);
				if(read(fd1, buffer, 1024)!=-1){
					close(fd1);
					char* newCmd=strtok(buffer,"\n");
					strcpy(k[ll-1],buffer);
					ll++;
				}
				else{
					redirect = false;
					printf("ERROR: Input file does not exist.\n");
					close(fd0);
					return;
				}
			}
			childPid = fork();
			if (childPid==0)
				dup2(fd0,1);
		}
	}
	else if(strcmp(typeBitString,"1000")==0){
		done = true;
		char* file = (char*)malloc(sizeof(char)*1000);
		file = "none\0";
		int cnt=0,fd1;
		for(;cnt<(i-1);cnt++){
			if(strstr(k[cnt],"<")){
				if(cnt==(i-2)&&(strcmp(k[cnt],"<")==0))
					printf("Input file not specified\n");
				else{
					if(strlen(k[cnt])>1)
						file=strtok(k[cnt],"<");
					else
						file=k[cnt+1];
					ll = cnt+1;
					break;
				}
			}
		}
		if(strcmp(file,"none")!=0){
			redirect = true;
			fd1=open(file, O_RDONLY);
			char* buffer = (char*) malloc(sizeof(char)*1024);
			if(read(fd1, buffer, 1024)!=-1){
				close(fd1);
				char* newCmd=strtok(buffer,"\n");
				strcpy(k[ll-1],buffer);
				ll++;
			}
			else{
				redirect = false;
				printf("ERROR: Input file does not exist.\n");
				return;
			}
		}
	}
	if(childPid==-1 || childPid==0){
		if(!done||op||(redirect&&!op)){
			char c[10000];
			struct stat buf;
			int count,i,no=0;
			char s[100][1000];
			DIR* dir;
			ll--;
			if(ll==0)return ;
			else if(ll==2 && strcmp(k[0],"ls")==0 && strcmp(k[1],"~")==0){
				dir=opendir(homeDir);
				while(files=readdir(dir))if(files->d_name[0]!='.')printf("%s  ",files->d_name);
				printf("\n");
				closedir(dir);
			}	
			else if(ll==2 && ((strcmp(k[0],"ls")==0) && (strcmp(k[1],"-a")==0 ))){
				getcwd(pathname,sizeof(pathname));
				dir=opendir(pathname);
				while(files=readdir(dir))printf("%s  ",files->d_name);
				printf("\n");
				closedir(dir);
			}	
			else if(ll==2 && ((strcmp(k[0],"ls")==0) && (strcmp(k[1],"-l")==0 ))){
				getcwd(pathname,sizeof(pathname));
				dir=opendir(pathname);
				register struct passwd *pw;
				register uid_t uid;
				register gid_t gid;
				struct group *g;
				uid = geteuid ();
				gid = getgid ();
				pw = getpwuid (uid);
				struct group *grp;
				grp = getgrgid(gid);
				char ano[1000000];
				while(files=readdir(dir)){
					if(files->d_name[0]!='.'){
						char file[2]="-\0";
						char userReadFile[2]="-\0";
						char userWriteFile[2] = "-\0";
						char userExecuteFile[2] = "-\0";
						char groupReadFile[2] = "-\0";
						char groupWriteFile[2] = "-\0";
						char groupExecuteFile[2] = "-\0";
						char othersReadFile[2] = "-\0";
						char othersWriteFile[2] = "-\0";
						char othersExecuteFile[2] = "-\0";
						struct stat ret;
						stat(files->d_name, &ret);
						if(ret.st_mode & S_IRUSR){
							strcpy(userReadFile,"r");
						}
						if(ret.st_mode & S_IWUSR){
							strcpy(userWriteFile, "w");	
						}
						if(ret.st_mode & S_IXUSR){
							strcpy(userExecuteFile,"x");
						}
						if(ret.st_mode & S_IRGRP){
							strcpy(groupReadFile,"r");
						}
						if(ret.st_mode & S_IWGRP){
							strcpy(groupWriteFile,"w");
						}
						if(ret.st_mode & S_IXGRP){
							strcpy(groupExecuteFile,"x");
						}
						if(ret.st_mode & S_IROTH){
							strcpy(othersReadFile,"r");
						}
						if(ret.st_mode & S_IWOTH){
							strcpy(othersWriteFile,"w");
						}
						if(ret.st_mode & S_IXOTH){
							strcpy(othersExecuteFile,"x");
						}
						if(isFile(files->d_name)==0)strcpy(file,"d");
						printf("%s%s%s%s%s%s%s%s%s%c ",file,userReadFile,userWriteFile,userExecuteFile,groupReadFile,groupWriteFile,groupExecuteFile,othersReadFile,othersWriteFile,othersExecuteFile[0]);
						stat(files->d_name,&buf);
						printf("%ld %s %s %ld %.16s %s\n",buf.st_nlink,pw->pw_name,grp->gr_name,buf.st_size,ctime(&buf.st_mtime),files->d_name);
						int g=0;
					}
				}
				closedir(dir);
			}
			else if((ll==3 && (((strcmp(k[1],"~")==0) && (strcmp(k[2],"-a")==0))))||ll==3&&strcmp(k[2],"~")==0&&(strcmp(k[1],"-a")==0)){
				dir=opendir(homeDir);
				while(files=readdir(dir))if(files->d_name[0]!='.')printf("%s  ",files->d_name);
				printf("\n");
				closedir(dir);
			}	
			else if((ll==3 && (((strcmp(k[1],"~")==0) && (strcmp(k[2],"-l")==0))))||ll==3&&strcmp(k[2],"~")==0&&(strcmp(k[1],"-l")==0)){
				dir=opendir(homeDir);
				register struct passwd *pw;
				register uid_t uid;
				register gid_t gid;
				struct group *g;
				uid = geteuid ();
				gid = getgid ();
				pw = getpwuid (uid);
				struct group *grp;
				grp = getgrgid(gid);
				char ano[1000000];
				while(files=readdir(dir)){
					if(files->d_name[0]!='.'){
						char file[2]="-\0";
						char userReadFile[2]="-\0";
						char userWriteFile[2] = "-\0";
						char userExecuteFile[2] = "-\0";
						char groupReadFile[2] = "-\0";
						char groupWriteFile[2] = "-\0";
						char groupExecuteFile[2] = "-\0";
						char othersReadFile[2] = "-\0";
						char othersWriteFile[2] = "-\0";
						char othersExecuteFile[2] = "-\0";
						struct stat ret;
						stat(files->d_name, &ret);
						if(ret.st_mode & S_IRUSR){
							strcpy(userReadFile,"r");
						}
						if(ret.st_mode & S_IWUSR){
							strcpy(userWriteFile, "w");	
						}
						if(ret.st_mode & S_IXUSR){
							strcpy(userExecuteFile,"x");
						}
						if(ret.st_mode & S_IRGRP){
							strcpy(groupReadFile,"r");
						}
						if(ret.st_mode & S_IWGRP){
							strcpy(groupWriteFile,"w");
						}
						if(ret.st_mode & S_IXGRP){
							strcpy(groupExecuteFile,"x");
						}
						if(ret.st_mode & S_IROTH){
							strcpy(othersReadFile,"r");
						}
						if(ret.st_mode & S_IWOTH){
							strcpy(othersWriteFile,"w");
						}
						if(ret.st_mode & S_IXOTH){
							strcpy(othersExecuteFile,"x");
						}
						if(isFile(files->d_name)==0)strcpy(file,"d");
						printf("%s%s%s%s%s%s%s%s%s%c ",file,userReadFile,userWriteFile,userExecuteFile,groupReadFile,groupWriteFile,groupExecuteFile,othersReadFile,othersWriteFile,othersExecuteFile[0]);
						stat(files->d_name,&buf);
						printf("%ld %s %s %ld %.16s %s\n",buf.st_nlink,pw->pw_name,grp->gr_name,buf.st_size,ctime(&buf.st_mtime),files->d_name);
					}
				}
				closedir(dir);

			}	
			else if(ll>=2 && (((strcmp(k[0],"ls")==0) && (strcmp(k[1],"-al")==0)) || ((strcmp(k[1],"-a")==0) && (strcmp(k[2],"-l")==0)) || (strcmp(k[1],"-la")==0) || (strcmp(k[1],"-a-l")==0)||(strcmp(k[0],"ls")==0&&strcmp(k[1],"-l")==0&&strcmp(k[2],"-a")==0))){
				DIR* dir;
				getcwd(pathname,sizeof(pathname));
				dir=opendir(pathname);
				register struct passwd *pw;
				register uid_t uid;
				register gid_t gid;
				struct group *g;
				uid = geteuid ();
				gid = getgid ();
				pw = getpwuid (uid);
				struct group *grp;
				grp = getgrgid(gid);
				char ano[1000000];
				count=1;
				if(count <= 0)printf("\n");
				while(files=readdir(dir)){
					char file[2]="-\0";
					char userReadFile[2]="-\0";
					char userWriteFile[2] = "-\0";
					char userExecuteFile[2] = "-\0";
					char groupReadFile[2] = "-\0";
					char groupWriteFile[2] = "-\0";
					char groupExecuteFile[2] = "-\0";
					char othersReadFile[2] = "-\0";
					char othersWriteFile[2] = "-\0";
					char othersExecuteFile[2] = "-\0";
					struct stat ret;
					stat(files->d_name, &ret);
					if(ret.st_mode & S_IRUSR){
						strcpy(userReadFile,"r");
					}
					if(ret.st_mode & S_IWUSR){
						strcpy(userWriteFile, "w");	
					}
					if(ret.st_mode & S_IXUSR){
						strcpy(userExecuteFile,"x");
					}
					if(ret.st_mode & S_IRGRP){
						strcpy(groupReadFile,"r");
					}
					if(ret.st_mode & S_IWGRP){
						strcpy(groupWriteFile,"w");
					}
					if(ret.st_mode & S_IXGRP){
						strcpy(groupExecuteFile,"x");
					}
					if(ret.st_mode & S_IROTH){
						strcpy(othersReadFile,"r");
					}
					if(ret.st_mode & S_IWOTH){
						strcpy(othersWriteFile,"w");
					}
					if(ret.st_mode & S_IXOTH){
						strcpy(othersExecuteFile,"x");
					}

					if(isFile(files->d_name)==0)strcpy(file,"d");
					printf("%s%s%s%s%s%s%s%s%s%s  ",file,userReadFile,userWriteFile,userExecuteFile,groupReadFile,groupWriteFile,groupExecuteFile,othersReadFile,othersWriteFile,othersExecuteFile);
					stat(files->d_name,&buf);
					printf("%ld %s %s %ld %.16s %s\n",buf.st_nlink,pw->pw_name,grp->gr_name,buf.st_size,ctime(&buf.st_mtime),files->d_name);
				}
				closedir(dir);
			}
			else if(ll>=2){
				int flag=0;
				if(ll==2){
					DIR * checkDir=opendir(k[1]);
					if((strcmp(k[0],"ls")==0)&&checkDir){
						while(files=readdir(checkDir))if(files->d_name[0]!='.')printf("%s  ",files->d_name);
						printf("\n");
						closedir(checkDir);
						flag=1;
					}
					else printf("ls: %s: No such file or directory\n",k[1]);

				}
				else if(ll==3){
					DIR *checkDir=opendir(k[1]);
					if(!checkDir){
						DIR *ano=opendir(k[2]);
						if(ano){
							if((strcmp(k[0],"ls")==0)&&strcmp(k[1],"-a")==0){
								while(files=readdir(ano))printf("%s  ",files->d_name);
								printf("\n");
								flag=1;
							}
						}
					}
					else if((strcmp(k[0],"ls")==0)&&strcmp(k[2],"-a")==0){
						flag=1;
						while(files=readdir(checkDir))printf("%s  ",files->d_name);
					}
				}
				if(ll==3){
					register struct passwd *pw;
					register uid_t uid;
					register gid_t gid;
					struct group *g;
					uid = geteuid ();
					gid = getgid ();
					char directx[10000];
					strcpy(directx,k[2]);
					pw = getpwuid (uid);
					struct group *grp;
					grp = getgrgid(gid);
					DIR *checkDir=opendir(k[1]);
					if(!checkDir){
						DIR *ano=opendir(k[2]);
						if(ano){
							if((strcmp(k[0],"ls")==0)&&strcmp(k[1],"-l")==0){
								flag=1;
								register struct passwd *pw;
								register uid_t uid;
								register gid_t gid;
								struct group *g;
								DIR * kkz=opendir(k[2]);
								uid = geteuid ();
								gid = getgid ();
								pw = getpwuid (uid);
								struct group *grp;
								grp = getgrgid(gid);
								strcat(k[2],"/");
								while(files=readdir(kkz)){
									if(files->d_name[0]!='.'){
										char file[2]="-\0";
										char userReadFile[2]="-\0";
										char userWriteFile[2] = "-\0";
										char userExecuteFile[2] = "-\0";
										char groupReadFile[2] = "-\0";
										char groupWriteFile[2] = "-\0";
										char groupExecuteFile[2] = "-\0";
										char othersReadFile[2] = "-\0";
										char othersWriteFile[2] = "-\0";
										char othersExecuteFile[2] = "-\0";
										struct stat ret = {0};
										char temp[1000];
										strcpy(temp,k[2]);
										strcat(temp,files->d_name);
										stat(temp, &ret);
										if(ret.st_mode & S_IRUSR){
											strcpy(userReadFile,"r");
										}
										if(ret.st_mode & S_IWUSR){
											strcpy(userWriteFile, "w");	
										}
										if(ret.st_mode & S_IXUSR){
											strcpy(userExecuteFile,"x");
										}
										if(ret.st_mode & S_IRGRP){
											strcpy(groupReadFile,"r");
										}
										if(ret.st_mode & S_IWGRP){
											strcpy(groupWriteFile,"w");
										}
										if(ret.st_mode & S_IXGRP){
											strcpy(groupExecuteFile,"x");
										}
										if(ret.st_mode & S_IROTH){
											strcpy(othersReadFile,"r");
										}
										if(ret.st_mode & S_IWOTH){
											strcpy(othersWriteFile,"w");
										}
										if(ret.st_mode & S_IXOTH){
											strcpy(othersExecuteFile,"x");
										}
										if(isFile(files->d_name)==0)strcpy(file,"d");
										printf("%s%s%s%s%s%s%s%s%s%c ",file,userReadFile,userWriteFile,userExecuteFile,groupReadFile,groupWriteFile,groupExecuteFile,othersReadFile,othersWriteFile,othersExecuteFile[0]);
										stat(files->d_name,&buf);
										printf("%ld %s %s %ld %.16s %s\n",buf.st_nlink,pw->pw_name,grp->gr_name,buf.st_size,ctime(&buf.st_mtime),files->d_name);
									}
								}
								closedir(dir);
							}
						}
						else if(!flag)printf("ls: %s: No such file or directory\n",directx);	
					}
					else if((strcmp(k[0],"ls")==0)&&strcmp(k[2],"-l")==0){
						while(files=readdir(checkDir)){
							if(files->d_name[0]!='.'){
								char file[2]="-\0";
								char userReadFile[2]="-\0";
								char userWriteFile[2] = "-\0";
								char userExecuteFile[2] = "-\0";
								char groupReadFile[2] = "-\0";
								char groupWriteFile[2] = "-\0";
								char groupExecuteFile[2] = "-\0";
								char othersReadFile[2] = "-\0";
								char othersWriteFile[2] = "-\0";
								char othersExecuteFile[2] = "-\0";
								struct stat ret;
								stat(files->d_name, &ret);
								if(ret.st_mode & S_IRUSR){
									strcpy(userReadFile,"r");
								}
								if(ret.st_mode & S_IWUSR){
									strcpy(userWriteFile, "w");	
								}
								if(ret.st_mode & S_IXUSR){
									strcpy(userExecuteFile,"x");
								}
								if(ret.st_mode & S_IRGRP){
									strcpy(groupReadFile,"r");
								}
								if(ret.st_mode & S_IWGRP){
									strcpy(groupWriteFile,"w");
								}
								if(ret.st_mode & S_IXGRP){
									strcpy(groupExecuteFile,"x");
								}
								if(ret.st_mode & S_IROTH){
									strcpy(othersReadFile,"r");
								}
								if(ret.st_mode & S_IWOTH){
									strcpy(othersWriteFile,"w");
								}
								if(ret.st_mode & S_IXOTH){
									strcpy(othersExecuteFile,"x");
								}
								if(isFile(files->d_name)==0)strcpy(file,"d");
								printf("%s%s%s%s%s%s%s%s%s%c ",file,userReadFile,userWriteFile,userExecuteFile,groupReadFile,groupWriteFile,groupExecuteFile,othersReadFile,othersWriteFile,othersExecuteFile[0]);
								stat(files->d_name,&buf);
								printf("%ld %s %s %ld %.16s %s\n",buf.st_nlink,pw->pw_name,grp->gr_name,buf.st_size,ctime(&buf.st_mtime),files->d_name);
							}
						}
					}
					else printf("ls: %s: No such file or directory\n",k[1]);

				}
			}
			else if(ll==1 && strcmp(k[0],"ls")==0){
				getcwd(pathname,sizeof(pathname));
				dir=opendir(pathname);
				while(files=readdir(dir))
					if(files->d_name[0]!='.')printf("%s  ",files->d_name);
				printf("\n");
				closedir(dir);
			}
		}	
	}
	if(op){
		if(childPid==0){
			close(fd0);
			fclose(stdout);
			exit(1);
		}
		else{
			wait(NULL);
		}
	}
	return;
}
