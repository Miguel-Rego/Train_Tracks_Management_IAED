
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_carr 201
#define MAX_stops 10001
#define MAX_connections 30001
#define MAX_nome_paragem 51
#define MAX_nome_carreira 21

typedef struct {
    char line_name[MAX_nome_carreira];
    char line_stops[MAX_stops][MAX_nome_paragem];
    int numb_stops;
    double cost_per_stop[MAX_stops - 1];
    double conn_duration[MAX_stops - 1];
} Carreira;

typedef struct {
    char stop_name[MAX_stops];
    double latitude;
    double longitude;
} Paragem;

/* Variáveis Globais */

int num_lines = 0;
Carreira v_lines[MAX_carr];

int num_stops = 0;
Paragem v_stops[MAX_stops];


void print_train_lines() {
    int i;
    int j;
    for (i = 0; i < num_lines; i++) {
        if (v_lines[i].numb_stops >= 2){
            double total_cost = 0.0;
            double total_duration = 0.0;
            for (j = 0; j < v_lines[i].numb_stops + 1; j++){
                total_cost += v_lines[i].cost_per_stop[j];
                total_duration += v_lines[i].conn_duration[j];

            }
            printf("%s ",v_lines[i].line_name);
            printf("%s ",v_lines[i].line_stops[0]);
            printf("%s ",v_lines[i].line_stops[v_lines[i].numb_stops - 1]);
            printf("%d ",v_lines[i].numb_stops);
            printf("%.2f ",total_cost);
            printf("%.2f\n",total_duration);
        }
        else{
            printf("%s ", v_lines[i].line_name);
            printf("0 ");
            printf("0.00 ");
            printf("0.00\n");
        }
    }
}

void add_train_line(){
    int i;
    int j;
    char invert[8];
    Carreira nova;
    char input[BUFSIZ];
    char arg1[MAX_nome_paragem];
    int num_args;
    fgets(input, BUFSIZ, stdin);
    if (input[1] == '\"') {
        num_args = sscanf(input," \"%[^\"]\" %s", arg1, invert);
    }
    else {
        num_args = sscanf(input, " %s %s", arg1, invert);
    }
    if(num_args == -1){
        print_train_lines();
        return;
    }
    strcpy(nova.line_name, arg1);
    if(num_args == 1){
        for(i = 0; i < num_lines; i++){
            if(strcmp(nova.line_name, v_lines[i].line_name) == 0){
                for(j = 0; j < v_lines[i].numb_stops; j++){
                    printf("%s", v_lines[i].line_stops[j]);
                    if(j != v_lines[i].numb_stops - 1)
                        printf(", ");
                    else printf("\n");
                }
                return;
            }
        }
        strcpy(v_lines[num_lines].line_name, nova.line_name);
        v_lines[num_lines].numb_stops = 0;
        num_lines += 1;
        return;
    }

    if(num_args == 2 && strlen(invert) >= 3 && ((strstr(invert, "inverso") != NULL ||
                                                 (strstr(invert, "invers") != NULL && strlen(invert) == 6) ||
                                                 (strstr(invert, "inver") != NULL && strlen(invert) == 5) ||
                                                 (strstr(invert, "inve") != NULL && strlen(invert) == 4) ||
                                                 (strstr(invert, "inv") != NULL && strlen(invert) == 3)))) {
        for (i = 0; i < num_lines; i++) {
            if (strcmp(nova.line_name, v_lines[i].line_name) == 0) {
                for (j = v_lines[i].numb_stops - 1; j >= 0; j--) {
                    printf("%s", v_lines[i].line_stops[j]);
                    if(j != 0)
                        printf(", ");
                }
                printf("\n");
                return;
            }
        }
    }
    else{
        printf("incorrect sort option.\n");
    }
}

void print_train_stops(){
    int i;
    int j;
    int k;
    int cont[10001];
    cont[num_stops + 1] = '\0';
    for(i = 0; i < num_stops; i++) {
        for (j = 0; j < num_lines; j++) {
            for (k = 0; k < num_stops; k++) {
                if (strcmp(v_stops[i].stop_name, v_lines[j].line_stops[k]) == 0)
                    cont[i]++;
            }
        }
    }
    for (i = 0; i < num_stops; i++) {
        printf("%s: ",v_stops[i].stop_name);
        printf("%16.12f ",v_stops[i].latitude);
        printf("%16.12f ",v_stops[i].longitude);
        printf("%d\n", cont[i]);
    }

}

int count_double_quotes(char *str) {
    int count = 0;
    char *ptr = str;

    while (*ptr != '\0') {
        if (*ptr == '\"') {
            count++;
        }
        ptr++;
    }
    return count;
}

