/*
 * main.c
 *
 *  Created on: May 24, 2018
 *      Author: sheida
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 51

typedef enum {M,F} Sex;

typedef struct node{


	char name[51];
	char lastName[51];
	int age;
	Sex sex;
	struct node *next;
	struct node *mother;
	struct node *father;
	struct node *children;
	struct node *nextBySex;
	struct node *nextByAge;

}node_t;

node_t *readFile(char *,int *);
node_t set2Null(node_t );//we already know about the specific pointer
node_t *search(char *,char *,node_t *,int ); //the last attribute is not a pointer we already know about the size of the file
node_t *assignParent(node_t *,node_t *,node_t *);
node_t *assignChild(node_t * ,node_t *);
int find_path_by_sex(node_t *,int, Sex);
int path_by_age_R(node_t *,int);
void solution_write(node_t *);
void solution_write_age(node_t *);

int main(int argc,char *argv[]){

	node_t *info;
	node_t *path_by_sex;
	node_t *path_by_age;
	int n,i,j,max;

	info=readFile(argv[1],&n);

	//max=0;
	for(max=0,i=0;i<n;i++){

		if(info[i].sex!=M) continue;

		j=find_path_by_sex(&info[i],0,M);
			//finding he maximum number of the male
			if(j>max){
				max=j;
				path_by_sex=&info[i];
			}
		}
	printf("path by sex: %d males\n",max);
	solution_write(path_by_sex);

	printf("\n");
	/*path for female*/
	printf("by F sex:\n");
	//max=0;
		for(max=0,i=0;i<n;i++){

			if(info[i].sex!=F) continue;

			j=find_path_by_sex(&info[i],0,F);
				//finding he maximum number of the females
				if(j>max){
					max=j;
					path_by_sex=&info[i];
				}
			}
		printf("path by sex: %d females\n",max);
		solution_write(path_by_sex);
		printf("\n");

		//path for age
		for(i=0;i<n;i++){
		j=path_by_age_R(&info[i],0);
		if(j>max){
			max=j;
			path_by_age=&info[i];
		}
		}
		printf("\n");
		printf("path by age : %d years",max);
		solution_write(path_by_age);

		free(info);


	return 0;

}
//because I dont know how many people there are in my file I should mention that in the function

node_t *readFile(char *fileName,int  *dim){
	FILE *fp;
	int i;
	char sex;
	node_t *person;
	node_t *currentChild,*currentDad,*currentMom;
	char childFN[MAX],childLN[MAX],fatherFN[MAX],fatherLN[MAX],motherFN[MAX],motherLN[MAX];

	fp=fopen(fileName,"r");
	if(fp==NULL){
		printf("Error on opening file\n");
		exit(0);
	}

	fscanf(fp,"%d",dim);

	//allocationg your info in sizeof the n

	person=(node_t *)malloc((*dim)*sizeof(node_t));
	if(person==NULL){
		printf("Allocation Error!\n");
		exit(0);
	}

// read the file in size of n and you save them in a proper data structure

	i=0;
	while(i<*dim){


		fscanf(fp,"%s %s %c %d",person[i].name,person[i].lastName,&sex,&person[i].age);
		//because we already specify which person has to be NULL
		person[i]=set2Null(person[i]);
		if(sex=='M'){
			person[i].sex=M;

		}if(sex=='F'){
			person[i].sex=F;
		}
		i++;
	}

	//now read the other lines of the file
	//fscanf(fp,"\n");

	while(fscanf(fp,"%s %s %s %s %s %s",childFN,childLN,fatherFN,fatherLN,motherFN,motherLN)!=EOF){

		/*anytime I read a line if the name was equal to the other name I read in my File
		/*I have to search in my file (first part)and put it in the proper pointer
		 *
		 * so I need 3 pointer (from my data structure)to devide children father and mother
		 */
		currentChild=search(childFN,childLN,person,*dim);
		currentDad=search(fatherFN,fatherLN,person,*dim);
		currentMom=search(motherFN,motherLN,person,*dim);


		currentChild=assignParent(currentChild,currentDad,currentMom);
		currentDad=assignChild(currentChild,currentDad);
		currentMom=assignChild(currentChild,currentMom);


	}

	fclose(fp);



	return person;
}

node_t set2Null(node_t person){


	person.father=NULL;
	person.mother=NULL;
	person.children=NULL;
	person.next=NULL;
	person.nextByAge=NULL;
	person.nextBySex=NULL;

	return person;
}

node_t *search(char *name,char *lastName,node_t * person,int n){

	int i;
	for(i=0;i<n;i++){
		if(strcmp(person[i].name,name)==0 && strcmp(person[i].lastName,lastName)==0){
			return &person[i];
		}
	}
	return NULL;

}

node_t *assignParent(node_t *child,node_t *father,node_t *mother){

	child->mother=mother;
	child->father=father;
	return child;
}

node_t *assignChild(node_t * child,node_t *parent){

	child->next=parent->children;
	parent->children=child;
	return parent;
}

int find_path_by_sex(node_t *currentPerson,int count,Sex sex){

	int i;
	node_t *ptr= currentPerson,*tmp=currentPerson->children;
	//termination case
	if(ptr ==NULL || ptr->sex !=sex){
		return count;
	}
	count++;
	while(ptr->children!=NULL){
		i=find_path_by_sex(ptr->children,sex,count);
		if(i>count){
			count=i;
			(*currentPerson).nextBySex=ptr->children;
				}
		ptr->children=ptr->children->next;

	}
	ptr->children=tmp;
	return count;
}
//find the path by age

int path_by_age_R(node_t *curr,int age){

	node_t *ptr=curr;
	node_t *tmp=curr->children;

	int i,max=0;
//termination

	if(ptr==NULL ) return age;
	if(ptr->children ==NULL){
		return age+ptr->age;

	}
	while (ptr->children !=NULL){
		i=path_by_age_R(ptr->children,age+ptr->age);
		if(i>max){

			max=i;
			//(*curr).nextByAge=ptr->children;
			ptr->nextByAge=ptr->children;
		}
		ptr->children=ptr->children->next;
		//tmp=tmp->next;
	}
	ptr->children=tmp;

	return max;


}
void solution_write(node_t *ptr){

	while(ptr!=NULL){
		printf("%s %s ",ptr->name,ptr->lastName);
		ptr=ptr->nextBySex;
	}
}
void solution_write_age(node_t *ptr){
	while(ptr !=NULL){
		printf("%s %s ",ptr->name,ptr->lastName);
		ptr=ptr->nextByAge;
	}
}


