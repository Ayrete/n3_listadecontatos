#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	char name[51];
	char phone[16];
	char email[251];
} contact;

#define LIMIT 250
#define FILE_NAME "contacts.ccf"

contact contacts[LIMIT];
int counter = 0;

void contactLoad(){

	FILE *file = fopen(FILE_NAME,"a+");
	
	if(file != NULL){	
		rewind(file);
				
		while(counter < LIMIT && fgets(contacts[counter].name, 50, file)!=NULL){
				fgets(contacts[counter].phone, 15, file);
				fgets(contacts[counter].email, 250, file);
				counter++;
			}
			
			fclose(file);
		}else{
		printf("Erro ao carregar contatos.\n");
	}
	
}

void contactSave(contact ctt, FILE *file){
	fputs(ctt.name, file);
	fputs(ctt.phone, file);
	fputs(ctt.email, file);
}

bool fileRewrite(int ignore){
	bool r = false;
	contact temp[LIMIT];
	int n = 0;
	
	remove(FILE_NAME);
	FILE *file = fopen(FILE_NAME, "a+");
	
	if(file != NULL){
		for(int i=0; i<counter; i++){
			if(i != ignore){
				contactSave(contacts[i], file);
				temp[n] = contacts[i];
				n++;
			}
		}
		
		for(int i=0; i<n; i++){
			contacts[i] = temp[i];
		}
		counter = n;
		r = true;
		
		fclose(file);

	}	
	return r;
}

bool contactInsert(contact ctt){
	
	bool r = false;
	FILE *file = fopen(FILE_NAME, "a+");
	
	if(file != NULL){
		r = true;
		
		contactSave(ctt, file);
		contacts[counter] = ctt;
		counter++;
		
		fclose(file);	
	}
		
	return r;
}

void contactCreate(){
	
	if(counter < LIMIT){
		printf("\nNOVO CONTATO\n");
	
		contact ctt;
		
		printf("Nome: ");
		fgets(ctt.name, 50, stdin);
		fflush(stdin);
		
		printf("Telefone: ");
		fgets(ctt.phone, 15, stdin);
		fflush(stdin);
		
		printf("E-mail: ");
		fgets(ctt.email, 250, stdin);
		fflush(stdin);
		
		if(contactInsert(ctt)){
			printf("\nContato salvo!\n");
		}else{
			printf("\nErro ao salvar contato.\n");
		}
	}else{
		printf("\nMemoria cheia.\n");
	}	
}

void contactEdit(int i){
	
	contact ctt;
	
	printf("\nEDITAR CONTATO\n");
	
	printf("%s",contacts[i].name);
	fgets(ctt.name, 50, stdin);
	fflush(stdin);
	
	if(strlen(ctt.name) > 1){
		strcpy(contacts[i].name, ctt.name);
	}
	
	printf("%s",contacts[i].phone);
	fgets(ctt.phone, 15, stdin);
	fflush(stdin);
	
	if(strlen(ctt.phone) > 1){
		strcpy(contacts[i].phone, ctt.phone);
	}
	
	printf("%s",contacts[i].email);
	fgets(ctt.email, 250, stdin);
	fflush(stdin);
	
	if(strlen(ctt.email) > 1){
		strcpy(contacts[i].email, ctt.email);
	}
	
	if(fileRewrite(-1)){
		printf("\nContato salvo.\n");
	}else{
		printf("\nErro ao atualizar arquivo.\n");
	}
}

bool confirm(char *message){
	char r;
	
	printf("%s | S para SIM, outro para NAO", message);
	scanf(" %c", &r);
	
	if(r == 's' || r == 'S'){
		return true;
	}else{
		return false;
	}	
}

void contactDelete(int i){

	if(confirm("\nDeseja excluir este contato?")){
		if(fileRewrite(i)){			
			printf("\nContato excluido.\n\n");
		}		
	}
}

void menuSelect(){
	int selected = -1;
	int op = 0;
	
	printf("Informe o codigo do contato para selecionar | 0 para voltar ao menu : ");
	scanf("%i", &selected);
	fflush(stdin);
	
	if(selected > 0){
		if(selected <= counter){
			selected--;
			printf("\nSelecionado: %s\n", contacts[selected].name);
			printf("1 - Editar\n");
			printf("2 - Excluir\n");
			printf("Outro para sair\n");
			
			scanf("%i", &op);
			fflush(stdin);
			

			switch(op){
				case 1: contactEdit(selected);
				break;
				case 2: contactDelete(selected);
				break;
			}	
		}else{
			printf("\nContato inexistente.\n\n");
		}
	}
}

void contactShow(int i){
	printf("[%i]\n", i+1);
	printf("%s", contacts[i].name);
	printf("%s", contacts[i].phone);
	printf("%s", contacts[i].email);
}

void contactList(){
	printf("\nLISTA DE CONTATOS\n\n");
	
	for(int i=0; i<counter; i++){
		contactShow(i);
		printf("------------------------\n");
	}
	
	if(counter == 0)
		printf("Nenhum contato cadastrado.\n");
}

bool searchMatch(char *a, char *b, int i, int limit){
	if(a[i] == b[i]){
		i++;
		if(i<limit){
			return searchMatch(a, b, i, limit);
		}else{
			return true;
		}
	}else{
		return false;
	}
}

void contactSearch(){
	char name[51] = {""};
	bool found = false;
	
	printf("BUSCA CONTATO\n\n");
	printf("Digite o nome para busca: ");
	fgets(name, 50, stdin);
	fflush(stdin);
	
	int limit = strlen(name);
	limit--;
	
	printf("\nRESULTADO\n\n");
	
	for(int i=0; i<counter; i++){
		if(strlen(contacts[i].name) >= limit){
			if(searchMatch(name, contacts[i].name, 0, limit)){
				contactShow(i);	
				printf("----------------------------\n");
				found = true;
			}
		}
	}
	
	if(!found){
		printf("Contato nao encontrado. \n");
		}
	
	if(found){
		menuSelect();
	}
}
void contactDeleteMenu(){
	int selected;
		
	if(counter == 0){
		printf("\n Nao ha contatos cadastrados. \n");
		return;
	}
	printf("\nLISTA DE CONTATOS\n\n");
		for(int i = 0; i < counter; i++){
			contactShow(i);
			printf("-------------------------\n");
		}
		
		printf("Informe o codigo do contato que deseja excluir (0 para cancelar): ");
		scanf("%i", &selected);
		fflush(stdin);
		
		if(selected > 0 && selected <= counter){
			contactDelete(selected - 1);
		}else if(selected != 0){
			printf("\nContato inexistente.\n");
		}
}

void menuDefault(){
	int op = 0;
	
	while(op != 5){
		printf("Selecione a opcao desejada:\n");
		printf("1 - Incluir contato:\n");
		printf("2 - Listar contatos:\n");
		printf("3 - Consultar contato:\n");
		printf("4 - Excluir contato:\n");
		printf("5 - Sair\n");
		
		scanf("%i", &op);
		fflush(stdin);
		
		if(op != 5){
			switch(op){
				case 1: contactCreate();
				break;
				case 2: contactList();
				break;
				case 3: contactSearch();
				break;
				case 4: contactDeleteMenu();
				break;
				default: printf("Opcao invalida.");
			}
		}else{
			printf("Voce saiu!\n");
		}
	}
}

int main(){
	contactLoad();
	menuDefault();
}