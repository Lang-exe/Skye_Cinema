#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <unistd.h> 
#include <time.h>
#include <windows.h>
#include <math.h>

struct Movie_Data{
    char seat[3];
	struct Movie_Data *next;
}; 

struct Datas{
    char title[50];
    int height;
    struct Datas *left, *right;
	struct Movie_Data *Visitor[5];
};

struct Datas *root = NULL;
struct Datas *saveRoot = NULL;

int letterFlag;
COORD coord = {0, 0};
int x, y;
int letterFlag;
int seatFlag[6][6];
char seatNumberVisitor[100][3]; 
int seatNumberVisitorFlag = 0;
char ChooseMovie[100];

void seatNumber();
void seatLetter();
void delay();
void ascii_cinemaName();
void gotoxy(int x, int y);
void buySeat();
void ascii_mainMenuAnimated();
void mainMenu();
void delay1();
void backToMainMenu();
void insert();
void summary();

int main (){
	system("CLS");
	ascii_mainMenuAnimated();
	mainMenu();
	return 0;
}

struct Movie_Data *createNewSeat (const char *kursi){
    struct Movie_Data *newSeat = (struct Movie_Data*) malloc(sizeof(struct Movie_Data));
    strcpy(newSeat->seat, kursi);
    newSeat->next = NULL;
    return newSeat;
}

void insertSeat (const char *kursi){
	struct Movie_Data *newSeat = createNewSeat(kursi);

	if (!saveRoot->Visitor[kursi[0] - 'A']){
		saveRoot->Visitor[kursi[0] - 'A'] = newSeat;
	} 
	else if (newSeat->seat[1] < saveRoot->Visitor[kursi[0] - 'A']->seat[1]){
		struct Movie_Data *newSeat = createNewSeat(kursi);
		newSeat->next = saveRoot->Visitor[kursi[0] - 'A'];
		saveRoot->Visitor[kursi[0] - 'A'] = newSeat;
	} 
	else if (newSeat->seat[1] > saveRoot->Visitor[kursi[0] - 'A']->seat[1]){
		struct Movie_Data *curr = saveRoot->Visitor[kursi[0] - 'A'];
		while (curr->next != NULL && kursi[1] > curr->next->seat[1]){
			curr = curr->next;
		}
		newSeat->next = curr->next;
		curr->next = newSeat;
	}
}

void insertSeatVariabel (){
	insertSeat(seatNumberVisitor[seatNumberVisitorFlag-1]);
	system("CLS");
	// seatMap();
	// for (int i = 0; i < seatNumberVisitorFlag; i++){
	// 	calculateDrawMap(seatNumberVisitor[i][0], seatNumberVisitor[i][1]);
	// }
	gotoxy(0,14);																																	
	int i = 0;
	for (i = 0; i < 5; i++){
		printf("Row %d: ", i + 1);
		if (saveRoot->Visitor[i]){
			struct Movie_Data *curr = saveRoot->Visitor[i];
			while (curr){
				printf("%s -> ", curr->seat);
				curr = curr->next;
			}
		}
		puts("NULL");
	}
	delay();
	delay();
}

void preOrder (struct Datas* root){
	if (root != NULL){
		printf("%s\n", root->title);
        preOrder(root->left);
        preOrder(root->right);
	}
}

struct Datas *createNewDatas (char title[]){
	struct Datas *root = (struct Datas*) malloc(sizeof(struct Datas));
	strcpy(root->title, title);
	root->left = NULL;
	root->right = NULL;
	root->height = 1;
	
	return root;
}

int maximum (int left, int right){
	return (left > right)? left : right;
} 
	
int getHeight (struct Datas *root){
	return (!root) ? 0 : root->height;
}

int getBalance (struct Datas *root){
	return (!root) ? 0 : getHeight(root->left) - getHeight(root->right);
}

int setHeight (struct Datas *root) {
    return maximum(getHeight(root->left), getHeight(root->right)) + 1;
}

struct Datas *leftRotate (struct Datas *root){
	struct Datas *y = root->right;
	struct Datas *temp = y->left;
	
	y->left = root;
	root->right = temp;
	
	root->height = setHeight(root);
	y->height = setHeight(root);

	return y;
}

struct Datas *rightRotate (struct Datas *root){
	struct Datas *y = root->left;
	struct Datas *temp = y->right;
	
	y->right = root;
	root->left = temp;
	
	root->height = setHeight(root);
	y->height = setHeight(root);

	return y;
}
	
struct Datas *selfBalance (struct Datas *root){
	root->height = 1 + maximum(getHeight(root->left), getHeight(root->right));

	int balance = getBalance(root);
	
