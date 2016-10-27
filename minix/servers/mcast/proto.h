/* Function prototypes. */


/* main.c */
int main(void);
void setreply(int proc_nr, int result);

/* handlers */
int do_test(void);
int do_msend(void);
int do_mreceive(void);
int do_opengroup(void);
int do_closegroup(void);
int do_recovergroup(void);

/* utility.c */
int no_sys(int who_e, int call_nr);