void add_stop(){
    int i;
    int j;
    char input[BUFSIZ];
    char arg1[MAX_nome_paragem];
    double arg2, arg3;
    int num_args;
    Paragem nova;
    fgets(input, BUFSIZ, stdin);
    if (input[1] == '\"') {
        num_args = sscanf(input," \"%[^\"]\" %lf %lf", arg1, &arg2, &arg3);
    }
    else {
        num_args = sscanf(input, " %s %lf %lf", arg1, &arg2, &arg3);
    }
    strcpy(nova.stop_name, arg1);
    if(num_args == 3) {
        nova.latitude = arg2;
        nova.longitude = arg3;
        for (i = 0; i < num_stops; i++) {
            if (strcmp(nova.stop_name, v_stops[i].stop_name) == 0) {
                printf("%s: stop already exists\n", nova.stop_name);
                return;
            }
        }
    }
    if(num_args == 3){
        strcpy(v_stops[num_stops].stop_name, nova.stop_name);
        v_stops[num_stops].latitude = nova.latitude;
        v_stops[num_stops].longitude = nova.longitude;
        num_stops += 1;
        return;
    }
    else if (num_args == 1){
        for(j = 0; j < num_stops; j++){
            if(strcmp(v_stops[j].stop_name, arg1) == 0){
                printf("%16.12f %16.12f\n", v_stops[j].latitude, v_stops[j].longitude);
                return;
            }
        }
        printf("%s: no such stop.\n", nova.stop_name);
        return;
    }
    else if (num_args == -1){
        print_train_stops();
        return;
    }
}


void shift_rows(char str[][MAX_nome_paragem], int rows) {
    int i, j;
    for (i = rows - 1; i >= 0; i--) {
        if (str[i][0] != '\0') {
            for (j = MAX_nome_paragem; j >= 0; j--) {
                str[i + 1][j] = str[i][j];
            }
        }
    }
}


void add_connection(){
    int index = -1;
    int stop_test = -2;
    int stop_test_2 = -3;
    int i;
    int j;
    int k;
    char input[BUFSIZ];
    char LineGiven[MAX_nome_paragem];
    char InitialStopGiven[MAX_nome_paragem];
    char FinalStopGiven[MAX_nome_paragem];
    double cost_per_stop_given;
    double conn_duration_given;
    int num_args;
    fgets(input, BUFSIZ, stdin);
    if(count_double_quotes(input) == 4) {
        if(sscanf(input," \"%[^\"]\" %s \"%[^\"]\" %lf %lf", LineGiven, InitialStopGiven, FinalStopGiven,
                  &cost_per_stop_given, &conn_duration_given) == 5)
            num_args = 5;
        else if(sscanf(input," %s \"%[^\"]\" \"%[^\"]\" %lf %lf", LineGiven, InitialStopGiven, FinalStopGiven,
                       &cost_per_stop_given, &conn_duration_given) == 5)
            num_args = 5;
        else if(sscanf(input," \"%[^\"]\" \"%[^\"]\" %s %lf %lf", LineGiven, InitialStopGiven,
                       FinalStopGiven,&cost_per_stop_given, &conn_duration_given) == 5)
            num_args = 5;
    }
    if(count_double_quotes(input) == 2){
        if(sscanf(input," \"%[^\"]\" %s %s %lf %lf", LineGiven, InitialStopGiven, FinalStopGiven,
                  &cost_per_stop_given, &conn_duration_given) == 5)
            num_args = 5;
        else if(sscanf(input," %s \"%[^\"]\" %s %lf %lf", LineGiven, InitialStopGiven, FinalStopGiven,
                       &cost_per_stop_given, &conn_duration_given) == 5)
            num_args = 5;
        else if(sscanf(input," %s %s \"%[^\"]\" %lf %lf", LineGiven, InitialStopGiven,
                       FinalStopGiven,&cost_per_stop_given, &conn_duration_given) == 5)
            num_args = 5;
    }

    if(count_double_quotes(input) == 0){
        num_args = sscanf(input," %s %s %s %lf %lf", LineGiven, InitialStopGiven,
                          FinalStopGiven,&cost_per_stop_given, &conn_duration_given);

    }
    if(num_args != 5){
        return;
    }
    if(cost_per_stop_given < 0 || conn_duration_given < 0) {
        printf("negative cost or duration.\n");
        return;
    }
    for(j = 0; j < num_lines; j++){
        if(strcmp(LineGiven, v_lines[j].line_name) == 0){
            index = j;
            break;
        }
    }
    if(index == -1) {
        printf("%s: no such line.\n", LineGiven);
        return;
    }
    for(k = 0; k < num_stops; k++){
        if(strcmp(InitialStopGiven, v_stops[k].stop_name) == 0){
            stop_test = 2;
            break;
        }
    }
    if(stop_test != 2){
        printf("%s: no such stop.\n", InitialStopGiven);
        return;
    }
    for(k = 0; k < num_stops; k++){
        if(strcmp(FinalStopGiven, v_stops[k].stop_name) == 0){
            stop_test_2 = 3;
            break;
        }
    }
    if(stop_test_2 != 3){
        printf("%s: no such stop.\n", FinalStopGiven);
        return;
    }

    if(v_lines[index].numb_stops == 0 && num_args == 5){
        for (i = 0; i < MAX_stops; i++)
            v_lines[index].line_stops[i][0] = '\0';
        strcpy(v_lines[index].line_stops[v_lines[index].numb_stops], InitialStopGiven);
        v_lines[index].line_stops[v_lines[index].numb_stops][strlen(InitialStopGiven)] = '\0';
        v_lines[index].numb_stops++;
        strcpy(v_lines[index].line_stops[v_lines[index].numb_stops], FinalStopGiven);
        v_lines[index].line_stops[v_lines[index].numb_stops][strlen(FinalStopGiven)] = '\0';
        v_lines[index].numb_stops++;
        v_lines[index].cost_per_stop[0] = cost_per_stop_given;
        v_lines[index].conn_duration[0] = conn_duration_given;
    } else {
        if(strcmp(InitialStopGiven, v_lines[index].line_stops[v_lines[index].numb_stops - 1]) == 0) {
            strcpy(v_lines[index].line_stops[v_lines[index].numb_stops], FinalStopGiven);
            v_lines[index].line_stops[v_lines[index].numb_stops][strlen(FinalStopGiven)] = '\0';
            v_lines[index].numb_stops++;
            v_lines[index].conn_duration[v_lines[index].numb_stops] = conn_duration_given;
            v_lines[index].cost_per_stop[v_lines[index].numb_stops] = cost_per_stop_given;
        }
        else if(strcmp(FinalStopGiven, v_lines[index].line_stops[0]) == 0) {
            shift_rows(v_lines[index].line_stops, v_lines[index].numb_stops);
            strcpy(v_lines[index].line_stops[0], InitialStopGiven);
            v_lines[index].line_stops[0][strlen(InitialStopGiven)] = '\0';
            v_lines[index].numb_stops++;
            for (i = v_lines[index].numb_stops; i > 0; i--) {
                v_lines[index].conn_duration[i] = v_lines[index].conn_duration[i - 1];
                v_lines[index].cost_per_stop[i] = v_lines[index].cost_per_stop[i - 1];
            }
            v_lines[index].conn_duration[0] = conn_duration_given;
            v_lines[index].cost_per_stop[0] = cost_per_stop_given;

        }
        else if(strcmp(InitialStopGiven, v_lines[index].line_stops[v_lines[index].numb_stops-1]) == 0 &&
                strcmp(FinalStopGiven, v_lines[index].line_stops[0]) == 0){
            strcpy(v_lines[index].line_stops[v_lines[index].numb_stops], FinalStopGiven);
            v_lines[index].line_stops[v_lines[index].numb_stops][strlen(FinalStopGiven)] = '\0';
            v_lines[index].numb_stops++;
            v_lines[index].conn_duration[v_lines[index].numb_stops] = conn_duration_given;
            v_lines[index].cost_per_stop[v_lines[index].numb_stops] = cost_per_stop_given;
        }
        else printf("link cannot be associated with bus line.\n");
    }
}

