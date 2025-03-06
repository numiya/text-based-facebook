/********* definitions.c ********
    Student Name 	= Kaitlyn Bui
    Student Number	= 101273818
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"

/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, int userid, const char *password){
   user_t *new_u = malloc(sizeof(user_t));
   assert(new_u != NULL);
   strcpy(new_u->username, username);
   new_u->userid = userid;
   strcpy(new_u->password, password);
   new_u->friends = NULL;
   new_u->posts = NULL;
   // Add to the head of the list if there are no users.
   if (users == NULL) {
      new_u->next = NULL;
      users = new_u;
      return users;
   }
   user_t * temp_p = users;
   user_t * prev_temp_p = NULL;
   // Traverse the list until the new user's id is smaller than the current index or index reaches the end
   while ((temp_p != NULL) && (new_u->userid >= temp_p->userid)){
      prev_temp_p = temp_p;
      temp_p = temp_p->next;
   }
   new_u->next = temp_p;
   // Checking if the new id is at least bigger than the first user's
   if(prev_temp_p != NULL){
      prev_temp_p->next = new_u;
   } else {
      users = new_u;
   }
   return users;
}

/*
   Function that searches if the user is available in the database 
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username){
   // Traverse the list until the username is found or it reaches the end.
   char name[31];
   strcpy(name, username);
   while(users != NULL && (strcmp(users->username, strlwr(name)) != 0)){
      users = users->next;
   }
   return users;
}


/*
   Function that creates a new friend's node.
   Return the newly created node.
*/
friend_t *create_friend(const char *username, int id){
   friend_t *new_f = malloc(sizeof(friend_t));
   assert(new_f != NULL);
   strcpy(new_f->username, username);
   new_f->id = id;
   new_f->posts = NULL;
   return new_f;
}


/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend, int id)
{
   assert(user != NULL);
   friend_t *n_friend = create_friend(friend, id);
   // If user has no friends, add to the front of the list.
   if(user->friends == NULL){
      n_friend->next = NULL;
      user->friends = n_friend;
      return;
   }
   friend_t *temp_p = user->friends;
   friend_t *prev_temp_p = NULL;
   // Travers the user's friend list until the new friend's id is smaller than the current index or index reaches the end
   while((temp_p != NULL) && (n_friend->id > temp_p->id)){
      prev_temp_p = temp_p;
      temp_p = temp_p->next;
   }
   n_friend->next = temp_p;
   // Checking if the new id is at least bigger than the first friend's
   if (prev_temp_p != NULL){
      prev_temp_p->next = n_friend;
   } else {
      user->friends = n_friend;
   }
   return;
}

/*
   Function that removes a friend from a user's friend list.
   Return true of the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name){
   assert(user != NULL);
   friend_t *searching_p = user->friends;
   friend_t *keep_prev_p = NULL;
   // If user has no friends, there are no friends to delete
   if(searching_p == NULL){
      return false;
   }
   // Traverse user's friend list until friend's username is found or pointer reaches end of list
   while((searching_p != NULL) && strcmp(searching_p->username, friend_name) != 0){
      keep_prev_p = searching_p;
      searching_p = searching_p->next;
   }
   friend_t *delete_p = searching_p;
   // Check if the username was the user's first friend in the list
   if(keep_prev_p == NULL){
      user->friends = user->friends->next;
   } else {
      keep_prev_p->next = searching_p->next;
   }
   free(delete_p);
   delete_p = NULL;
   return true;
}

/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text){
   post_t *new_pt = malloc(sizeof(post_t));
   assert(new_pt != NULL);
   strcpy(new_pt->content, text);
   return new_pt;
}

/*
   Function that adds a post to a user's timeline. New posts should be added following
   the stack convention (LIFO) (i.e., to the beginning of the Posts linked list).
*/
void add_post(user_t *user, const char *text){
   assert(user != NULL);
   post_t *n_post = create_post(text);
   n_post->next = user->posts;
   user->posts = n_post;
   return;
}

/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user, int number){
   assert(user != NULL);
   // Check if there are posts to delete
   if(user->posts == NULL){
      return false;
   }
   post_t *delete_p = user->posts;
   post_t *keep_prev_p = NULL;
   if (number == 0 || number == 1){
      user->posts = user->posts->next;
   } else {
      for(int i = 1; i < number; i++){
         keep_prev_p = delete_p;
         delete_p = delete_p->next;
      }
      keep_prev_p->next = delete_p->next;
   }
   free(delete_p);
   delete_p = NULL;
   return true;
}

/*
   Function that  displays a specific user's posts
*/
void display_user_posts(user_t *user){
   assert(user != NULL);
   post_t *holding_p = user->posts;
   printf("\n--- %s's posts: ---\n", user->username);
   // Chrck if there are posts to display
   if(holding_p == NULL){
      printf("\n*** No posts are available for %s. ***\n\n---\n", user->username);
      return;
   }
   unsigned int post_num = 1;
   // Display posts until it reaches the end of the list
   while(holding_p != NULL){
      printf("%d- %s: %s\n", post_num, user->username, holding_p->content);
      post_num++;
      holding_p = holding_p->next;
   }
   printf("---\n");
   return;
}


