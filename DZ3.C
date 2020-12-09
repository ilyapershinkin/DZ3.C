#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void PrintGraph(int num, char **graph){
    printf("\n");
    int graph_check = 1;
    
    for(int i=0; i<num; i++){
        int temp_graph_check = 0;
        for(int j=0; j<num; j++){
            if(graph[i][j+1] == 1)
                temp_graph_check = 1;
    
            if(graph[j][i+1] == 1)
                temp_graph_check = 1;
        }
        if(temp_graph_check == 0)
            graph_check=0;
    }
    
    if(graph_check == 0){
        printf("unrelated graph\n");
    } else {
        printf("related graph\n");

    }
    printf("--------------------\n");

    for(int i=0; i<num; i++) {
        
        printf("%c: ", graph[i][0]);
        
        for (int j=1; j<num+1; j++) {
            printf("%d  ", graph[i][j]);
        }

        printf("\n");
    }
}

char **EnterGraph(int num){
    char **graph;
    graph = calloc(num, sizeof(char*));
    for(int i=0; i<num; i++){
        graph[i] = calloc(num+1, sizeof(char));
    }
    
    for(int i=0; i<num; i++){
        printf("%d -> ",i+1);
        graph[i][0] = getchar();
        getchar();

        if(!(('A' <= graph[i][0] && graph[i][0] <= 'Z') || ('a' <= graph[i][0] && graph[i][0] <= 'z'))){
            printf("Введите БУКВУ!\n");
            i--;
        } else {
            for(int j=0; j<i; j++){
                if( graph[i][0] == graph[j][0] ){
                    printf("Введите другую букву\n");
                    i--;
                }
            }
        }
    }
    return graph;
}

char **EnterGraphConn(int num, char **graph){
    for(int i=0; i<num; i++){
        for(int j=1; j<num; j++){
            graph[i][j] = 0;
        }
    }
    printf("enter connections like: A-B;B-C;...\n");
    char str[1024] = {0};
    scanf("%s", str);
    int i=0;
    if(str[0] == ' ' || str[0] == ';' || str[0] == '\n'){
        str[0] = '\0';
    }
    while(str[i] != '\0'){
        char name1 = 0, name2 = 0;
        name1 = str[i++];
        i++;
        name2 = str[i++];
        i++;
    
        for(int k=0; k<num; k++){
            if(graph[k][0] == name1){
                name1 = k;
            }
            if(graph[k][0] == name2){
                name2 = k;
            }
        }
        graph[name1][name2+1]++;
    
    }
    return graph;
}

void OutputGraph(int num, char **graph){
    FILE *file=fopen("graph.dot", "w");
    if(file == NULL) {
        printf("Unable to create file\n");
        exit(0);
    }
    
    char filestr[1024] = {0};
    
    strcat(filestr, "digraph G {");
    
    for(int i=0; i<num; i++){
        char elem[2] = "";
        elem[0] = graph[i][0];
        strcat(filestr, elem);
        strcat(filestr, "; ");
    }
    for(int i=0; i<num; i++){
        for(int j=0; j<num; j++){
            for(int k=0; k<graph[i][j+1]; k++){
                char elem[2] = "";
                elem[0] = graph[i][0];
                
                strcat(filestr, elem);
                strcat(filestr, "->");
                
                elem[0] = graph[j][0];
                strcat(filestr, elem);
                strcat(filestr, "; ");
            }
        }
    }
    strcat(filestr, "}");
    
    fputs(filestr, file);
    fclose(file);

}

char **SortGraph(char **graph, int num, int V){
    // 20. Удалить заданную вершину и в модифицированном графе
    //     отсортировать вершины по убыванию степени вершины

    for(int i=0; i<num; i++){
        for(int j=1; j<num+1; j++){
            int a=i, b=j;
            if(i > V){ a--;}
            if(j > V+1){ b--;}
            if(a != i || b != j){
                graph[a][b] = graph[i][j];
                graph[a][0] = graph[i][0];
            }
        }
    }

    num--;
    int stepen[num];
    int changes[num];
    for(int n=0; n<num; n++){
        stepen[n] = 0;
        changes[n] = n;
    }
    for(int i=0; i<num; i++){
        for(int j=0; j<num; j++){
            stepen[i] += graph[i][j+1];
            stepen[i] += graph[j][i+1];
        }
    }
    for(int i=0; i<num; i++){
        printf("Степень вершины '%c' -> '%d'\n", graph[i][0], stepen[i]);
    }
    
    for (int i=0; i<num-1; i++){
        for (int j=i+1; j<num; j++){
            if (stepen[i] < stepen[j]) {
                int temp1 = stepen[i];
                stepen[i] = stepen[j];
                stepen[j] = temp1;
                
                int temp2 = changes[i];
                changes[i] = changes[j];
                changes[j] = temp2;
            }
        }
    }
    
    char **graph2 = calloc(num, sizeof(char*));
    for(int i=0; i<num; i++){
        graph2[i] = calloc(num+1, sizeof(char));
    }
    
    for(int i=0; i<num; i++){
        graph2[i][0] = graph[ changes[i] ][0];
        for(int j=0; j<num; j++){
            graph2[i][j+1] = graph[ changes[i] ][ changes[j]+1 ];
        }
    }

    free(graph);
    return graph2;
}

int main(){
    printf("Введите кол-во вершин:\n> ");
    int num;
    scanf("%d",&num);
    if (num <= 0){
        printf("Отрицательное число...\n");
        return 9;
    }
    
    getchar();
    printf("Введите название каждой вершины по одной букве\n");
    char **graph = EnterGraph(num);
        
    graph = EnterGraphConn(num, graph);
    PrintGraph(num, graph);
    
    getchar(); //delete \n
    printf("Введите название вершины, которую вы хотите удалить\n");
    int V = -1;
    char A = 0;
    scanf("%c", &A);
    for(int i=0; i<num; i++){
        if(A == graph[i][0]){
            V = i;
            i = num;
        }
    }
    if(V == -1){
        printf("Такой вершины нет");
    }
    graph = SortGraph(graph, num, V);
    num--;
    
    PrintGraph(num, graph);

    OutputGraph(num, graph);
    
    return 0;
}
