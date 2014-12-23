#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "skull/api.h"
#include "skull/sk_txn.h"
#include "skull_metrics.h"

#define CMD_HELP "commands help:\n - help\n - show\n"

void module_init()
{
    printf("admin module(test): init\n");
}

// The command format: command_string\r\n
// commands:
// - show [metrics]
// - help
size_t module_unpack(const char* data, size_t data_sz)
{
    printf("admin module_unpack(test): data sz:%zu\n", data_sz);

    if (data_sz <= 2) {
        return 0;
    }

    if (0 != strncmp(&data[data_sz - 2], "\r\n", 2)) {
        return 0;
    }

    SKULL_LOG_INFO(1, "admin module_unpack(test): data sz:%zu", data_sz);
    return data_sz;
}

static
void process_show()
{
    skull_metric_dump(0);
}

int module_run(sk_txn_t* txn)
{
    size_t data_sz = 0;
    const char* data = sk_txn_input(txn, &data_sz);
    char* command = calloc(1, data_sz + 1);
    memcpy(command, data, data_sz - 2); // remove the tailer '\r\n'

    printf("receive command: %s\n", command);
    SKULL_LOG_INFO(1, "receive command: %s", command);

    if (0 == strcmp("help", command)) {
        sk_txn_output_append(txn, CMD_HELP, strlen(CMD_HELP));
    } else if (0 == strcmp("show", command)) {
        process_show();
    } else {
        sk_txn_output_append(txn, CMD_HELP, strlen(CMD_HELP));
    }

    free(command);
    return 0;
}

void module_pack(sk_txn_t* txn)
{
    size_t data_sz = 0;
    const char* data = sk_txn_input(txn, &data_sz);

    printf("module_pack(test): data sz:%zu\n", data_sz);
    SKULL_LOG_INFO(1, "module_pack(test): data sz:%zu", data_sz);
    sk_txn_output_append(txn, data, data_sz);
}
