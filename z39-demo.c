#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <yaz/zoom.h>

// written by Cyrus Gomes
// Other contributors: Vincent Scalfani and Avery Fernandez
// Jan 18, 2023
// MIT License, see repository details

// 4 external functions
// marc_create
// uses an input query and saves MARC raw into a output file

// marc_create_terminal
// uses default UA catalog, an input query and renders the MARC ASCII as stdout

// marc_create_server
// uses an input query w/ custom Z39.50 address and saves MARC raw into a output file

// marc_create_server_terminal
// uses an input query w/ custom Z39.50 address, renders MARC ASCII to stdout

// The following function outputs the MARC record in the 
// output file in binary with UA Libraries as default connection
// default server address and uses input query to retrieve results 
// and create an output marc file

void marc_create(char* query, int num, char* custom_f_name, int num_results){           
   char input[100]; // filename is 100 characters long (max)
   if (strcmp(custom_f_name, "\0") == 0){
    char input[100] = "output";
    char number[20];  // number of iteration is 20 charaters long. (e.g out1, out2, out3...)
    sprintf(number, "%d", num);    
    strcat(input, number);
    strcat(input, ".marc");
    
  }

  else{
    char input[100] = {};
    strcat(input,custom_f_name);
  }
  

  char *filename = input;   // for file name
  FILE *fp = fopen(filename, "w");    // for creating the custom file

  // setting up the connection with the server
  ZOOM_connection z = ZOOM_connection_new ("library.ua.edu:7090/voyager", 0); 
  ZOOM_resultset r;   // records the result set after retrieving it from the query
  const char *rec;    // a string that contains the individual marc record

  ZOOM_connection_option_set(z, "preferredRecordSyntax", "USMARC");

  r = ZOOM_connection_search_pqf(z, query);
    
  int nums = num_results;

  if (nums>0){
    if (nums >= ZOOM_resultset_size(r)){      //number of records in a single query
        nums = ZOOM_resultset_size(r);
    }

    else nums = nums;  
  }

  else {
    nums = ZOOM_resultset_size(r);
  }

  for (int i = 0; i < nums; i++){
      rec =  ZOOM_record_get (ZOOM_resultset_record (r, i), "raw", 0);   // records in a binary format
      fprintf(fp, "%s", rec);
    }
    fclose(fp);

}




// The following marc_create_terminal function outputs the queries using a default server address
// and updates the result to the terminal in MARC ASCII format

void marc_create_terminal(char* query, int num, int num_results){           

  ZOOM_connection z = ZOOM_connection_new ("library.ua.edu:7090/voyager", 0); // connection with the server
  ZOOM_resultset r;   // records the result set after retrieving it from the query
  const char *rec;    // a string that contains the individual marc record

  ZOOM_connection_option_set(z, "preferredRecordSyntax", "USMARC");
  r = ZOOM_connection_search_pqf(z, query);
  int nums = num_results;

  if (nums>0){
    if (nums >= ZOOM_resultset_size(r)){      //number of records in a single query
        nums = ZOOM_resultset_size(r);
    }

    else nums = nums;  
  }

  else {
    nums = ZOOM_resultset_size(r);
  }

  for (int i = 0; i < nums; i++){
      rec =  ZOOM_record_get (ZOOM_resultset_record (r, i), "render", 0);   // gets all the records in ASCII format
      printf("%s", rec);
    }

}

//The following marc_create_server function outputs the queries using a custom server address
// and updates the result to the custom marc file

void marc_create_server(char* query, int num, char* server, char* custom_f_name, int num_results){
  char input[100]; // filename is 100 characters long (max)
    
  if (strcmp(custom_f_name, "\0") == 0){
    char input[100] = "output";
    char number[20];  // number of iteration is 20 charaters long. (e.g out1, out2, out3...)
    sprintf(number, "%d", num);    
    strcat(input, number);
    strcat(input, ".marc");
  }

  else{
    char input[100] = {};
    strcat(input,custom_f_name);
  }

  char *filename = input;
  FILE *fp = fopen(filename, "w");

  ZOOM_connection z = ZOOM_connection_new (server, 0);
  ZOOM_resultset r;
  const char *rec;

  ZOOM_connection_option_set(z, "preferredRecordSyntax", "USMARC");

  r = ZOOM_connection_search_pqf(z, query);
  int nums = num_results;

  if (nums>0){
    if (nums >= ZOOM_resultset_size(r)){      //number of records in a single query
        nums = ZOOM_resultset_size(r);
    }

    else nums = nums;  
  }

  else {
    nums = ZOOM_resultset_size(r);
  }

  for (int i = 0; i < nums; i++){
      rec =  ZOOM_record_get (ZOOM_resultset_record (r, i), "raw", 0);   // gets all the records in a binary format
      fprintf(fp, "%s", rec);
    }
    
  fclose(fp);


}