/*
   Function that displays a specific user's friends
*/
void display_user_friends(user_t *user){
   assert(user != NULL);
   friend_t *reference_p = user->friends;
   printf("\n--- %s's list of friends: ---\n", user->username);
   // Check if user had friends to display
   if(reference_p == NULL){
      printf("\n*** No friends available for %s. ***\n\n---\n", user->username);
      return;
   }
   // Display friends until reaches the last friend
   unsigned int num_f = 1;
   while(reference_p != NULL){
      printf("%d- %s (%d)\n", num_f, reference_p->username, reference_p->id);
      num_f++;
      reference_p = reference_p->next;
   }
   printf("---\n");
   return;
}

/*
   Function that displays "number" of posts for a given user.
   After displaying "number" posts, it prompts if you want to display
   posts of the next "number" posts.
   If there are no more post or the user types “n” or “N”, the function returns. 
*/
void display_posts(user_t *users, int number){
   assert(users != NULL);
   post_t *holder_p = users->posts;
   printf("\n--- %s's posts: ---\n", users->username);
   if(holder_p == NULL){
      printf("\n*** No posts are available for %s. ***\n\n---\n", users->username);
      return;
   }
   char cont = 'y';
   unsigned short int counter = 1;
   while((holder_p != NULL) && (cont == 'y' || cont == 'Y')){
      for (int i = 1; i <= number; i++){
         if(holder_p != NULL){
            printf("%d- %s: %s\n", counter, users->username, holder_p->content);
            holder_p = holder_p->next;
            counter++;
         } else {
            printf("\n*** All posts have been displayed for %s. ***\n\n---\n", users->username);
            return;
         }
      }
      if (holder_p == NULL){
         printf("\n*** All posts have been displayed for %s. ***\n\n---\n", users->username);
         return;
      }
      printf("Would you like to display more posts (Y/y or N/n): ");
      scanf(" %c", &cont);
   }
   printf("\n*** All posts have been displayed for %s. ***\n\n---\n", users->username);
   return;
}

/*
   Fucntion that free all users from the database before quitting the application.
*/
void teardown(user_t *users){
   assert(users != NULL);
   user_t *u_hold = users;
   post_t *post_hold = users->posts;
   friend_t *f_hold = users->friends;
   // Going through the list of users until the end and freeing all memory allocated to friends and posts and users
   while(u_hold != NULL){
      u_hold = u_hold->next;
      while(post_hold != NULL){
         post_hold = post_hold->next;
         free(users->posts);
         users->posts = post_hold;
      }
      while(f_hold != NULL){
         f_hold = f_hold->next;
         free(users->friends);
         users->friends = f_hold;
      }
      free(users);
      users = u_hold;
   }
}

/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu(){
   printf("\n************************************************\nMAIN MENU:\n************************************************\n");
   printf("1. Register a new user\n2. Login with existing user's information\n3. Exit\n\nPlease enter your choice: ");
}

/*
   Function that checks valid user input for main menu options 
*/
unsigned int main_option(){
   print_menu();
   unsigned int choice = 0;
   scanf("%d", &choice);
   while(1 > choice || choice > 3){
      printf("\nInvalid choice.\nPlease enter a choice between 1 and 3: ");
      scanf("%d", &choice);
   }
   return choice;
}

void print_user_menu(const char *name){
   printf("\n************************************************\nWelcome %s:\n************************************************\n", name);
   printf("1. Manage profile (change password)\n2. Manage posts (add/remove)\n3. Manage friends (display/add/remove)\n4. Display a friend's posts\n5. Exit\n\nPlease enter your choice: ");
}

/*
   Function that checks valid user input for user menu options
*/
unsigned int user_option(const char *name){
   print_user_menu(name);
   unsigned int choice = 0;
   scanf("%d", &choice);
   while(1 > choice || choice > 5){
      printf("\nInvalid choice.\nPlease enter a choice between 1 and 5: ");
      scanf("%d", &choice);
   }
   return choice;
}

/*
   Function that handles the menu for user posts
*/
unsigned int post_menu(user_t * user){
   printf("\n1. Add a new post\n2. Remove %s's latest post\n3. Remove one of %s's posts\n4. Return to main menu\n\nPlease enter your choice: ", user->username, user->username);
   unsigned int choose;
   scanf("%d", &choose);
   while(1 > choose || choose > 4){
      printf("Invalid choice.\n1. Add a new post\n2. Remove %s's latest post\n3. Remove one of %s's posts\n4. Return to main menu\n\nPlease enter a value between 1 and 4: ", user->username, user->username);
      scanf("%d", &choose);
   }
   return choose;
}

