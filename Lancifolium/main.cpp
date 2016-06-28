#include "DevLancifolium.h"
#include "GeneBase.h"
#include "FileBuff.h"
#include "FunLancifolium.h"

int main(int argc, char *argv[]) {
    GnTree sig;
    DevLancifolium dev;
    dev.gntree = &sig;
    printf("----------------\n");
    if (argc < 2) {
        printf("exe [filename]\n");
    }
    if (dev.configManual(argv[1]) == 1) {

    }
    else {
        printf("----------------\n");
        //sig.printtree();
		sig.printpool();
		sig.deleteroot(sig.treeroot);
    }
}
