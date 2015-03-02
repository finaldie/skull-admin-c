/**
 * This is an example admin module for pulling metrics informations
 * IDL: admin.proto
 *
 * @note before you compile it, make sure the admin.proto has in
 *       $top_project/config
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "skull/api.h"
#include "skull_metrics.h"
#include "skull_idl.h"
#include "config.h"

#define CMD_HELP "commands help:\n - help\n - show\n"
#define MAX_METRICS_COUNT 100
#define MAX_METRICS_LEN   256

void* module_init(skull_config_t* config)
{
    printf("admin module(test): init\n");
    return NULL;
}

void module_release(void* user_data)
{
    printf("admin module(test): release\n");
}

// The command format: command_string\r\n
// commands:
// - show [metrics]
// - help
size_t module_unpack(skull_txn_t* txn, const void* data, size_t data_sz)
{
    printf("admin module_unpack(test): data sz:%zu\n", data_sz);

    if (data_sz < 2) {
        return 0;
    }

    const char* cmd = data;
    if (0 != strncmp(&cmd[data_sz - 2], "\r\n", 2)) {
        return 0;
    }

    Skull__Admin* admin_data = skull_idldata_admin(txn);
    admin_data->ignore = 0;

    // for the empty line, just mark this request can be ignored
    if (data_sz == 2) {
        admin_data->ignore = 1;
        return data_sz;
    }

    // store command string, remove the '\r\n', and append '\0'
    admin_data->command = calloc(1, data_sz - 2 + 1);
    memcpy(admin_data->command, data, data_sz - 2);

    // create 100 metrics slots
    admin_data->n_metrics_array = MAX_METRICS_COUNT;
    admin_data->metrics_array =
        calloc(MAX_METRICS_COUNT, sizeof(*admin_data->metrics_array));

    SKULL_LOG_INFO(1, "admin module_unpack(test): data sz:%zu", data_sz);

    return data_sz;
}

static
void _metrics_each_cb(const char* name, double value, void* ud)
{
    skull_txn_t* txn = ud;
    Skull__Admin* admin_data = skull_idldata_admin(txn);
    int index = admin_data->metrics_count;
    if (index >= MAX_METRICS_COUNT) {
        printf("exceed the max metrics limitation: %d\n", MAX_METRICS_COUNT);
        return;
    }

    printf("metrics loop: index=%d, name: %s, value: %f\n", index, name, value);

    // fill the metrics
    admin_data->metrics_array[index].len = MAX_METRICS_LEN;
    admin_data->metrics_array[index].data = calloc(1, MAX_METRICS_LEN);
    snprintf((char*)admin_data->metrics_array[index].data, MAX_METRICS_LEN,
             "%s: %f\n", name, value);

    // update the counter
    admin_data->metrics_count = ++index;
}

static
void process_show(skull_txn_t* txn)
{
    skull_metric_foreach(_metrics_each_cb, txn);
}

static
void process_help(skull_txn_t* txn)
{
    Skull__Admin* admin_data = skull_idldata_admin(txn);
    admin_data->metrics_array[0].len = MAX_METRICS_LEN;
    admin_data->metrics_array[0].data = calloc(1, MAX_METRICS_LEN);

    // fille the response
    char* response = (char*)admin_data->metrics_array[0].data;
    strncpy(response, CMD_HELP, strlen(CMD_HELP));

    // update the counter
    admin_data->metrics_count = 1;
}

int module_run(skull_txn_t* txn)
{
    Skull__Admin* admin_data = skull_idldata_admin(txn);
    bool ignore = admin_data->ignore;
    if (ignore) {
        return 0;
    }

    const char* command = admin_data->command;
    printf("receive command: %s\n", command);
    SKULL_LOG_INFO(1, "receive command: %s", command);

    if (0 == strcmp("help", command)) {
        process_help(txn);
    } else if (0 == strcmp("show", command)) {
        process_show(txn);
    } else {
        process_help(txn);
    }

    return 0;
}

void module_pack(skull_txn_t* txn, skull_txndata_t* txndata)
{
    Skull__Admin* admin_data = skull_idldata_admin(txn);
    bool ignore = admin_data->ignore;
    if (ignore) {
        return;
    }

    // pack the reponse string to txndata
    int metrics_count = admin_data->metrics_count;
    for (int i = 0; i < metrics_count; i++) {
        const ProtobufCBinaryData* metrics = &admin_data->metrics_array[i];
        skull_txndata_output_append(txndata, metrics->data,
                                    strlen((const char*)metrics->data));
    }

    printf("module_pack(test): complete\n");
    SKULL_LOG_INFO(1, "module_pack(test): complete");
}
