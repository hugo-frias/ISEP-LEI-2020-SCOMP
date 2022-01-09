#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define STRING_SIZE 80
#define NR_CHILD_PROCESSES 5
#define NR_PRODUCTS 3
#define NR_BARCODES 5

int pl2_ex12() {
	//Create Product struct
	struct s_product {
		char product_name[STRING_SIZE];
		char product_price[STRING_SIZE];
	} ;
	
	//Variables
	int status;
	int nr_requests = 0;
	int i;
	int all_barcodes[NR_CHILD_PROCESSES];
	int all_nr_products[NR_CHILD_PROCESSES];
	int process_id;
	pid_t p;
	int pids[NR_CHILD_PROCESSES];
	int active_barcodes[NR_CHILD_PROCESSES];
	int barcodes[NR_BARCODES];
	int request_products[NR_BARCODES];
	int barcode_to_read = 0;
	int nr_product_to_read = 0;
	char product_name_to_read[STRING_SIZE] = {0};
	char product_price_to_read[STRING_SIZE] = {0};
	
	//Product's database
	struct s_product products[NR_PRODUCTS];
	
	struct s_product p1, p2, p3;
	strcpy(p1.product_name, "Pasta dos dentes");
	strcpy(p1.product_price, "2,00 euros");
	strcpy(p2.product_name, "Shampô");
	strcpy(p2.product_price, "5,00 euros");
	strcpy(p3.product_name, "Creme facial");
	strcpy(p3.product_price, "4,00 euros");
	
	products[0] = p1;
	products[1] = p2;
	products[2] = p3;
	
	
	//Create 6 pipes
	int fd[2];
	if (pipe(fd) == -1) {
		perror("Erro ao criar pipe!");
		return 1;
	}
	int fd1[2];
	int pipes[NR_CHILD_PROCESSES*2];
	for(i=0;i<NR_CHILD_PROCESSES*2;i=i+2) {
		if (pipe(fd1) == -1) {
			perror("Erro ao criar pipe!");
			return 1;
		}
		pipes[i] = fd1[0];
		pipes[i+1] = fd1[1];
	}
	
	//Ask user to choose barcode and product's info
	printf("Escolha um barcode (1 a 5): ");
	scanf("%d", &barcodes[0]);
	printf("\n1.Pasta de dentes\n");
	printf("2.Shampô\n");
	printf("3.Creme facial\n");
	printf("\nEscolha um dos produtos acima (coloque a numeração): ");
	scanf("%d", &request_products[0]);
	nr_requests++;
	
	//Test case, with more than one barcode on use
	/*barcodes[1] = 2;
	barcodes[2] = 3;
	request_products[1] = 1;
	request_products[2] = 2;
	nr_requests = 3;*/
	
	//Create 5 processes
	process_id = 0;
	p = getpid();
	for(i=0;i<NR_CHILD_PROCESSES;i++) {
		if (p > 0) {
			p = fork();
			if (p == 0) {
				if (process_id == 0) {
					process_id = i+1;
				}
			} else {
				pids[i] = p;
			}
		}
	}

	//Send request to parent process
	for(i=0;i<nr_requests;i++) {
		if (barcodes[i] == process_id) {
			
			sleep(1*i);
			
		    close(fd[0]);
		    write(fd[1],&barcodes[i],sizeof(int));
		    write(fd[1],&request_products[i],sizeof(int));
		    close(fd[1]);
		    
		    int fd2[2];
		    fd2[1] = pipes[2*(process_id)-1];
		    fd2[0] = pipes[2*(process_id-1)];
		    close(fd2[1]);
		    read(fd2[0],product_name_to_read,strlen(products[(request_products[i]-1)].product_name));
		    read(fd2[0],product_price_to_read,strlen(products[(request_products[i]-1)].product_price));
		    
		    sleep(1*i);
		    
		    printf("\nBarcode nº %d\n", process_id);
		    printf("Nome do produto: %s\n", product_name_to_read);
		    printf("Preço do produto: %s\n", product_price_to_read);
		    close(fd2[0]);
		    exit(0);
		}
	}
	
	//Kill unnecessary processes
	if(process_id != 0) {
		exit(0);
	}
	
	//Parent process reads the requests
	close(fd[1]);
	for(i=0;i<nr_requests;i++) {
		read(fd[0],&barcode_to_read,sizeof(int));
		read(fd[0],&nr_product_to_read,sizeof(int));
		active_barcodes[i] = barcode_to_read;
		all_barcodes[i] = barcode_to_read;
		all_nr_products[i] = nr_product_to_read;
	}
	close(fd[0]);
	
	//Parent process sends to the requesting childs the corresponding product's info
	for(i=0;i<nr_requests;i++) {			
		int fd3[2];
		fd3[0] = pipes[2*(all_barcodes[i]-1)];
		fd3[1] = pipes[2*(all_barcodes[i])-1];
		close(fd3[0]);
		write(fd3[1],products[(all_nr_products[i]-1)].product_name,strlen(products[(all_nr_products[i]-1)].product_name));
		write(fd3[1],products[(all_nr_products[i]-1)].product_price,strlen(products[(all_nr_products[i]-1)].product_price));
		close(fd3[1]);			
	}
		
	//Kill the remaining child processes
	for(i=0;i<nr_requests;i++) {
		waitpid(pids[active_barcodes[i]-1],&status,0);
	}
	return 0;
}

int main() {
    int i = pl2_ex12();
    if (i == 0) {
        printf("\n\nOperação bem sucedida!");
    } else {
        printf("\n\nOperação má sucedida!");
    }
    return 0;
}