//The following function outputs the marc record in the terminal in ASCII with custom server address

void marc_create_server_terminal(char* query, int num, char* server, int num_results){         
  char input[100] = "output";
  char number[20];
  sprintf(number, "%d", num);
  strcat(input, number);
  strcat(input, ".marc");

  ZOOM_connection z = ZOOM_connection_new (server, 0);
  ZOOM_resultset r;
  const char *rec;

  ZOOM_connection_option_set(z, "preferredRecordSyntax", "USMARC");
  r = ZOOM_connection_search_pqf(z, query);
  int nums = num_results;

  if (nums>0){
    if (nums >= ZOOM_resultset_size(r)){      //number of records in a single query
        nums = ZOOM_resultset_size(r);
    }

    else nums = nums;  
  }

  else {
    nums = ZOOM_resultset_size(r);
  }

  for (int i = 0; i < nums; i++){
      rec =  ZOOM_record_get (ZOOM_resultset_record (r, i), "render", 0);   // gets all the records in ASCII format
      printf("%s", rec);
    }

}

int main (int argc, char **argv){
  
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int num = 0;

  if (argc == 1){
      printf("See ./z39-demo -h \n");
      return 1;
    }  

  if (strcmp(argv[1],"-q") == 0){
                               
    if(argc == 3){ 
       
      marc_create_terminal(argv[2], num, 0);

    }

    else if (argc == 4 && strcmp(argv[3],"-n")==0){
      marc_create_terminal(argv[2], num, 0);
    }

    else if(argc == 5 && strcmp(argv[3],"-n") == 0){
      marc_create_terminal(argv[2], num, atoi(argv[4]));
    }

    else if (argc == 4 && strcmp(argv[3],"-o")==0){
      marc_create(argv[2], num, "\0", 0);
    }

    else if (argc == 5 && strcmp(argv[3],"-o")==0 && strcmp(argv[4],"-n") == 0){
      marc_create(argv[2], num, "\0", 0);
    }

    else if (argc == 6 && strcmp(argv[3],"-o")==0 && strcmp(argv[4],"-n") == 0){
      marc_create(argv[2], num, "\0", atoi(argv[5]));
    }

    else if (argc == 5 && strcmp(argv[3],"-o")==0){
      marc_create(argv[2], num, argv[4], 0);                  
    }

    else if (argc == 6 && strcmp(argv[3],"-o")==0 && strcmp(argv[5],"-n") == 0){
      marc_create(argv[2], num, argv[4], 0);
    }

    else if (argc == 7 && strcmp(argv[3],"-o")==0 && strcmp(argv[5],"-n") == 0){
      marc_create(argv[2], num, argv[4], atoi(argv[6]));
    }

    else {
      printf("Please seek help!\n");
      return 0;
    }
  }

  else if (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"-help") == 0){
        printf("usage: myprogram [-h] [-z] [-o] -q/-i FILE\n\n");
        printf("z39-demo is a command line program that can run Z39.50 searches via an input query or an input file\n\n");
        printf("positional arguments:\n");
        printf("\t -q           required query for Z39.50 search\n");
        printf("\t -i FILE      input file with queries for Z39.50 with one per line\n");
        printf("optional arguments\n");
        printf("\t -h, -help    show help and exit\n");
        printf("\t -z           optional custom Z39.50 adress; default is University of Alabama Libraries Catalog\n");
        printf("\t -o FILE      optional to output binary MARC file (FILE 100 chars max.); default is to print to stdout in MARC ASCII format \n");
        printf("\t -n number    optional to output specified number of results; default is to print the maximum number of results \n");
    }

  else if (strcmp(argv[1],"-i") == 0){     // we test to see if input file is being used

      fp = fopen(argv[2], "r");              // the specified input file is then opened

      if (fp == NULL){
        printf("Hello, something is wrong! :)");
        return 0;
      }

      if(argc == 3){
        while ((read = getline(&line, &len, fp)) != -1) {
        marc_create_terminal(line, num, 0);
        num++;
        sleep(5); // delay between queries

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
          }
        }
      }

      else if(argc == 4 && strcmp(argv[3], "-n")==0){
        while ((read = getline(&line, &len, fp)) != -1) {
        marc_create_terminal(line, num, 0);
        num++;
        sleep(5); // delay between queries

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
          }
        }
      }

      else if(argc == 5 && strcmp(argv[3], "-n")==0){
        while ((read = getline(&line, &len, fp)) != -1) {
        marc_create_terminal(line, num, atoi(argv[4]));
        num++;
        sleep(5); // delay between queries

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
          }
        }
      }

