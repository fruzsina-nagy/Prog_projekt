#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === Forráskód minták ===

const char* c_kod =
    "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "\n"
    "int main()\n"
    "{\n"
    "\tprintf(\"Hello World!\\n\");\n"
    "\n"
    "\treturn 0;\n"
    "}\n";

const char* java_kod =
    "public class Main {\n"
    "\tpublic static void main(String[] args) {\n"
    "\t\tSystem.out.println(\"Hello World\");\n"
    "\t}\n"
    "}\n";

const char* bash_kod =
    "#!/bin/bash\n"
    "echo 'Hello, World!'\n";

const char* python_kod =
    "def main():\n"
    "\tprint(\"Hello World!\")\n"
    "\n"
    "if __name__ == \"__main__\":\n"
    "\tmain()\n";

const char* js_kod =
    "console.log(\"Hello, World!\");\n";

// === Struktúra a beállításokhoz ===

typedef struct {
    const char* nyelv;
    const char* fajlnev;
    const char* kod;
    int stdout_flag;
} IrasBeallitasok;

// === Segédfüggvények ===

void help_menu()        // help menü összetétele
{
    puts("Használat: alap <nyelv> [opciók]\n");
    puts("Parancsok:\n");
    puts("-h    --help");
    puts("-v    --version");
    puts("      --stdout\n");
    puts("Elérhető minták:\n");
    puts("* c      - C forráskód [main.c]");
    puts("* java   - Java forráskód [Main.java]");
    puts("* sh     - Bash forráskód [main.sh]");
    puts("* python - Python forráskód [main.py]");
    puts("* js     - JavaScript forráskód [main.js]");
}

int van_e(const char* nev)      // létező fájl felülírása ellen
{
    FILE* keres = fopen(nev, "r");
    if (keres != NULL) 
    {
        fclose(keres);
        return 1;
    }
    return 0;
}

void kiiratas(const IrasBeallitasok* b)     // kiíratást ellenőriz (fájlba vagy stdout)
{
    if (b->stdout_flag) 
    {
        fputs(b->kod, stdout);
    } 
    else 
    {
        if (van_e(b->fajlnev)) 
        {
            fprintf(stderr, "Hiba: a %s fájl már létezik.\n", b->fajlnev);
            exit(1);
        }

        FILE* fp = fopen(b->fajlnev, "w");
        if (!fp) 
        {
            perror("Fájl írási hiba");
            exit(1);
        }

        fputs(b->kod, fp);
        fclose(fp);
        printf("A %s fájl elkészült.\n", b->fajlnev);
    }
}

int nyelvek(const char* nyelv)      // támogatott nyelvek
{
    return strcmp(nyelv, "c") == 0 || strcmp(nyelv, "java") == 0 ||
           strcmp(nyelv, "sh") == 0 || strcmp(nyelv, "python") == 0 ||
           strcmp(nyelv, "js") == 0;
}

IrasBeallitasok beallitasok_letrehoz(const char* nyelv, int stdout_flag)        // megadottak alapján hoz létre
{
    IrasBeallitasok b;
    b.nyelv = nyelv;
    b.stdout_flag = stdout_flag;

    if (strcmp(nyelv, "c") == 0) 
    {
        b.fajlnev = "main.c";
        b.kod = c_kod;
    } 
    else if (strcmp(nyelv, "java") == 0) 
    {
        b.fajlnev = "Main.java";
        b.kod = java_kod;
    } 
    else if (strcmp(nyelv, "sh") == 0) 
    {
        b.fajlnev = "main.sh";
        b.kod = bash_kod;
    } 
    else if (strcmp(nyelv, "python") == 0) 
    {
        b.fajlnev = "main.py";
        b.kod = python_kod;
    } 
    else if (strcmp(nyelv, "js") == 0) 
    {
        b.fajlnev = "main.js";
        b.kod = js_kod;
    } 
    else 
    {
        b.fajlnev = NULL;
        b.kod = NULL;
    }

    return b;
}

int main(int argc, char* argv[]) 
{
    if (argc == 1) 
    {
        help_menu();
        return 0;
    }

    if (argc >= 2) 
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) 
        {
            help_menu();
            return 0;
        }
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) 
        {
            printf("Verziószám: 1.5.2\n");
            return 0;
        }

        if (nyelvek(argv[1])) 
        {
            int stdout_flag = (argc == 3 && strcmp(argv[2], "--stdout") == 0);
            IrasBeallitasok b = beallitasok_letrehoz(argv[1], stdout_flag);
            if (b.kod != NULL) 
            {
                kiiratas(&b);
                return 0;
            }
        }
    }

    // Ha ide jutunk, akkor valami nem stimmelt a megadásokkal
    help_menu();
    return 0;
}