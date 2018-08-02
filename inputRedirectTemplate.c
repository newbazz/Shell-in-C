int S=0;
		char* ASepCommands[10000];
		token=strtok(allCommands[i],"<");	
		while(token){
			ASepCommands[S++]=token;
			token=strtok(NULL,"<");
		}
		char* currentCommand = (char*) malloc(sizeof(char)*1024);
		trim(currentCommand, 1024, ASepCommands[0]);
		if(strcmp(ASepCommands[0],"sort ")){
			removeSpaces(ASepCommands[1]);
			char *input =ASepCommands[1];
			char * argv1[] = { "sort", input, NULL };
			int pid=fork();
			if(!pid){
				execvp(argv1[0],argv1);
			}
		}
		free(currentCommand);