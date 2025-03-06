/********* main.c ********
    Student Name 	= Kaitlyn Bui
    Student Number	= 101273818
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"
#define NUM_POSTS 3

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
    user_t *keep_head = users;
    // Changing all usernames to lowercase
    while(users!=NULL)
    {
        strlwr(users->username);
        users = users->next;
    }
    users = keep_head;
    //display_all_users(users);
    printf("************************************************\nWelcome to Text-Based Facebook\n************************************************\n\n");
    char name[31];
    int id;
    char pass[16];
    user_t *temp_p = NULL;
    unsigned int main_choice = main_option();
    unsigned int option;
    while (main_choice != 3) {
        if (main_choice == 1){
            printf("Please enter the new user's username: ");
            scanf("%s", name);
            // Checking that the username is not already an existing username
            temp_p = find_user(users, name);
            while(temp_p != NULL){
                printf("Sorry, this user already exists.\n");
                printf("Please enter a different username: ");
                scanf("%s", name);
                temp_p = find_user(users, name);
            }
            strlwr(name);
            printf("Please enter the new user's id: ");
            scanf("%d", &id);
            // Checking id input is not negative
            while(id < 0){
                printf("Invalid input, ID cannot be negative.\nPlease enter the new user's id: ");
                scanf("%d", &id);
            }
            printf("Please enter the new user's password (up to 15 characters): ");
            scanf("%s", pass);
            users = add_user(users, name, id, pass);
            printf("\n*** %s Added Successfully! ***\n", name);
        } else if (main_choice == 2){
            printf("Please enter the username: ");
            scanf("%s", name);
            printf("Please enter the password: ");
            scanf("%s", pass);
            temp_p = find_user(users, name);
            // Displays error if user DNE or password is incorrect.
            if(temp_p == NULL){
                printf("\nUsername %s does not exist.\n\n", name);
            } else if (strcmp(pass, temp_p->password) != 0){
                printf("\nThe password is incorrect.\n");
            } else {
                option = user_option(name);
                while(option != 5){
                    switch(option){
                    case 1:
                        printf("Please enter your new desired password (up to 15 characters): ");
                        scanf("%s", pass);
                        strcpy(temp_p->password, pass);
                        printf("\n*** %s's Password Changed Successfully to %s ***\n", temp_p->username, temp_p->password);               
                        break;
                    case 2:           
                        post_process(temp_p);
                        break;
                    case 3:            
                        friend_process(users, temp_p->username);
                        break;
                    case 4:
                        friend_t *friend_p = temp_p->friends;
                        user_t *user_friend = NULL;
                        char f_name[31];
                        // Checks if user has friends
                        if (friend_p != NULL){
                            display_user_friends(temp_p);
                            printf("Please enter the friend whose posts you would like to view: ");
                            scanf("%s", f_name);
                            while(friend_p != NULL && (strcmp(strlwr(f_name), strlwr(friend_p->username)) != 0)){
                                friend_p = friend_p->next;
                            }
                            // Checking if the friend is on user's friend list
                            while(friend_p == NULL){ 
                                printf("\nInvalid friend name.\nPlease enter the friend whose posts you would like to view: ");
                                scanf("%s", f_name);
                                strlwr(f_name);
                                friend_p = temp_p->friends;
                                while(friend_p != NULL && (strcmp(strlwr(f_name), strlwr(friend_p->username)) != 0)){
                                    friend_p = friend_p->next;
                                }
                            }
                            user_friend = find_user(users, f_name);
                            display_posts(user_friend, NUM_POSTS);
                        } else {
                            printf("Sorry, you do not currently have any added friends.\n");
                        }
                        
                        break;            
                    } 
                    option = user_option(name);
                }
            }
        }
        main_choice = main_option();
    }
    printf("\nThank you, have a fantastically whimsical day! Bye for now!");
    teardown(users);
}