/*
   Function that carries out options from the user post menu
*/
void post_process(user_t * user){
   display_user_posts(user);
   char new_text[251];
   unsigned int post_num = 0;
   unsigned int number_of_posts;
   post_t *temp_post_p = user->posts;
   unsigned int choice = post_menu(user);
   while(choice != 4){
      number_of_posts = 0;
      temp_post_p = user->posts;
      while(temp_post_p != NULL){
         number_of_posts++;
         temp_post_p = temp_post_p->next;
      }
      if(choice == 1){
         printf("Please enter the new post's content (Press enter to finish): ");
         scanf(" %[^\n]s", new_text);
         add_post(user, new_text);
         printf("\n*** New post added successfully! ***\n");
         number_of_posts++;
         display_user_posts(user);
      } else if (choice == 2){
         if(delete_post(user, 0)){
            printf("\n*** Post deleted successfully! ***\n");
            number_of_posts--;
            display_user_posts(user);
         } else {
            printf("\n*** There were no posts to be deleted! ***\n");
         }
      } else if (choice == 3){
         if(number_of_posts != 0){
            printf("\nPlease enter the post's number you would like to delete: ");
            scanf("%d", &post_num);
            while(1 > post_num || post_num > number_of_posts){
               printf("\nInavlid post number.\nPlease enter the post's number you would like to delete: ");
               scanf("%d", &post_num);
            }
            if(delete_post(user, post_num)){
               printf("\n*** Post deleted successfully! ***\n");
               number_of_posts--;
               display_user_posts(user);
            }
         } else {
            printf("\n*** There were no posts to be deleted! ***\n");
         }
      }
      choice = post_menu(user);
   }
   return;
}

/*
   Function that handles the menu for user friends
*/
unsigned int friend_menu(user_t * user){
   printf("\n1. Display all of %s's friends\n2. Add a new friend\n3. Delete a friend\n4. Return to main menu\n\nPlease enter your choice: ", user->username);
   unsigned int choose;
   scanf("%d", &choose);
   while(1 > choose || choose > 4){
      printf("\n1. Display all of %s's friends\n2. Add a new friend\n3. Delete a friend\n4. Return to main menu\n\nPlease enter a value between 1 and 4: ", user->username);
      scanf("%d", &choose);
   }
   return choose;
}

/*
   Function that carries out options from the user friend menu
*/
void friend_process(user_t * users, const char * u_name){
   user_t * user = find_user(users, u_name);
   user_t * search_for_user = NULL;
   unsigned int opt = friend_menu(user);
   char name[101];
   _Bool delete_ok;
   friend_t *finding;
   while(opt != 4){
      finding = user->friends;
      switch(opt){
      case 1:
         display_user_friends(user);
         break;
      case 2:
         printf("Please enter a new friend's name: ");
         scanf("%s", name);
         search_for_user = find_user(users, name);
         // Verifying inputted name is a user 
         while(search_for_user == NULL){
            printf("\n%s is not a user here.\nPlease enter a new friend's name: ", name);
            scanf("%s", name);
            search_for_user = find_user(users, name);
         }
         // Verifying user is not already friends with new friend
         while ((finding != NULL)){
            while (strcmp(strlwr(name), strlwr(finding->username)) == 0){
               finding = user->friends;
               printf("\nSorry, %s is already your friend.\nPlease enter a new friend's name: ", name);
               scanf("%s", name);
               search_for_user = find_user(users, name);
               // Check name is a user
               while(search_for_user == NULL){
                  printf("\n%s is not a user here.\nPlease enter a new friend's name: ", name);
                  scanf("%s", name);
                  search_for_user = find_user(users, name);
               }
            }
            finding = finding->next;
         }
         strlwr(name);
         add_friend(user, name, search_for_user->userid);
         // Finding correct friend to link posts to friend's user
         finding = user->friends;
         while(strcmp(name, finding->username) != 0){
            finding = finding->next;
         }
         finding->posts = &(search_for_user->posts);
         printf("\n*** %s added successfully to friends! ***\n", name);
         break;
      case 3:
         // Checking if user has friends to delete
         if (user->friends != NULL){
            display_user_friends(user);
            printf("\nPlease enter the name of the friend you would like to delete: ");
            scanf("%s", name);
            // Checking to see if friend's name is in friend list
            while((finding != NULL) && (strcmp(strlwr(name), strlwr(finding->username)) != 0)){
               finding = finding->next;
            }
            while(finding == NULL){
               finding = user->friends;
               printf("\nInvalid friend name.\nPlease enter the name of the friend you would like to delete: ");
               scanf("%s", name);
               while((finding != NULL) && (strcmp(name, finding->username) != 0)){
                  finding = finding->next;
               }
            }
            delete_ok = delete_friend(user, name);
            if(delete_ok){
               printf("\n*** %s deleted successfully from friends! ***\n", name);
               display_user_friends(user);
            } else {
               printf("\n*** %s was not deleted from friends. ***\n", name);
            }
         } else {
            printf("\n*** Sorry, there are no friends to delete at this time. ***\n\n---\n");
         }
         break;
      }
      opt = friend_menu(user);
   }
   return;
}

/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        char *token3 = strtok(NULL, ",");
        users = add_user(users, token, atoi(token3), token2);
        char *username = token;
        int id = atoi(token3);

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token, rand());
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
