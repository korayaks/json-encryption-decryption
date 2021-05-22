#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "jrb.h"
#include "fields.h"

typedef struct {
    char* word;
    char* huffmanCode;
} wordHuffmanCode;

wordHuffmanCode* huffmanString(wordHuffmanCode* p, char* tempFirst, char* tempSecound, int huffmanLetterCount);
wordHuffmanCode* wordString(wordHuffmanCode* p, char* tempFirst, char* tempSecound, int wordLetterCount);
wordHuffmanCode* addTreeEncode(JRB t, wordHuffmanCode* p, char* tempFirst, char* tempSecound, int huffmanLetterCount, int wordLetterCount, int nsize, int i, IS is);
wordHuffmanCode* addTreeDecode(JRB t, wordHuffmanCode* p, char* tempFirst, char* tempSecound, int huffmanLetterCount, int wordLetterCount, int nsize, int i, IS is);
wordHuffmanCode* controlAndPrintToFile(wordHuffmanCode* p,IS is,FILE* fpointer, JRB t, JRB tmp, int i, int control);
IS openFileIS(IS is, char* file);
FILE* openFileFILE(FILE* fpointer, char* file);

int main(int argc, char** argv)
{
    JRB t, tmp;
    IS is;
    FILE * fpointer;
    wordHuffmanCode* p;
    int nsize, i, control;
    int wordLetterCount = 0, huffmanLetterCount = 0;
    int controlCommand;
    char tempFirst[500];
    char tempSecound[500];
    //control to if argv[1] parameter equal to -d or -e
    if (strcmp(argv[1], "-d") == 0) {//if argv[1] parameter equal -d
        is=openFileIS(is,".kilit");//open .kilit file with new_inputstruct
        t = make_jrb();
        while (get_line(is) >= 0) {//add .kilit file line by line to tree to end of file
                addTreeDecode(t, p, tempFirst, tempSecound, huffmanLetterCount, wordLetterCount, nsize, i, is);//add huffmancode-word duo to tree (huffmancode->key, word->value)
        }
        is=openFileIS(is,argv[2]);//open argv[2] file with new_inputstruct, which means open giris_metin file
        fpointer = openFileFILE(fpointer,argv[3]);//open argv[3] file with fopen, which means open cikis_metin file
        while (get_line(is) >= 0) {
            controlAndPrintToFile(p,is,fpointer,t,tmp,i,control);//control ornek_metin file word by word, if words equal tree's key, print tree's value to cikis_metin. If not equal, print word direct to cikis_metin
        }
        fclose(fpointer);//close file.
    }
    else if (strcmp(argv[1], "-e") == 0) {//if argv[1] parameter equal -e
        is=openFileIS(is,".kilit");//open .kilit file new_inputstruct
        t = make_jrb();
        while (get_line(is) >= 0) {//add .kilit file line by line to tree to end of file
                addTreeEncode(t, p, tempFirst, tempSecound, huffmanLetterCount, wordLetterCount, nsize, i, is);//add huffmancode-word duo to tree (huffmancode->value, word->key) 
        }
        is=openFileIS(is,argv[2]);//open argv[2] file with new_inputstruct, which means open giris_metin file
        fpointer = openFileFILE(fpointer,argv[3]);//open argv[3] file with fopen, which means open cikis_metin file
        while (get_line(is) >= 0) {
            controlAndPrintToFile(p,is,fpointer,t,tmp,i,control);//control ornek_metin file word by word, if words equal tree's key, print tree's value to cikis_metin. If not equal, print word direct to cikis_metin
        }
        fclose(fpointer);//close file.
    }   
    else {
        printf("Missing or wrong command please try to use '-d' or '-e' for first parameter.\n"); exit(1);//if argv[1] parameter not equal -e or -d, exit program.
    }  
    return 0;
}

