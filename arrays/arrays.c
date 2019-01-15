#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
    Array *new_array = malloc(sizeof(Array));
  // Set initial values for capacity and count
    new_array->capacity = capacity;
    new_array->count = 0;
  // Allocate memory for elements
    new_array->elements = calloc(capacity, sizeof(char *));

    // new_array->elements = malloc(sizeof(20 * capacity)); //each string can be no more than 20 characters and we have the capacity of the array
    // for (int i = 0; i<capacity; i++ )
    // {
    //   new_array->elements[i] = malloc(50 * sizeof(char));
    // }

    return new_array;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {

  // Free all elements
  for (int i = 0; i < arr->count; i++) // WHy not <=?
  {
    free(arr->elements[i]);
  }
  
  // Free array
  free(arr->elements);
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  char **new_array_storage = calloc(2 * arr->capacity, sizeof(char *));

  // new_array_storage = malloc(sizeof(char*) * arr->capacity * 2);
  
  
  // Copy elements into the new storage
  for (int i = 0; i < arr->count; i++)
  {
    new_array_storage[i] = arr->elements[i];
  }


  // memcpy(new_element_storage, arr->elements, sizeof(char));
  // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);

  // Update the elements and capacity to new values
  arr->elements = new_array_storage;
  arr->capacity = 2 * arr->capacity;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater than the current count
  if (index >= arr->count)
  {
    fprintf(stderr, "IndexError, Index %d out of range", index);
    // exit(1); Does this work?
    return NULL;
  }
  // Otherwise, return the element at the given index
  return arr->elements[index];
}


/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if (index > arr->count)
  {
    fprintf(stderr, "IndexError, Index %d out of range", index);
    // exit(1); ???
    return;
  }
  // Resize the array if the number of elements is over capacity
  
  if (arr->count >= arr->capacity)
  {
    resize_array(arr);
  }
  // Move every element after the insert index to the right one position
  for (int i = arr->count; i > index; i--)
  {
    arr->elements[i] = arr->elements[i-1];
  }
  // Copy the element and add it to the array
    // strcpy(arr->elements[index], element); Does this work?
    char *element_to_insert = strdup(element);
    arr->elements[index] = element_to_insert;
  // Increment count by 1
  arr->count++; //Dk if I can use this syntax to increment
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  if (arr->count >= arr->capacity)
  {
    resize_array(arr);
  }
  // or throw an error if resize isn't implemented yet.

  // Copy the element and add it to the end of the array
  char *element_to_append = strdup(element);
  arr->elements[arr->count] = element_to_append;
  // Increment count by 1
  arr->count++;

  // Could also have done arr_insert(arr, element, arr->count);
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  // Search for the first occurence of the element and remove it.
  int counter = 0;
  if (arr->count == 0)
  {
    fprintf(stderr, "ValueError, attempting to pop from empty list\n");
    return;
  }
  while (strcmp(element, arr->elements[counter]) != 0)
  {
    if (counter >= arr->count)
    {
      fprintf(stderr, "ValueError, value %s not in array", element);
      return;
    }

    counter++;
  }


  // Don't forget to free its memory!
  free(arr->elements[counter]);
  // Shift over every element after the removed element to the left one position
  for (int i = counter; i < arr->count; i++)
  {
    arr->elements[i] = arr->elements[i+1];
  }
  // Decrement count by 1
  arr->count--;
  arr->elements[arr->count] = NULL;
}

char *arr_pop(Array *arr)
{
  if (arr->count == 0)
  {
    printf("Array Empty\n");
    return NULL;
  }

  char *rv = arr->elements[arr->count-1];
  free(arr->elements[arr->count-1]);
  arr->count--;

  return rv;
}

void arr_clear(Array *arr)
{
  for (int i = 0; i < arr->count; i++)
  {
    free(arr->elements[i]);
  }

  arr->count = 0;
  arr->capacity = 10;
  arr->elements = calloc(arr->capacity, sizeof(char *));
}

Array *arr_copy(Array *arr)
{
  Array *array_copy = create_array(arr->capacity);

  for (int i=0; i<arr->count; i++)
  {
    // array_copy->elements[i] = arr->elements[i]; Why doesn't this work?
    arr_append(array_copy, strdup(arr->elements[i]));
  }

  return array_copy;
}

void arr_extend(Array *arr1, Array *arr2)
{
  for (int i = 0; i < arr2->count; i++)
  {
    arr_append(arr1, arr2->elements[i]);
  }
}

int arr_index(Array *arr, char *element)
{
  for (int i = 0; i < arr->count; i++)
  {
    if (strcmp(arr->elements[i], element) == 0)
    {
      return i;
    }
  }

  return -1;
}

void arr_reverse(Array *arr)
{
  char **buffer = calloc(arr->capacity, sizeof(char *));
  int counter = 0;

  for (int i = arr->count-1; i >= 0; i--)
  {
    char* add_next = strdup(arr->elements[i]);
    buffer[counter] = add_next;
    counter++;
  }

  arr->elements = buffer; // Do I need a call to free somewhere in here? Maybe at the top of the for loop
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  printf("POP RETURNS: %s\n", arr_pop(arr));
  arr_clear(arr);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);

  Array *copy = arr_copy(arr);
  arr_print(arr);
  printf("Array copy...\n");
  arr_print(copy);

  Array *extended = create_array(10);
  arr_append(extended, "STRING88");
  arr_append(extended, "STRING99");
  arr_append(extended, "STRING0");
  arr_append(extended, "STRING34");
  arr_print(extended);
  arr_extend(extended, copy);
  arr_print(extended);
  printf("Array Index: %i\n", arr_index(extended, "STRING34"));
  arr_reverse(extended);
  arr_print(extended);


  destroy_array(arr);

  return 0;
}
#endif 
