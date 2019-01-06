/* doubly_linked_list.c
 *
 * Implement a sorted linked list of strings with operations Insert 
 * in alphabetical order, Print, Member, Delete, Free_list.
 * The list nodes are doubly linked.
 * 
 * Input:    Single character lower case letters to indicate operations, 
 *           possibly followed by value needed by operation -- e.g. 'i'
 *           followed by "hello" to insert the string "hello" -- no 
 *           double or single quotes.
 *
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o doubly_linked_list doubly_linked_list.c
 *           
 * Run:      ./doubly_linked_list
 *
 */

/*
Author: Meghana Gadiraju
Professor: Vahab
Class: CS 221 C & Systems
Due Date: December 6, 2018
*/

/* You may not add any new header file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Max 99 chars + '\0' */
#define STRING_MAX 100

struct list_node_s {
   char*  data;
   struct list_node_s* prev_p;
   struct list_node_s* next_p;
};

/* Pointers to the head and tail of the list */
struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

void Insert(struct list_s* list_p, char string[]);
void Print(struct list_s* list_p);
int  Member(struct list_s* list_p, char string[]);
void Delete(struct list_s* list_p, char string[]);
void Free_list(struct list_s* list_p);
char Get_command(void);
void Get_string(char string[]);
void Free_node(struct list_node_s* node_p);
struct list_node_s* Allocate_node(int size);
void Print_node(char title[], struct list_node_s* node_p);


/*-----------------------------------------------------------------*/
int main(void) {
   char          command;
   char          string[STRING_MAX];
   struct list_s list ;  

   list.h_p = list.t_p = NULL;
      /* start with empty list */

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
            Get_string(string);
            Insert(&list, string);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm': 
         case 'M':
            Get_string(string);
            if (Member(&list, string))
               printf("%s is in the list\n", string);
            else
               printf("%s is not in the list\n", string);
            break;
         case 'd':
         case 'D':
            Get_string(string);
            Delete(&list, string);
            break;
         case 'f':
         case 'F':
            Free_list(&list);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }
   Free_list(&list);

   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:   Allocate_node
 * Purpose:    Allocate storage for a list node
 * Input arg:  size = number of chars needed in data member (including
 *                storage for the terminating null)
 * Return val: Pointer to the new node
 */

struct list_node_s* Allocate_node(int size) 
{
   struct list_node_s* new_node = (struct list_node_s*)malloc(sizeof(struct list_node_s));//allocating space for new node
   if(new_node == NULL)//sending error if no space left to create new node
   {
      printf("No memory left in heap, delete items in list to add more.");
      return NULL;
   }
   else//creates new node
   {
      new_node->data = (char*)malloc((size+1)*sizeof(char));
      printf("New item added to list\n");
      return new_node;
   }
} /* Allocate */

/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in correct alphabetical location in list
 * Input arg:  string = new string to be added to list
 * In/out arg: list_p = pointer to struct storing head and tail ptrs
 * Note:       If the string is already in the list, print a message
 *                and return, leaving list unchanged
 */

   void Insert(struct list_s* list_p, char string[])
   {

      if(Member(list_p, string) == 1)//checking if string is already in list
      {
         printf("This item is already in the list: '%s'\n",string);
         return;
      }
      if (Member(list_p, string) == 0)
      {
         struct list_node_s* new_node = Allocate_node(strlen(string));//creating a new node
         strcpy(new_node->data, string);//copying strings 
         struct list_node_s* curr_node = list_p->h_p;//will use to insert in alphabetical order

         if(curr_node == NULL)//checks for an empty list
         {
            list_p->h_p = new_node;
            new_node->prev_p = NULL;
            new_node->next_p = NULL;
         }
         else
         {
            while(curr_node->next_p != NULL && strcmp(new_node->data, curr_node->data) > 0)//increments current node pointer until it gets to the tail or is in the location to be placed
            {
               curr_node = curr_node->next_p;
            }

            if(curr_node == list_p->h_p)
            {
               if(strcmp(new_node->data, curr_node->data) > 0)//checks if node is inserted at head or if there is only 1 other node in the list and inserts node at the head. 
               {
                  new_node->prev_p = curr_node;
                  curr_node->next_p = new_node;
                  list_p->t_p = new_node;        
               }
               else //insert node before head
               {  
                  new_node->next_p = curr_node;
                  curr_node->prev_p = new_node;
                  list_p->h_p = new_node; 
               } 
            }
            else
            {
               
               if(strcmp(new_node->data, curr_node->data) > 0)
               {
                  //Insert after tail
                  curr_node->next_p = new_node;
                  new_node->prev_p = curr_node;

                  if(curr_node == list_p->t_p)
                  {
                     list_p->t_p = new_node;
                  }
               }
               else //Insert before current node/tail
               {
                  
                  curr_node->prev_p->next_p = new_node;
                  new_node->prev_p = curr_node->prev_p;
                  new_node->next_p = curr_node;
                  curr_node->prev_p = new_node; 
               }
            }
         }
      }
   }  /* Insert */


