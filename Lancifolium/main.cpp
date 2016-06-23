#include "DevLancifolium.h"
#include "GeneBase.h"
#include "FileBuff.h"
#include "FunLancifolium.h"

int main(int argc, char *argv[]) {
    GnTree sig;
    DevLancifolium dev;
    dev.gntree = &sig;
    printf("----------------\n");
    dev.configManual("tmp.sgf");
    printf("----------------\n");
    sig.printtree();
}