	//Left Left Case
	if(balance > 1 && getBalance(root->left) >= 0){
		return rightRotate(root);
	}
	
	//Left Right Case
	if(balance > 1 && getBalance(root->left) < 0){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	
	//Right Right Case
	if (balance < -1 && getBalance(root->left) <= 0){
		return leftRotate(root);
	}
	
	//Right Left Case
	if (balance < -1 && getBalance(root->left) > 0){
		root->right = leftRotate(root->right);
		return leftRotate(root);
	}
	
	return root;
}	

struct Datas *insertDatas (struct Datas *root, char title[]){
	if (root == NULL){
		return createNewDatas(title);
	}
	else if (strcmp(title, root->title) < 0){
		root->left = insertDatas(root->left, title);
	}
	else if (strcmp(title, root->title) > 0){
		root->right = insertDatas(root->right, title);
	}
	else{
		return root;
	}
	
	return selfBalance(root);
}

struct Datas *searchMovie (struct Datas *root, char movie[]){
	if (root == NULL){
		return NULL;
	}
	else if (strcmp(movie, root->title) < 0){
		root->left = searchMovie(root->left, movie);
	}
	else if (strcmp(movie, root->title) > 0){
		root->right = searchMovie(root->right, movie);
	}
	else{
		
	}
}

struct Datas *insertMovie (struct Datas *root){
	char title[100];
	printf("Enter \"X\" to go back to main menu\n");
	printf("Insert movie title : ");
	scanf("%[^\n]", title); getchar();

	if (title[0] == 'X' || title[0] == 'x')
	{
		mainMenu();
	}

