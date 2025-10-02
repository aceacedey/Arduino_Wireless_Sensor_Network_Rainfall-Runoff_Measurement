#include<stdio.h>
#include<conio.h>

void PrintMenu()
{
	printf("Enter the choice\n");
	printf("1.Insert a Key\n");
	printf("2.Delete a Key\n");
	printf("3.Search a Key\n");
	printf("4.Print all keys\n");
	printf("5.Exit\n");
}
void InsertKey()
{
	int insertion_key;
	printf("Enter the key to be inserted\n");
	scanf("%d",&insertion_key);

	DHTInsert(insertion_key);
	
}
void DeleteKey()
{
	int delete_key;
	printf("Enter the key to be deleted\n");
	scanf("%d",&delete_key);

	DHTDelete(delete_key);
	
}
void SearchKey()
{
	int search_key;
	printf("Enter the key to be searched\n");
	scanf("%d",&search_key);

	DHTDelete(search_key);
	
}
void PrintAllkeys()
{
	DHTPrintAllkeys();
}
void Exit()
{
	exit(0);
}
int main()
{
	int choice;
	
	while(1)
	{
		PrintMenu();
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:InsertKey();
			       break;
			case 2:DeleteKey();
			       break;
			case 3:SearchKey();
			       break;
			case 4:PrintAllKeys();
			       break;
			case 5:Exit();
			default:break;
		}
	}
	
	return 0;
}