// continue work from here

      else if(argc == 4 && strcmp(argv[3],"-o")==0){
        while ((read = getline(&line, &len, fp)) != -1) {
        marc_create(line, num, "\0", 0);
        num++;
        sleep(5);

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
          }
        }
      }

      else if(argc == 5 && strcmp(argv[3],"-o")==0 && strcmp(argv[4], "-n")==0){
        while ((read = getline(&line, &len, fp)) != -1) {
        marc_create(line, num, "\0", 0);
        num++;
        sleep(5);

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
          }
        }
      }


      else if(argc == 6 && strcmp(argv[3],"-o")==0 && strcmp(argv[4], "-n")==0){
        while ((read = getline(&line, &len, fp)) != -1) {
        marc_create(line, num, "\0", atoi(argv[5]));
        num++;
        sleep(5);

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
          }
        }
      }


      fclose(fp);

      if (line)
        free(line);
  
      return 0;
    }

    //here on again

    else if (strcmp(argv[1],"-z") == 0){     //we test to see if the server is changed
        
      if (strcmp(argv[3],"-q") == 0){

        if(argc == 5){ 
       
          marc_create_server_terminal(argv[4], num, argv[2], 0);

        }

        else if (argc == 6 && strcmp(argv[5], "-n") == 0){
          marc_create_server_terminal(argv[4], num, argv[2], 0);
        }

        else if (argc == 7 && strcmp(argv[5], "-n") == 0){
          marc_create_server_terminal(argv[4], num, argv[2], atoi(argv[6]));
        }

        else if (argc == 6 && strcmp(argv[5],"-o")==0){
          marc_create_server(argv[4], num, argv[2], "\0", 0);
        }

        else if (argc == 7 && strcmp(argv[5],"-o")==0 && strcmp(argv[6], "-n")==0){
          marc_create_server(argv[4], num, argv[2], "\0", 0);
        }

        else if (argc == 8 && strcmp(argv[5],"-o")==0 && strcmp(argv[6], "-n")==0){
          marc_create_server(argv[4], num, argv[2], "\0", atoi(argv[7]));
        }

        else if (argc == 7 && strcmp(argv[5],"-o")==0){
          marc_create_server(argv[4], num, argv[2], argv[6], 0);
        }

        else if (argc == 8 && strcmp(argv[5],"-o")==0 && strcmp(argv[7], "-n") == 0){
          marc_create_server(argv[4], num, argv[2], argv[6], 0);
        }

        else if (argc == 9 && strcmp(argv[5],"-o")==0 && strcmp(argv[7], "-n") == 0){
          marc_create_server(argv[4], num, argv[2], argv[6], atoi(argv[8]));
        }

      } 

      else if (strcmp(argv[3],"-i") == 0){     //we test to see if input file is being used

        fp = fopen(argv[4], "r");

        if (fp == NULL){
          printf("Hello, something is wrong! :)");
          return 0;
        }
          
        if(argc == 5){
          while ((read = getline(&line, &len, fp)) != -1) {
          marc_create_server_terminal(line, num, argv[2], 0);
          num++;
          sleep(5);

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
            }
          }
        }

        else if(argc == 6 && strcmp(argv[5],"-n")==0){
          while ((read = getline(&line, &len, fp)) != -1) {
          marc_create_server_terminal(line, num, argv[2], 0);
          num++;
          sleep(5);

        if (num > 999){       //sets limit of the number of queries to 1000
          break;
            }
          }
        }

        else if(argc == 7 && strcmp(argv[5],"-n")==0){
          while ((read = getline(&line, &len, fp)) != -1) {
          marc_create_server_terminal(line, num, argv[2], atoi(argv[6]));
          num++;
          sleep(5);

          if (num > 999){       //sets limit of the number of queries to 1000
            break;
            }
          }
        }


        else if(argc == 6 && strcmp(argv[5],"-o")==0){
          while ((read = getline(&line, &len, fp)) != -1) {
          marc_create_server(line, num, argv[2], "\0", 0);
          num++;
          sleep(5);

          if (num > 999){       //sets limit of the number of queries to 1000
            break;
            }
          }
        }

        else if(argc == 7 && strcmp(argv[5],"-o")==0 && strcmp(argv[6], "-n") == 0){
          while ((read = getline(&line, &len, fp)) != -1) {
          marc_create_server(line, num, argv[2], "\0", 0);
          num++;
          sleep(5);

          if (num > 999){       //sets limit of the number of queries to 1000
            break;
            }
          }
        }

        else if(argc == 8 && strcmp(argv[5],"-o")==0 && strcmp(argv[6], "-n") == 0){
          while ((read = getline(&line, &len, fp)) != -1) {
          marc_create_server(line, num, argv[2], "\0", atoi(argv[7]));
          num++;
          sleep(5);

          if (num > 999){       //sets limit of the number of queries to 1000
            break;
            }
          }
        }



        fclose(fp);

        if (line)
          free(line);
  
        return 0;
      }

    }

}