		root = insertDatas(root, title);
	return root;
}

struct Datas *preDesc (struct Datas *root){
	struct Datas *temp = root;
	while (temp->right != NULL){
		temp = temp->right;
	}
}

struct Datas *deleteDatas (struct Datas *root, char title[]){
	if (root == NULL){
		return NULL;
	}
	else if (strcmp(title, root->title) < 0){
		root->left = deleteDatas(root->left, title);
	}
	else if(strcmp(title, root->title) > 0){
		root->right = deleteDatas(root->right, title);
	}
	else{

        FILE *fp1 = fopen("db.txt", "a");
	
		fprintf(fp1, "%s\n", title);
	
		int i = 0;
		for (i = 0; i < 5; i++){
			fprintf(fp1, "Row %d: ", i + 1);
			if (root->Visitor[i]){
				struct Movie_Data *curr = root->Visitor[i];
				while (curr){
					fprintf(fp1, "%s -> ", curr->seat);
					curr = curr->next;
				}
			}
			fprintf(fp1, "NULL\n");
		}
		fprintf(fp1, "\n");
		fclose(fp1);
		
		if (root->right == NULL || root->left == NULL){
			struct Datas *temp = root->left ? root->left : root->right;
			free(root);
			root = temp;	
			if (root == NULL){
				return NULL;
			}
		}else{
			struct Datas *temp = root->left;
			while (temp->right){
				temp = temp->right;
			}
			strcpy(root->title, temp->title);
			for(int i = 0;i < 5;i++){
				root->Visitor[i] = temp->Visitor[i];
			}
			root->left = deleteDatas(root->left,temp->title);
		}
	}
	return selfBalance(root);
}

bool checkFilm (struct Datas *root, const char movie[]) {
	if (root != NULL) {
		if (strcmp(root->title, movie) == 0) {
			// printf("%s", root->title);
			saveRoot = root;
			return true;
		}
		else if (strcmp(movie, root->title) < 0){ 
			checkFilm(root->left, movie);	
		}
		else{
			checkFilm(root->right, movie);
		}
	}
}

void mainMenu (){
	system("CLS");
	ascii_cinemaName();
	printf("> Insert New Movie <\n");
	printf("Movie List\nChoose Seat\nList Visitor\nDelete Played Movie\nExit\n");

	char input;
	char mainMenuChar[6][30] = {"Insert New Movie", "Movie List", "Choose Seat", "List Visitor", "Delete Played Movie", "Exit"};
	int now = 0;

	for (int i = 0;; i++){
		input = _getch();
		system("CLS");

		if (input == 80){
			now++;
		}
		else if (input == 72){
			now--;
		}

		if (now < 0){
			now = 0;
		}
		else if (now > 5){
			now = 5;
		}

		if (input == '\x0D' && now == 0){
			printf("Insert New Movie\n");
			root = insertMovie(root);
			system("CLS");
		}
		else if (input == '\x0D' && now == 1){
			printf("Movie List\n");
			preOrder(root);
			printf("\nPress Enter to back to menu..."); getchar();
			system("CLS");
		}
		else if (input == '\x0D' && now == 2){
			printf("Choose Seat\n");
			printf("Enter \"X\" to go back to main menu\n");
			preOrder(root);
			do
			{
				printf("\nChoose Film : ");
				scanf("%[^\n]", &ChooseMovie);
				getchar();

				int compare1 = strcmp(ChooseMovie, "X");
				int compare2 = strcmp(ChooseMovie, "x");
				if (compare1 == 0 || compare2 == 0)
				{
					mainMenu();
				}
			} while (checkFilm(root, ChooseMovie) != true);
			buySeat();
		}
		else if (input == '\x0D' && now == 3){
			printf("List Visitor\n");
			preOrder(root);
			printf("Enter \"X\" to go back to main menu\n");
			do{
				printf("\nChoose Film : ");
				scanf("%[^\n]", &ChooseMovie);
				getchar();
				
				int compare1 = strcmp(ChooseMovie, "X");
				int compare2 = strcmp(ChooseMovie, "x");
				if (compare1 == 0 || compare2 == 0)
				{
					mainMenu();
				}
			} while (checkFilm(root, ChooseMovie) != true);
			printf("\n");

			for (i = 0; i < 5; i++){
				printf("Row %d: ", i + 1);
				if (saveRoot->Visitor[i]){
					struct Movie_Data *curr = saveRoot->Visitor[i];
					while (curr){
						printf("%s -> ", curr->seat);
						curr = curr->next;
					}
				}
				puts("NULL");
			}
			printf("\nPress Enter...");getchar();
			system("CLS");
		}
		else if (input == '\x0D' && now == 4){
			printf("Delete Played Movie\n");
			printf("Enter \"X\" to go back to main menu\n");
			preOrder(root);
			do{
				printf("\nChoose Film to Delete : ");
				scanf("%[^\n]", &ChooseMovie);
				getchar();
				int compare1 = strcmp(ChooseMovie,"X");
				int compare2 = strcmp(ChooseMovie,"x");
				if (compare1 == 0|| compare2 == 0)
				{
					mainMenu();
				}
			} while (checkFilm(root, ChooseMovie) != true);

			root = deleteDatas(root, ChooseMovie);
			system("CLS");
		}
        else if (input == '\x0D' && now == 5){
			summary(root);
            printf("Thank you for using our program!\n");
			exit(0);
		}

		for (int i = 0; i < 6; i++){
			if (i != now){
				printf("%s\n", mainMenuChar[i]);
			}
			else{
				printf("> %s <\n", mainMenuChar[now]);
			}
		}
	}
}

void seatNumber (){
	printf("1");

	char input;
	char mainMenu[5][30] = {"1", "2", "3", "4", "5"};
	int now = 0;

	for (int i = 0;; i++){
		input = _getch();
		system("CLS");
		printf("Enter your seat (use your arrow up and down key): ");

		if (letterFlag == 0){
			printf("A");
		}
		else if (letterFlag == 1){
			printf("B");
		}
		else if (letterFlag == 2){
			printf("C");
		}
		else if (letterFlag == 3){
			printf("D");
		}
		else if (letterFlag == 4){
			printf("E");
		}

		if (input == 80){
			now++;
		}
		else if (input == 72){
			now--;
		}

		if (now < 0){
			now = 0;
		}
		else if (now > 4){
			now = 4;
		}

		if (input == '\x0D' && now == 0){
			printf("1");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "1");
			break;
		}
		else if (input == '\x0D' && now == 1){
			printf("2");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "2");
			break;
		}
		else if (input == '\x0D' && now == 2){
			printf("3");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "3");
			break;
		}
		else if (input == '\x0D' && now == 3){
			printf("4");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "4");
			break;
		}
		else if (input == '\x0D' && now == 4){
			printf("5");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "5");
			break;
			// exit(1);
		}
		printf("%s", mainMenu[now]);
	}
}

void seatLetter (){
	system("CLS");
	printf("Enter your seat (use your arrow up and down key): ");
	printf("A");

	char input;
	char mainMenu[5][30] = {"A", "B", "C", "D", "E"};
	int now = 0;

	for (int i = 0;; i++){
		input = _getch();
		system("CLS");
		printf("Enter your seat (use your arrow up and down key): ");

		if (input == 80){
			now++;
		}
		else if (input == 72){
			now--;
		}

		if (now < 0){
			now = 0;
		}
		else if (now > 4){
			now = 4;
		}

		if (input == '\x0D' && now == 0){
			printf("A");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "A");
			break;
		}
		else if (input == '\x0D' && now == 1){
			printf("B");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "B");
			break;
		}
		else if (input == '\x0D' && now == 2){
			printf("C");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "C");
			break;
		}
		else if (input == '\x0D' && now == 3){
			printf("D");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "D");
			break;
		}
		else if (input == '\x0D' && now == 4){
			printf("E");
			strcat(seatNumberVisitor[seatNumberVisitorFlag], "E");
			break;
			// exit(1);
		}
		printf("%s", mainMenu[now]);
	}
	letterFlag = now;
	seatNumber();
}

