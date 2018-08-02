The following commands have been implemented:

  ==> cd:
  Filename: cd-command.c
	In this we have implemented all the possible flags of cd like "cd -", "cd", "cd ~", "cd .." where:
		--> cd -  : Returns to the previous directory the user was in, or displays an error message if the previous 				directory was not set
		--> cd ~  : Takes user to the home directory
		--> cd    : Takes user to the home directory
		--> cd .. : Takes the user to the parent directory of the present working directory
		--> cd .  : Keeps you in the present working directory only

	'cd'ing to a directory which does not exist prints an appropriate error message.

  ==> pwd:
  Filename: pwd-command.c
	Prints the absolute path of the present working directory

  ==> echo:
  Filename: echo-command.c
	It works just like its' bash equivalent. 
	For example:     	 'echo x' prints x
					But, 'echo $x' prints the value of x variable, if assigned. Otherwise, it just prints an empty line.
	A few use cases for echo:
		--> echo "Hello World" :  It prints Hello World to stdout.
		--> echo  a 		   :  It prints a to stdout.
		--> echo $a 		   :  It prints the value of a if assigned, otherwise prints an empty line.
		--> echo "i'm john"	   :  It prints i'm john to stdout.
  
  ==> Setting variables:
    Implemented in Shell.c
  	Variables can be set the same way they are set in bash. They can be set, like so:
  		--> a=2   : Sets the value of variable a to 2 		
  		--> a =2  : Returns an error, saying that "a" command not found
  		--> a = 2 : Returns an error, saying that "a" command not found
  		--> a= 2  : Returns an error, saying that "a=" command not found

  	Once the variable is set as described in point 1, 'echo $a' will print the value of a (2, in this case) to stdout. Now a can be reassigned to some other value. For instance, doing 'a=5' and then doing 'echo $a' will print 5 to stdout.

  ==>pinfo:
  Filename: pinfo-command.c
	It will give the info of the process related to the shell of program.
	It will give info like:
		-->Process Status
		-->Virtual Memory
		-->Executable Path
	==>pinfo <pid>: It will give the above info about the process having the given pid.

  ==>ls:
  Filename: ls-command.c
	ls lists all the files and directories present in the specified directory.
	In this we have implemented all the possible flags of ls like "ls", "ls -a", "ls -al", "ls -l" where:
        --> ls 	   :  Returns all the files and folders in the pwd.
        --> ls -a  :  It prints all the files and folder including the hidden files in the pwd.
        --> ls -l  :  Prints all the files and folder in long listing form exluding the hidden ones.
        --> ls -al :  It prints all the files including the hidden ones in long listed form.

  ==> miscellaneous:
    Implemented in Shell.c
  	In addition, commands like emacs, vi, firefox, gedit etc also work.
  	For example: 'emacs hello.txt' opens a file named 'hello.txt' in emacs.
  	Invalid commands will return an appropriate error message.

  ==> Foreground and Background Prcesses:
      Implemented in Shell.c
	    The above processes can be run in foreground as well as background. To run a process, in background, simply suffix the command with '&'.
	    This has been implemented by forking to create a child process which runs this process and prints a message on completion just like it would in bash. The parent process and the child process run separately.

  ==> Multiple commands:
      Implemented in Shell.c
  	  Multiple commands can be given in the same line by semicolon separating them. Some of the commands can be made to run in background by suffixing them with '&'.

  BONUS:
  	  
  1. The process displays a message on completion after being run in background.

  2. nightswatch:

  	 ==> interrupts:
       Filename: interrupts-command.c
  	 	 It prints the number of times the CPU(s) has(ve) been interrupted by the keyboard controller once in every time interval that was specified using -n. Can be exited by pressing Ctrl-C.
  	 	 For instance: 'nightswatch -n 5 interrupt' prints the above output to stdout once every five seconds.

  	 ==> dirty:
       Filename: dirty-command.c
  	 	 It prints the size of the part of the memory which is dirty. Can be exited by pressing Ctrl-C.
  	 	 For instance: 'nightswatch -n 3 dirty' prints the above output to stdout once every three seconds.

  Naming Conventions and Modularity:
  
  ==> All the commands have been implemented in separate files, and the command x would be in the file x-command.c
  ==> Miscellaneous actions, like setting variables, background processes, opening applications like emacs, firefox etc have been implemented in Shell.c.
  ==> The files consist of functions, which are called as per the command. For example, 'cd-command.c' contains the function cd, which is called when the user types the cd command.
  ==> The global variables and all the external libraries have been included in 'global.c'.

 Made by:

 	Vivek Iyer   20161188	
  Krutam Hathi 20161012
