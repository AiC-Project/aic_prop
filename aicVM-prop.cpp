#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

char *getPropFile(char *propname)
{
    FILE *f_prop;
    char f_name[129];
    char *value;

    snprintf(f_name, 128, "/data/.aicVM_%s", propname);
    f_prop = fopen(f_name, "r");
    if (!f_prop)
        return NULL;
    value = (char *)malloc(1024*sizeof(char));
    if (!fgets(value, 1024, f_prop)) {
        fclose(f_prop);
        return NULL;
    }
    fclose(f_prop);
    return value;
}

int setPropFile(char *propname, char *value)
{
    FILE *f_prop;
    char f_name[129];

    snprintf(f_name, 128, "/data/.aicVM_%s", propname);
    if (!value || !strlen(value)) {
        unlink(f_name);
        return 0;
    }

    f_prop = fopen(f_name, "w");
    if (!f_prop)
        return -1;
    fputs(value, f_prop);
    fclose(f_prop);
    return 0;
}

void print_usage(char *pname) 
{
    fprintf(stderr, "Usage: %s get [property_name]\n       %s set [property_name] [property_value]\n       %s rm [property_name]", pname, pname, pname);
}

int main(int argc, char *argv[]) 
{
    int clientID;

    if (argc<3) {
        print_usage(argv[0]);
        return -1;
    }

    if (strcmp(argv[1],"get")==0) {
        char *val;

        val = getPropFile(argv[2]);
        if (!val)
            return -1;
        printf("%s",val);
        free(val);
    }
    else if (strcmp(argv[1],"set")==0) {
        if (argc<4) {
            print_usage(argv[0]);
            return -1;
        }
        if (setPropFile(argv[2], argv[3])<0)
            return -1;
    }
    else if (strcmp(argv[1],"rm")==0) {
        if (setPropFile(argv[2], "")<0)
            return -1;
    }
    else {
        print_usage(argv[0]);
        return -1;
    }

    return 0;
}