void sort_rows(char arr[][MAX_nome_carreira], int n) {
    int i, j;
    char temp[MAX_nome_carreira];

    for(i = 0; i < n-1; i++) {
        for(j = i+1; j < n; j++) {
            if(strcmp(arr[i], arr[j]) > 0) {
                strcpy(temp, arr[i]);
                strcpy(arr[i], arr[j]);
                strcpy(arr[j], temp);
            }
        }
    }
}


void stop_intersections(){
    int i;
    int j;
    int k;
    int l = 0;
    int f;
    int g = 0;
    int h = 0;
    int m = 0;
    int n = 0;
    int cont[MAX_stops];
    char intersect_lines[MAX_connections][MAX_nome_carreira];
    char intersect_lines_stop[MAX_connections][MAX_nome_carreira];
    cont[num_stops + 1] = '\0';
    memset(cont, 0, num_stops);
    for(i = 0; i < num_stops; i++){
        for(j = 0; j < num_lines - 1; j++){
            for(k = 0; k < v_lines[j].numb_stops; k++){
                if(strcmp(v_stops[i].stop_name, v_lines[j].line_stops[k]) == 0){
                    cont[i]++;
                    strcpy(intersect_lines[l], v_lines[j].line_name);
                    l++;

                }
            }
        }
    }
    for(f = 0; f <= num_stops; f++){
        h = g;
        m = 0;
        n = 0;
        if(cont[f] > 1){
            printf("%s %d: ", v_stops[f].stop_name, cont[f]);
            while(g < cont[f] + h) {
                strcpy(intersect_lines_stop[m], intersect_lines[g]);
                g++;
                m++;
            }
            sort_rows(intersect_lines_stop, m);
            while(n < m){
                printf("%s ", intersect_lines_stop[n]);
                n++;
            }
            memset(intersect_lines_stop, '\0',
                   sizeof(intersect_lines_stop[0][0]) * MAX_connections * MAX_nome_carreira);
            printf("\n");
        }
        else g++;
    }
}

int main(){
    char c;

    while((c = getchar()) != EOF){
        switch(c) {
            case 'q':
                return 0;
            case 'c':
                add_train_line();
                break;
            case 'p':
                add_stop();
                break;
            case 'l':
                add_connection();
                break;
            case 'i':
                stop_intersections();
                break;

        }
    }
    return 0;
}
