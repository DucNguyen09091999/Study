#include "log.h"
#include "flog.h"
#include <stdio.h>

#define ILOG(log,msg)   (log)->interface->info(log,msg)
#define ELOG(log,msg)   (log)->interface->error(log,msg)

int main(int argc, char *argv[])
{
    /*Console log sample*/
    log_interface *clog = conlog.interface;

    /*Not really necessary for console log*/
    clog->init(&conlog,0);

    /*Long winded usage*/
    conlog.interface->info(&conlog,"Info message 1");
    conlog.interface->error(&conlog, "Error message 1");

    /*Shorter usage*/
    clog->info((void*)clog, "Info message 2");
    clog->error((void*)clog, "Error message 2");

    ILOG(&conlog, "Info message 3");
    ELOG(&conlog, "Error message 3");

    /*Fle log samples*/

    logger f1;
    logger f2;
    filelog_create(&f1, "file1.log");
    filelog_create(&f2, "file2.log");

    log_interface *flog1 = f1.interface;
    log_interface *flog2 = f2.interface;

    /*short usage*/
    flog1->info(&f1, "Info message 1");
    flog1->info(&f2, "Info message 1");
    flog1->error(&f1, "Info message 1");
    flog1->error(&f1, "Info message 1");

    /*Macro usage*/

    ILOG(&f1, "Info message 1");
    ILOG(&f1, "Info message 2");
    ELOG(&f1, "Error message 1");
    ELOG(&f2, "Error message 2");

    flog1->close(&f1);
    flog2->close(&f2);

    return 0;

}