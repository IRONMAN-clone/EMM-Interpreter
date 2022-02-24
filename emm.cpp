#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

vector<string> Tokenize(string & expression, unordered_map<string, int> & opMap)
{
    vector<string> tokens = {};
    string temp = "";
    for(char c : expression)
    {
        if( c == ' ') continue;
        else if( opMap.find(string(1, c)) != opMap.end())
        {
            if(temp != "")
                tokens.push_back(temp);
            temp = "";
            tokens.push_back(string(1, c));
        }
        else temp += c;
    }
    if(temp != "")
        tokens.push_back(temp);

    return tokens;
}

vector<string> InfixToPostfix(vector<string> & infix, unordered_map <string, int> & opMap)
{
    vector<string> postfix = {};
    vector<string> stack = { "("};
    infix.push_back(")");
    for(string token : infix)
    {
        //cout << token << endl;
        if(opMap.find(token) == opMap.end()) // oparand
            postfix.push_back(token);
        else if( token == "(")
            stack.push_back(token);
        else if (token == ")")
        {
            while(stack.back() != "(")
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        }
        else // operator
        {
            while(stack.size() > 0 && opMap[token] <= opMap[stack.back()])
            {
                postfix.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(token);
        }
    }
    
    return postfix;
}

float CalculatePostfix(vector<string> & postfix, unordered_map<string, int> &opMap)
{
    vector<float> evaluationStack = {};
    for(string token : postfix)
    {
        if(opMap.find(token) != opMap.end())
        {
            float n1 = evaluationStack[evaluationStack.size() - 1]; 
            float n2 = evaluationStack[evaluationStack.size() - 2];

            switch(token[0])
            {
                case '+':
                    n2 = n2 + n1;
                    break;
                case '-':
                    n2 = n2 - n1;
                    break;
                case '*':
                    n2 = n2 * n1;
                    break;
                case '/':
                    n2 = n2 / n1;
                    break;
            }
            evaluationStack.pop_back();
            evaluationStack.pop_back();
            evaluationStack.push_back(n2);
        }
        else
        {
            evaluationStack.push_back(atof(token.c_str()));
        }
    }
    return evaluationStack[0];
}

//Important Macros 
#define CAP 256*2
#define BOOL int
#define TRUE 1
#define FALSE 0

//<DOC> PrintLn: Statement Prints a new line <DOC>// 


//<TODO> Make new line characters and that type things <TODO>//

int main(int ARGC,char **ARGV)
{
	// PostFix Calculation Eval 
	unordered_map<string, int> opMap = {
        { "*", 2},
        { "/", 2},
        { "+", 1},
        { "-", 1},
        { "(", -1},
        { ")", -1},
    };

	//Program Lines
	int line_passed = 1;

	//Error Codes 
	int no_initializer = 1;
	int no_str_coat_butstr = 2;
	int no_end = 3;
	int def_case_marks = 4;
	int num_inst_op = 5;
	// Storage, External 
	char storage[CAP];
	char* ext = NULL;

	//Sizeof Usage -
	//SIZEOF <type> (TODO: hold)
	// Statement 
	//<DOC>
	//Hold Usage - hold[int] i_Var:SIZEOF <int>
	//Pushes 'i_Var' string to Variable stack 
	//Pushes after : to [...] stack
	//<DOC>

	const char* EMM_STT_PRINT = "puts"; // Not accepts variable names
	const char* EMM_EVAL_TOK = "eval";
	const char* EMM_S_O = "SIZEOF";
	const char* EMM_HOLD = "hold"; // could be printed with puts: "..." + hold
	// Token 
	const char STT_INIT = ':'; //Initializer (better check)
	const char STR_LIT = '\"';

	const char OP_PLUS = '+';
	const char OP_MIN = '-';
	const char OP_MUL = '*';
	const char OP_DIV = '/';
	const char OP_MOD = '%';

	// Basic Interpreter Error
	if(ARGC < 2){
		printf("Expected atleast 3 arguments, got %d arguments\n",ARGC);
	}
	if(ARGV[1] == NULL){
		printf("ERROR: expected valid file got \"%s\"\n",ARGV[1]);
	}

	// File opening
	FILE *fp;
	fp = fopen(ARGV[1], "r");
	if(!fp){
		printf("Can't open \"%s\"\n",ARGV[1]);
	}

	// File Checking
	while(fgets(storage,CAP,fp)){
		ext = (char*)malloc(sizeof(char)*CAP);
		for(int i = 0; i < CAP; i++){
			ext[i] = storage[i];

			// PrintLn Statement
			if(strcmp(ext,EMM_STT_PRINT)==0){
				// Extra Check Variables
				int Fin2;
				int Fin1;
				int break1;
				char initializer_check[] = {'\0'};
				int t = 0;
				BOOL has_init = FALSE;
				BOOL str_in_coat = FALSE;
				BOOL has_end = FALSE;
				
				// Error Checking
				for(int TMP=i+1;TMP<CAP;TMP++){
					initializer_check[t] = storage[TMP];
					t += 1;
					if(initializer_check[t] == STT_INIT){
						has_init = TRUE;
						break;
					}
				}
				if(has_init == FALSE){
					printf("\nError: expect Token <%c>\tLine No. <%d>\nError: Code <%d>",STT_INIT,line_passed,no_initializer);
					break;
				}
				for(int Fin1 = t;Fin1 < CAP; Fin1++){
					if(storage[Fin1] == STR_LIT){
						break1 = Fin1;
						str_in_coat = TRUE;
						for(Fin2 = Fin1+1;Fin2 < CAP;Fin2++){
							if(storage[Fin2+1] == STR_LIT){	
								has_end = TRUE;
								break;
							}
						}
						break;
					}
				}


				if(has_end == FALSE){
					printf("\nError: expect Token <%c>\tLine No. <%d>\nError: Code <%d>",STR_LIT,line_passed,no_end);
					break;
				}

				if(str_in_coat == FALSE){
					printf("\nError: expect Token <%c>\tLine No. <%d>\nError: Code <%d>",STR_LIT,line_passed,no_str_coat_butstr);
					break;
				}

				
				for(int ffx=break1;ffx<CAP;ffx++){
					if(storage[ffx] == '\\'){
						switch(storage[ffx+1]){
							case 'n':
								storage[ffx] = '\n';
								storage[ffx+1] = '\b';
								break;
							case 't':
								storage[ffx] = '\b';
								storage[ffx+1] = '\t';	  
								break; 
							case 'b':
								storage[ffx] = '\b';
								storage[ffx+1] = '\b';
		
								break; 
							case '0':
								storage[ffx] = '\b';
								storage[ffx+1] = '\0';
								break;
						}
					}
				}
				

				// Need to fix cases like
				/*
				PrintLn: "HELLO" <- from this
				PrintLn: Hello" this line is getting printed and 
				PrintLn: HellO" 
				*/

				if(has_end == TRUE && str_in_coat == TRUE){
					for(int jx=break1+1;jx<CAP;jx++){
						printf("%c",storage[jx]);
						if(storage[jx+1] == STR_LIT){
							break;
						}
					}
				}
			
			}

			// Evaluator Expression
			else if(strcmp(ext,EMM_EVAL_TOK)==0){
				BOOL has_init = FALSE;
				int breakC = 0;
				int exp_c = 0;
				for(int ix = i+1;ix < 500; ix++){
					if(storage[ix] == ':'){
						has_init = TRUE;
						breakC = ix+1;
						break;
					}
				}
				if(has_init == FALSE){
					printf("\nError: expect Token <%c>\tLine No. <%d>\nError: Code <%d>",STT_INIT,line_passed,no_initializer);
				}
				char *setX = (char*)malloc(sizeof(char)*CAP);
				if(has_init == TRUE){
					for(int IXX = breakC;IXX < CAP; IXX++){
						//exp[exp_c] = storage[IXX];
						setX[exp_c] = storage[IXX];
						exp_c += 1;
						if(storage[IXX+1] == '\0'){
							string exp(setX); // load the exp 
							vector<string> tokens = Tokenize(exp, opMap); //infix : a * b
    						tokens = InfixToPostfix(tokens, opMap); // postfix : a b *
    						float result = CalculatePostfix(tokens, opMap);
    						cout << result;
							break;
						}
					}
				}		
			}

				/*
				for(int Fin = t;Fin < CAP; Fin++){
					printf("%c",storage[Fin]);
					if(storage[Fin+1] == '\0'){
						break;
					}
				}
				*/
			// PrintLn statement end
		}
		line_passed++;
	}

	fclose(fp); 
	return 0; 
}