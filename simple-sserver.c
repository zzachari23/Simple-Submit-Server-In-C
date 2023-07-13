#include "simple-sserver.h"
#include "split.h"
#include "safe-fork.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*Name: Zachariah Zachariah
 *ID: zzachari
 *UID: 177656893
 *Section: 0106

 *
 *Program Overview: The program will be simulating a small part of
 *the CS submit server. The program will run some commands to 
 *compile some code and make them into exectuables. While another
 *part of the program will test compiled student code. The program
 *will have four functions and the descriptions of these
 *functions will be commented right before where the function 
 *starts. The program will also implement a data structure called
 *commands in which in this particular program, the data structure
 *will be a doubly linked list. The data structure will be used
 *to hold the commands read in by the program to be later on used
 *by the functions in the program. 
 */


/*List of helper functions*/
static void list1(Commands *lists, char command_data[]);
static void list2(Commands *lists, char command_data[]);
static void erase1(Commands *lists);
static void erase2(Commands *lists);
static void input(char **array);
static void output(char **array);
 
/*Function overview: The function read_commands will read in 
commands from two files that contain a list of compilation 
commands and test commnads. While the function reads in each 
of the commands it will store them in the data strucutre's list
either in list 1 for the compilation commands or in test 2 for 
the test commands. This is acomplished with the helper functions
list1 and list2 which help add the commands to list 1 and list 2.
The function will return a Command variable called lists 
containing both the lists. However, if the paramters are NULL 
or if the name of the file doesn't exit  or there is an error
when trying to open either files for reading, the program
should quit with the exit status of 1 without doing anything else 
 */
Commands read_commands(const char compile_cmds[], const char 
                       test_cmds[]) {


  /*Command variable data structure*/
  Commands lists;
  /*Place holder for the command value with the maximum limit of
    characters the array can hold*/
  char command_data[LINE_MAX]; 
  /*Opens the two files for reading*/
  FILE *file1 = fopen(compile_cmds, "r");
  FILE *file2 = fopen(test_cmds, "r");

  /*Checks whether the parameters are NULL and if not the commands
    will be retrived and be added to the lists*/
  if (compile_cmds != NULL && test_cmds != NULL && file1 != NULL 
      && file2 != NULL) {
      
    /*Initializes the fields of list 1*/
      lists.count_list1 = 0;
      lists.head1 = NULL;
      lists.tail1 = NULL;
      
      /*Gets each compilation command and then adds it to list 1*/
      while (fgets(command_data, 255, file1) != NULL) {

	  list1(&lists, command_data); 

      }

      /*Closes file1*/
      fclose(file1);

      /*Initializes the fields of list 2 */
      lists.count_list2 = 0;
      lists.head2 = NULL;
      lists.tail2 = NULL;

      
      /*Gets each test command and then adds it to list 2*/
      while (fgets(command_data, 255, file2) != NULL) {

	  list2(&lists, command_data);

      }

      /*Closes file2*/
      fclose(file2);

     

 } else {
    /*If any of the parameters are NULL, program exits with exit
      status 1*/
      exit(1);

   }
 
  return lists;

}



/*Function overview: list1 is a helper function which basically
 adds the command's it receives to the list 1 doubly linked 
 data structure of Command. The function als allocates memory 
 to store the command in it's data field. The function does not
 return anything. */
static void list1(Commands *lists, char command_data[]) {

  /*Creates a new node*/
  Node *new_node = NULL;

  /*Allocates memory for the new node*/
  new_node = malloc(sizeof(Node));
  
  /*Allocates memory for the data of the new node*/
  new_node -> data = malloc(strlen(command_data) + 1);

  /*Copies the command into the node's data field*/
  strcpy(new_node -> data, command_data);

  /*Initializes the node's fields*/
  new_node -> right_node = NULL;
  new_node -> left_node = NULL;

  /*Adds node to list 1 if there is no node's present*/
  if (lists -> head1 == NULL && lists -> tail1 == NULL) {

      lists -> head1 = new_node;
      lists -> tail1 = new_node;
      (lists -> count_list1)++;
  
    /*Adds node if only one node is present in the list*/
  } else if (lists -> count_list1 == 1) {

      lists -> head1 -> right_node = new_node;
      new_node -> left_node = lists -> head1;
      lists -> tail1 = new_node;
      (lists -> count_list1)++;

    /*Adds node to end of the list*/
  } else {

      lists -> tail1 -> right_node = new_node;
      new_node -> left_node = lists -> tail1;
      lists -> tail1 = new_node;
      (lists -> count_list1)++;

    }

      					    
}

