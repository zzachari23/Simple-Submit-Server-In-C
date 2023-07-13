/*Data strucutre: doubly linked list*/
typedef struct node {

  /*Variable to hold the command*/
  char *data;
  /*Points to the next node*/
  struct node *right_node;
  /*Points to the previous node*/
  struct node *left_node;

} Node;


typedef struct {

  /*Counts the number of compilation commands*/
  unsigned int count_list1;
  /*Counts the number of test commands*/
  unsigned int count_list2;

  /*Head and tail pointers for list 1*/
  Node *head1;
  Node *tail1;
  /*Head and tail pointers for list 2*/
  Node *head2;
  Node *tail2;

} Commands;