/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 *
 * Hint: The implementation of this function (below) shows how
 * to traverse a linked list.
 */
void Print(struct list_s* list_p) 
{
   struct list_node_s* curr_node = list_p->h_p;

   printf("list = ");

   while (curr_node != NULL) 
   {
      printf("%s ", curr_node->data);
      curr_node = curr_node->next_p;
   }
   printf("\n");
}  /* Print */

/*-----------------------------------------------------------------*/
/* Function:   Member
 * Purpose:    Search list for string
 * Input args: string = string to search for
 *             list_p = pointers to first and last nodes in list
 * Return val: 1, if string is in the list, 0 otherwise
 */
int  Member(struct list_s* list_p, char string[]) 
{

  //TODO: Complete me
   char* data;
   struct list_node_s* curr_node = list_p -> h_p;//the list

   while (curr_node != NULL)
   {
      //check alphabetically down the list until you find position
      data = curr_node->data;
   
      if (data[0] == string[0])//makes sure to only check the words that start with the same letter
      {
         if (strcmp(data,string) == 0)//compares strings
         {
            return 1;
         }
         curr_node = curr_node -> next_p;
      }
      else if (data [0] > string [0])
      {
         return 0;
      }
      curr_node = curr_node -> next_p;//increment
   }
   return 0;
} /* Member */

/*-----------------------------------------------------------------*/
/* Function:   Free_node
 * Purpose:    Free storage used by a node of the list
 * In/out arg: node_p = pointer to node to be freed
 */
void Free_node(struct list_node_s* node_p) 
{
   free(node_p->data);
   free(node_p);
}  /* Free_node */


/*-----------------------------------------------------------------*/
/* Function:   Delete
 * Purpose:    Delete node containing string.
 * Input arg:  string = string to be deleted
 * In/out arg  list_p = pointers to head and tail of list
 * Note:       If the string is in the list, it will be unique.  So
 *             at most one node will be deleted.  If the string isn't
 *             in the list, the function just prints a message and
 *             returns, leaving the list unchanged.
 */

   //TODO: Complete me
void Delete(struct list_s* list_p, char string[]) 
{

   if (list_p->t_p == NULL && list_p->h_p == NULL)//check for an empty list
   {
      printf ("The list is empty.\n");
      return;

   }
   else if(!Member(list_p, string))//check for string
   {
      printf("The string is not in the List.\n");
      return;
   }
   else if (Member (list_p, string))
   { 
      struct list_node_s* curr_node = list_p->h_p;

      while (curr_node != NULL) 
      {
         if(strcmp(curr_node->data,string) == 0)
         {

            if(curr_node->prev_p == NULL && curr_node->next_p == NULL)//check if there is only 1 element
            {
               list_p->h_p = NULL;
               list_p->t_p = NULL;
            }
            else if(curr_node == list_p->h_p)
            {
               list_p->h_p = curr_node->next_p; 
            }
            else if(curr_node == list_p->t_p)
            {   
               list_p->t_p = curr_node->prev_p;
            }

            curr_node->prev_p->next_p = curr_node->next_p;
            curr_node->next_p->prev_p = curr_node->prev_p;

            Free_node(curr_node);
            printf("Deleted string: '%s' from list.\n",string);
         }
         curr_node = curr_node->next_p;
      }
   }

}  /* Delete */

/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free storage used by list
 * In/out arg: list_p = pointers to head and tail of list
 */
void Free_list(struct list_s* list_p) 
{
   //TODO: Complete me
   struct list_node_s* curr_node = list_p -> h_p;
   while (curr_node)
   {
      struct list_node_s* next = curr_node -> next_p;
      Free_node(curr_node);
      curr_node = next;
   }
   list_p -> h_p = NULL;
   list_p -> t_p = NULL;
   printf("List Storage Freed\n");

}  /* Free_list */



/*-----------------------------------------------------------------*/
/* Function:   Get_command
 * Purpose:    Find and return the next non-whitespace character in
 *             the input stream
 * Return val: The next non-whitespace character in stdin
 */
char Get_command(void) 
{
   char c;

   printf("Please enter a command (i, d, m, p, f, q):  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  /* Get_command */

/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin (delimited by whitespace)
 * Out arg:    string = next string in stdin
 */
void Get_string(char string[]) 
{

   printf("Please enter a string:  ");
   scanf("%s", string);
}  /* Get_string */


/*-----------------------------------------------------------------*/
/* Function:  Print_node
 * Purpose:   Print the data member in a node or NULL if the
 *            pointer is NULL
 * In args:   title:  name of the node
 *            node_p:  pointer to node
 */
void Print_node(char title[], struct list_node_s* node_p) 
{
   printf("%s = ", title);
   if (node_p != NULL)
      printf("%s\n", node_p->data);
   else
      printf("NULL\n");
}  /* Print_node */