/*Function overview: This is a helper function that is exactly
the same as helper function list1 but the only difference is that
the function adds the commands to list 2*/
static void list2(Commands *lists, char command_data[]) {

  Node *new_node = NULL;

  new_node = malloc(sizeof(Node));
  
  new_node -> data = malloc(strlen(command_data) + 1);

  strcpy(new_node -> data, command_data);

  new_node -> right_node = NULL;
  new_node -> left_node = NULL;

  if (lists -> head2 == NULL && lists -> tail2 == NULL) {

      lists -> head2 = new_node;
      lists -> tail2 = new_node;
      (lists -> count_list2)++;

  } else if (lists -> count_list2 == 1) {

      lists -> head2 -> right_node = new_node;
      new_node -> left_node = lists -> head2;
      lists -> tail2 = new_node;
      (lists -> count_list2)++;

 } else {

      lists -> tail2 -> right_node = new_node;
      new_node -> left_node = lists -> tail2;
      lists -> tail2 = new_node;
      (lists -> count_list2)++;

   }

      					    
}


/*Function overview: The function clear_commands deallocates
all memory that has been dynamically-allocated and it does this
with the help of two helper functions called erase1 and erase2.
If the parameter is NULL the function should return without 
doing anything and if the parameter contains no elements the
functio should not do anything.*/
void clear_commands(Commands *const commands) {

  /*Checks if the paramter command is NULL or not*/
  if (commands != NULL) {

    /*Goes through list 1 and dellocates all the memory in it*/
    while (commands -> count_list1 != 0) {

          /*Calls helper function*/
	  erase1(commands);

    }
  
    /*Goes through list 2 and dellocates all the memory in it*/
    while (commands -> count_list2 != 0) {

          /*Calls helper function*/
	  erase2(commands);

    }


  }      
  


}

/*Function overview: The helper function erase1 dellocates
all dynamically-allocated memory used in list of Commands lists 
variable. It goes through each node and frees the node's data
and itself until the list is completely empty. In addition,
the function does not return anything.*/
static void erase1(Commands *lists) { 

  /*Creates temporary node*/
  Node *temp = NULL;

  /*Checks if the list contains only one element*/
  if (lists -> count_list1 == 1) {

      /*Free's the head node's data*/
      free(lists -> head1 -> data);
      /*Free's the head node itself*/
      free(lists -> head1);
      /*Initializes the head and tail pointer of the list to 
	NULL*/
      lists -> head1 = NULL;
      lists -> tail1 = NULL;
      /*Decrements the number of elements in list1*/
      (lists -> count_list1)--;

    /*Checks if the list contains more than one element*/
  } else if (lists -> count_list1 > 1) {
    
      /*Rearranges the node's before a node is freed*/
      temp = lists -> head1;
      temp -> right_node -> left_node = temp -> left_node;
      lists -> head1 = temp -> right_node;
      /*Free's the node's data*/
      free(temp -> data);
      /*Free's the node itself*/
      free(temp);
      temp = NULL;
      /*Decrements the number of elements in list1*/
      (lists -> count_list1)--;

    }


}

/*Function overview: The helper function erase2 is exactly the 
same as helper function erase1 but the only difference is that
this function deallocates all dynamically-allocated memory for 
list 2 of the Commands variable lists*/
static void erase2(Commands *lists) { 


  Node *temp = NULL;

  if (lists -> count_list2 == 1) {

      free(lists -> head2 -> data);
      free(lists -> head2);
      lists -> head2 = NULL;
      lists -> tail2 = NULL;
      (lists -> count_list2)--;

  } else if (lists -> count_list2 > 1) {

      temp = lists -> head2;
      temp -> right_node -> left_node = temp -> left_node;
      lists -> head2 = temp -> right_node;
      free(temp -> data);
      free(temp);
      temp = NULL;
      (lists -> count_list2)--;

    }


}

