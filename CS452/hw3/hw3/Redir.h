
#include "Tree.h"
typedef void* Redir;

extern Redir newRedir(T_redir redirec);
extern void free_redir(Redir redir);
void execRedir(Redir redir);