void gotoxy (int x, int y){
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void delay (){
	for (int i = 0; i < 5e8; i++)
	{
	}
}

void buySeat (){
	printf("Enter your seat (use your arrow up and down key): ");	
	seatLetter();

	for (int i = 0; i < seatNumberVisitorFlag; i++){
		struct Movie_Data *temp = saveRoot->Visitor[seatNumberVisitor[seatNumberVisitorFlag][0] - 'A'];
		if (temp != NULL){
			while (temp != NULL){
				if (strcmp(seatNumberVisitor[seatNumberVisitorFlag], temp->seat) == 0){
					printf("\nThis seat is not available\n");
					delay();
					delay();
				}
				temp = temp->next;
			}
		}
	}
	seatNumberVisitorFlag++;

	gotoxy(0, 14);
	printf("Do you want to purchase a ticket again?\n");
	printf("> Yes <\n");
	printf("No\n");

	char input;
	char texts[3][30] = {"Yes", "No"};
	int inputChoice = 0;

	for (int i = 0;; i++){
		input = _getche();
		system("CLS");
		gotoxy(0, 14);
		printf("Do you want to purchase a ticket again?\n");

		if (input == 80){
			inputChoice++;
		}
		else if (input == 72){
			inputChoice--;
		}

		if (inputChoice < 0){
			inputChoice = 0;
		}
		else if (inputChoice > 1){
			inputChoice = 1;
		}

		if (input == '\x0D' && inputChoice == 0){ // yes or no question
			system("CLS");
			insertSeatVariabel(seatNumberVisitor[seatNumberVisitorFlag-1]);
			buySeat();
		}
		else if (input == '\x0D' && inputChoice == 1){
			insertSeatVariabel(seatNumberVisitor[seatNumberVisitorFlag-1]);
			mainMenu();
		}

		for (int i = 0; i < 2; i++){

			if (i != inputChoice){
				printf("%s\n", texts[i]);
			}
			else{
				printf("> %s <\n", texts[inputChoice]);
			}
		}
	}
}

void summary (struct Datas *root){
	FILE *fp2 = fopen("db.txt", "a");
		
	if (root != NULL){
		fprintf(fp2, "%s\n", root->title);
		int i = 0;
		for (i = 0; i < 5; i++){
			fprintf(fp2, "Row %d: ", i + 1);
			if (root->Visitor[i]){
				struct Movie_Data *curr = root->Visitor[i];
				while (curr){
					fprintf(fp2, "%s -> ", curr->seat);
					curr = curr->next;
				}
			}
			fprintf(fp2, "NULL\n");
		}
		fprintf(fp2, "\n");
	}
	fclose(fp2);
}

void backToMainMenu (){
	char input;
	input = _getche();
	puts("\n\nPress enter to continue...");
	if (input == '\x0D'){
		system("CLS");
		mainMenu();
	}
}

void delay1 (){
	for (int i = 0; i < 2e8; i++){
		
	}
}

void ascii_mainMenuAnimated (){
	printf(" __          __  _                            _ \n");
	delay1();
	printf(" \\ \\        / / | |                          | |\n");
	delay1();
	printf("  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___  | |\n");
	delay1();
	printf("   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | |\n");
	delay1();
	printf("    \\  /\\  /  __/ | (_| (_) | | | | | |  __/ |_|\n");
	delay1();
	printf("     \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___| (_)\n");
	delay1();
}

void ascii_cinemaName (){
	printf("   _____ _                  _____ _                            \n");
	printf("  / ____| |                / ____(_)                           \n");
	printf(" | (___ | | ___   _  ___  | |     _ _ __   ___ _ __ ___   __ _ \n");
	printf("  \\___ \\| |/ / | | |/ _ \\ | |    | | '_ \\ / _ \\ '_ ` _ \\ / _` |\n");
	printf("  ____) |   <| |_| |  __/ | |____| | | | |  __/ | | | | | (_| |\n");
	printf(" |_____/|_|\\_\\__,  |\\___|  \\_____|_|_| |_|\\___|_| |_| |_|\\__,_|\n");
	printf("               __/ |                                           \n");
	printf("              |___/                                            \n\n\n");
}