/*Function overview: The function compiles the compilation 
commands stored in Commands variable's commands list1. The 
function will create a child process using the safe_fork() 
function for each command it executes. The commands will have
to executed from first to last and if the commands all execute
successfully they will exit with an exit status of 0 and will
return SUCCESSFUL_COMPILATION which is a symbolic constant
defined in simple-sserver.h. While executing if any of the 
commands have errors the program should stop executing commands
and return FAILED_COMPILATION which is another symbolic constant.
If list1 has no commands then the function will return
SUCCESSFUL_COMPILATION without executing any commands. The 
function also uses the split() function that will split a
command into individual strings and then add each of the 
strings to a double pointer char array. The function also
handles commands with input and output redirection.*/
int compile_program(Commands commands) {

  /*Temporary node pointer*/
  Node *temp;
  int i, status, stop = 0;
  

  /*Checks if list 1 is empty or not*/ 	
  if (commands.count_list1 != 0) {

    /*Pointer points to the head of list 1*/
      temp = commands.head1;
      
      /*Loops through each occurence of a command in list 1*/
      for (i = 0; i < commands.count_list1 && stop != 1; i++) {
	
          /*Creates a child process*/
          if (safe_fork() == 0) {
	    
	      /*Splits each command into individual strings*/
	      char **array = split(temp -> data);
	      int counter = 0;
	      
	      /*Counts how elements are in the array*/
	      while (array[counter] != NULL) {

		  counter++;

		}
	      
	      /*Executes compilation command with two strings*/
	      if (counter == 2) {

		/*Executes program*/
		  execlp(array[0], array[0], array[1], NULL);
		  /*Free's array*/
		  free(array);

	    /*Executes compilation command with three strings*/   
	      } else if (counter == 3) {

		/*Executes input redirection*/
		  if (strcmp(array[1], "<") == 0) {

		      /*Calls helper function for input 
			redirection*/
		      input(array);

		      /*Executes output redirection*/  
		  } else if (strcmp(array[1], ">") == 0) {

		    /*Calls helper function for output
		      redirection*/
		      output(array);

		    }		   
  
        /*Executes compilation command with four strings*/		     
                  } else if (counter == 4) {
		
		/*Executes program*/
		  execlp(array[0], array[0], array[1], array[2], 
                         array[3], NULL);
		  
		  /*Free's array*/
		  free(array);

        /*Executes compilation command with five strings*/
	      }  else if (counter == 5 && strcmp(array[3], ">") != 0) {
		    
		/*Exeutes program*/
	          execlp(array[0], array[0], array[1], array[2],
                         array[3], array[4], NULL);
		  /*Free's array*/
		  free(array);
		 	  	
		  /*Executes command with both input and output redirection*/
	      }  else if (counter == 5 && strcmp(array[3], ">") == 0) {

		  /*File descriptors*/
		  int fd0, fd1;
		  /*Allocates memory to char pointers */
		  char *infile = malloc(strlen(array[2]) + 1);
		  char *outfile = malloc(strlen(array[4]) + 1);
		     
		  /*Gets input file*/
		  strcpy(infile, array[2]);
		  
		  /*Removes "<" and input file from array*/
		  free(array[1]);
		  free(array[2]);
		  
		  /*Opens file for read only*/
		  fd0 = open(infile, O_RDONLY, 0);
		  /*Redirects standard input*/
		  dup2(fd0, STDIN_FILENO);
		  /*Closes file discriptor*/
		  close(fd0);
		  /*Free's allocated memory for infile*/
		  free(infile);
		  
		  /*Gets outputfile*/
		  strcpy(outfile, array[4]);
		  
		  /*Removes ">" and output file from array*/
		  free(array[3]);
		  free(array[4]);
		      		      
		  /*Opens file for writing*/
		  fd1 = open(outfile, FILE_FLAGS, FILE_MODE);
		  /*Redirects standard output*/
		  dup2(fd1, STDOUT_FILENO);
		  /*Closes file discriptor*/
		  close(fd1);
		  
		  /*Inovke's program*/
		  execlp(array[0], array[0], NULL);
		  /*Free's array*/
		  free(array);
		  /*Free's memory allocated to file*/
		  free(outfile);


		} 
         }
	
	 
	  /*Parent process waits for child process to finish*/
	      wait(&status);
	      
	      /*Checks child's exit status*/
	      if (WIFEXITED(status)) {

		  int exit_status = WEXITSTATUS(status);
		  
		  /*If exit status is good move onto the 
		    next command and continue executing*/
		  if (exit_status == 0) {

		    /*Moves onto the next command*/
		      temp = temp -> right_node;

		  } else {
		    
		    /*Stops loop if something went wrong
		      with the exit status of the child*/
		      stop = 1;

		    }

	     } else {

		/*Stops loof if WIFEXITED status if false*/
		  stop = 1;

	      }
	    
	    
	}

      /*Means something went wrong with executing the commands
	so returns error status*/
      if (stop == 1) {

	  return FAILED_COMPILATION;

      } else {

	/*Returns this status if all the commands were executed
	  successfully*/
	return SUCCESSFUL_COMPILATION;

        }


      /*Returns this status if the list was empty*/
  } else {

      return SUCCESSFUL_COMPILATION;

    }



}