IS openFileIS(IS is,char* file)
{
    if (access(file, F_OK) == 0) {//file exist control 
        is = new_inputstruct(file);//open parameter "file"
    }
    else {
        printf("%s file does not exist\n",file); exit(1);//if file does not exist, exit program.
    }
    return is;
}
FILE* openFileFILE(FILE* fpointer, char* file)
{
    //there is no need control. Because if cikis_metin does not exist, it will create file and file name will be "file" parameter.
    fpointer = fopen(file,"w");//open cikis_metin file
    return fpointer;
}
wordHuffmanCode* controlAndPrintToFile(wordHuffmanCode* p,IS is,FILE* fpointer, JRB t, JRB tmp, int i, int control)
{
    for (i = 0; i < is->NF; i++) {
        control=0;
        jrb_traverse(tmp, t) {//This macro loops all tree. Macro is in jrb.h
            if(strcmp(is->fields[i],tmp->key.s) == 0){//compare string and tree's key. If two strings is equal, print tree's value to cikis_metin file
                fprintf(fpointer,"%s ",tmp->val.s);//print tree's value to cikis_metin file.
                control=1; 
            }
        }
        if(control==0){//if is->fields[i] not equals any tree's key, print is->fields[i] direct to cikis_metin file
            fprintf(fpointer,"%s ",is->fields[i]);//print is->fields[i] direct to cikis_metin file
        }        
    }
    return p;
}
wordHuffmanCode* huffmanString(wordHuffmanCode* p, char* tempFirst, char* tempSecound, int huffmanLetterCount)
{
    for (int i = 0, j = 0; i < strlen(tempFirst); i++) {
        if (tempFirst[i] == 48 || tempFirst[i] == 49) //Huffman code is only get 1 or 0. i get only 1 or 0 from string
        {
            tempSecound[j] = tempFirst[i];
            j++;
            huffmanLetterCount++;
        }
    }
    tempSecound[huffmanLetterCount] = '\0';//define end of the huffman code string
    huffmanLetterCount = 0;
    strcpy(p->huffmanCode, tempSecound);//put the huffman code to p->huffmanCode
    return p;
}
wordHuffmanCode* wordString(wordHuffmanCode* p, char* tempFirst, char* tempSecound, int wordLetterCount)
{
    for (int i = 0, j = 0; i < strlen(tempFirst); i++)
    {
        if (i != 0 && i != strlen(tempFirst) - 1 && i != strlen(tempFirst) - 2)//remove first and last two letter from string
        {
            tempSecound[j] = tempFirst[i];
            j++;
            wordLetterCount++;
        }
    }
    tempSecound[wordLetterCount] = '\0';//define end of the word string
    wordLetterCount = 0;
    strcpy(p->word, tempSecound);//put the word to p->word
    return p;
}
wordHuffmanCode* addTreeEncode(JRB t, wordHuffmanCode* p, char* tempFirst, char* tempSecound, int huffmanLetterCount, int wordLetterCount, int nsize, int i, IS is)
{
    p = malloc(sizeof(wordHuffmanCode));
    nsize = strlen(is->fields[is->NF - 1]);
    for (i = 1; i < is->NF - 1; i++) nsize += (strlen(is->fields[i]) + 1);

    p->huffmanCode = (char*)malloc(sizeof(char) * (nsize + 1));
    strcpy(tempFirst, is->fields[is->NF - 1]);//copy huffman code string in field to the tempFirst --> huffman code string like ("10001",)
    huffmanString(p, tempFirst, tempSecound, huffmanLetterCount);//remove ( " and , ) character from huffman code string and put this string to the p->huffmanCode

    nsize = strlen(is->fields[0]);
    for (i = 1; i < is->NF - 1; i++) nsize += (strlen(is->fields[i]) + 1);

    p->word = (char*)malloc(sizeof(char) * (nsize + 1));
    strcpy(tempFirst, is->fields[0]);//copy word string in field to the tempFirst --> word string like ("available,":)
    wordString(p, tempFirst, tempSecound, wordLetterCount);//remove ( " and : ) character from word string and put this string to the p->word
    jrb_insert_str(t, p->word, new_jval_s(p->huffmanCode));//insert key and val to red black tree. key -> word, val -> huffmanCode.
    return p;
}
wordHuffmanCode* addTreeDecode(JRB t, wordHuffmanCode* p, char* tempFirst, char* tempSecound, int huffmanLetterCount, int wordLetterCount, int nsize, int i, IS is)
{
    p = malloc(sizeof(wordHuffmanCode));
    nsize = strlen(is->fields[is->NF - 1]);
    for (i = 1; i < is->NF - 1; i++) nsize += (strlen(is->fields[i]) + 1);

    p->huffmanCode = (char*)malloc(sizeof(char) * (nsize + 1));
    strcpy(tempFirst, is->fields[is->NF - 1]);//copy huffman code string in field to the tempFirst --> huffman code string like ("10001",)
    huffmanString(p, tempFirst, tempSecound, huffmanLetterCount);//remove ( " and , ) character from huffman code string and put this string to the p->huffmanCode

    nsize = strlen(is->fields[0]);
    for (i = 1; i < is->NF - 1; i++) nsize += (strlen(is->fields[i]) + 1);

    p->word = (char*)malloc(sizeof(char) * (nsize + 1));
    strcpy(tempFirst, is->fields[0]);//copy word string in field to the tempFirst --> word string like ("available,":)
    wordString(p, tempFirst, tempSecound, wordLetterCount);//remove ( " and : ) character from word string and put this string to the p->word
    jrb_insert_str(t, p->huffmanCode, new_jval_s(p->word));//insert key and val to red black tree. key -> huffmanCode, val-> word.
    return p;
}
