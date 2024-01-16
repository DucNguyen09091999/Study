#pragma once

typedef struct logger_ logger;


typedef struct log_interface_ 
{
    int (*init) (logger *self, void *data);
    void (*info) (logger *self, char *str);
    void (*error) (logger *self, char *str);
    void (*close) (logger *self);
} log_interface;

//Logger struct with data object
struct logger_
{
    log_interface *interface;
    void *data;
};

//Only really one implementation of console so extern the implement
extern logger conlog;