/*Function overview: The function executes all the test commands
that are in list 2 of Commands variable commands. The function
is almost exactly like the function compile_program but the only
exception is that this function continues executing commands 
even if one of the commands being executed fails. The program 
then counts how many succesfull commands it has executed and 
then returns that count. In addition, the function will only
completely execute if a call to compile_program returns
successful.*/ 
int test_program(Commands commands) {

  /*Creates temporary pointer*/
  Node *temp;
  int i, status, count_passed = 0;

  /*Checks if the return value of compile_program is 
    successful or not and whether or not the list empty*/
  if (commands.count_list2 != 0) {

      /*Points to the head of list2*/
       temp = commands.head2; 

       /*Loops through the list*/
      for (i = 0; i < commands.count_list2; i++) {
	  
	  /*Creates child process*/
	  if (safe_fork() == 0) {
	   
	    /*Splits the command into individual strings*/
	      char **array = split(temp -> data);
	      int counter = 0;

	      /*Counts the number of elements in the array*/
	      while (array[counter] != NULL) {

		  counter++; 

	      }
	      
	      /*Executes one test command*/
	      if (counter == 1) {

		  /*Invokes program*/
		  execlp(array[0], array[0], NULL);
		  /*Free's array*/
		  free(array);

	      /*Executes commands with input and output 
		redirection*/
	      } else if(counter == 3) {
   
		/*Executes input redirection*/
		  if (strcmp(array[1], "<") == 0) {

		    /*Executes input redirection*/
		      input(array);

		      /*Executes output redirection*/
		  } else if(strcmp(array[1], ">") == 0) {

		    /*Executes output redirection*/ 
		      output(array);

		    }
		     
	       /*Executes both input and output redirection*/
	      } else if(counter == 5) {

		  /*File descriptors*/
		  int fd0, fd1;
		  /*Allocates memory to char pointers */
		  char *infile = malloc(strlen(array[2]) + 1);
		  char *outfile = malloc(strlen(array[4]) + 1);
		     
		  /*Gets input file*/
		  strcpy(infile, array[2]);
		  
		  /*Removes "<" and input file from array*/
		  free(array[1]);
		  free(array[2]);
		  
		  /*Opens file for read only*/
		  fd0 = open(infile, O_RDONLY, 0);
		  /*Redirects standard input*/
		  dup2(fd0, STDIN_FILENO);
		  /*Closes file discriptor*/
		  close(fd0);
		  /*Free's allocated memory for infile*/
		  free(infile);
		  
		  /*Gets outputfile*/
		  strcpy(outfile, array[4]);
		  
		  /*Removes ">" and output file from array*/
		  free(array[3]);
		  free(array[4]);
		      		      
		  /*Opens file for writing*/
		  fd1 = open(outfile, FILE_FLAGS, FILE_MODE);
		  /*Redirects standard output*/
		  dup2(fd1, STDOUT_FILENO);
		  /*Closes file discriptor*/
		  close(fd1);
		  
		  /*Inovke's program*/
		  execlp(array[0], array[0], NULL);
		  /*Free's array*/
		  free(array);
		  /*Free's memory allocated to file*/
		  free(outfile);
		  

		}


	    }
	  
	  /*Parent waits on child*/
	  wait (&status);

	  /*Check's child's exit status*/
	  if (WIFEXITED(status)) {

	      int exit_status = WEXITSTATUS(status);
	      
	      /*If the child process executed successfully
		then increment the number of commands passed*/
	      if (exit_status == 0) {

		  count_passed++;

	      }

	  }

	  /*Move onto the next command in the list*/
	  temp = temp -> right_node;

       }



    }

    

  /*Returns the number of commans executed successfully*/
  return count_passed;


}


/*Function overview: This function is a helper function that is
used to execute input redirection when called upon. The function
does not return anything.*/
static void input(char **array) {

  /*File discriptor*/
  int fd0;
  /*Allocates memory for char pointer*/
  char *infile = malloc(strlen(array[2]) + 1);
		   
  /*Get's inputfile*/  
  strcpy(infile, array[2]);

  /*Removes "<" and input file from the array*/
  free(array[1]);
  free(array[2]);
		   
  /*Opens file for reading*/
  fd0 = open(infile, O_RDONLY, 0);
  /*Redirects standard input*/
  dup2(fd0, STDIN_FILENO);
  /*Closes file discriptor*/
  close(fd0);

  /*Invokes program*/
  execlp(array[0], array[0], NULL);
  /*Free's array*/
  free(array);
  /*Free's memory allocated to file*/
  free(infile);


}

/*Function overview: This is a helper function that executes
output redirection. The function does not return anything.*/
static void output(char **array) {

  /*File discriptor*/
  int fd1;
  /*Allocates memory for char pointer*/
  char *outfile = malloc(strlen(array[2]) + 1);

  /*Get's file name*/		     
  strcpy(outfile, array[2]);
 
  /*Removes ">" and output file from array*/
  free(array[1]);
  free(array[2]);
		      
  /*Open's file for writing*/		      
  fd1 = open(outfile, FILE_FLAGS, FILE_MODE);
  /*Redirect's standard output*/
  dup2(fd1, STDOUT_FILENO);
  /*Close's file discriptor*/
  close(fd1);

  /*Invoke's program*/
  execlp(array[0], array[0], NULL);
  /*Free's the array*/
  free(array);
  /*Free's memory allocated to the filename*/
  free(outfile);

}
