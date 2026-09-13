#include "mathmania/source.h"
#include "mathmania/tokenarray.h"
#include "mathmania/program.h"
#include "mathmania/lexer.h"
#include "mathmania/parser.h"
#include "mathmania/eval.h"
#include "mathmania/memory.h"
#include <stdio.h>

static bool readFile(const char *path, char **out, size_t *outLength)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", path);
        return false;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fprintf(stderr, "Could not seek file: %s\n", path);
        fclose(file);
        return false;
    }

    long size = ftell(file);

    if (size < 0)
    {
        fprintf(stderr, "Could not determine file size: %s\n", path);
        fclose(file);
        return false;
    }

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        fprintf(stderr, "Could not seek file: %s\n", path);
        fclose(file);
        return false;
    }

    size_t length = (size_t)size;
    char *buffer;

    if (!Allocate(length + 1, &buffer))
    {
        fprintf(stderr, "Out of memory reading file: %s\n", path);
        fclose(file);
        return false;
    }

    size_t bytesRead = fread(buffer, 1, length, file);

    if (bytesRead != length)
    {
        fprintf(stderr, "Could not read entire file: %s\n", path);
        Release(&buffer);
        fclose(file);
        return false;
    }

    buffer[length] = '\0';

    if (fclose(file) != 0)
    {
        fprintf(stderr, "Error closing file: %s\n", path);
        Release(&buffer);
        return false;
    }

    *out = buffer;
    *outLength = length;

    return true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <script.mm>\n", argv[0]);
        return 1;
    }

    const char *path = argv[1];
    char *code;
    size_t length;

    if (!readFile(path, &code, &length))
        return 1;

    Source source = {path, code, length};

    TokenArray tokens;

    if (!TokenArrayInit(&tokens))
    {
        fprintf(stderr, "Out of memory initializing tokens\n");
        Release(&code);
        return 1;
    }

    if (!Lex(source, &tokens))
    {
        TokenArrayFree(&tokens);
        Release(&code);
        return 1;
    }

    Program program;

    if (!ProgramInit(&program))
    {
        fprintf(stderr, "Out of memory initializing program\n");
        TokenArrayFree(&tokens);
        Release(&code);
        return 1;
    }

    if (!Parse(source, &tokens, &program))
    {
        ProgramFree(&program);
        TokenArrayFree(&tokens);
        Release(&code);
        return 1;
    }

    if (!Eval(source, &program))
    {
        ProgramFree(&program);
        TokenArrayFree(&tokens);
        Release(&code);
        return 1;
    }

    ProgramFree(&program);
    TokenArrayFree(&tokens);
    Release(&code);

    return 0;
}
