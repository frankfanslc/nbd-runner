/*
  Copyright (c) 2019 Red Hat, Inc. <http://www.redhat.com>
  This file is part of nbd-runner.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "rpc_nbd.h"
#include "utils/utils.h"
#include "nbd-log.h"
#include "nbd-cli-cmd.h"

struct cli_cmd gluster_cmds[];

static int gluster_help_routine(int count, char **options)
{
    int i;

    _nbd_out("Usage: \n\n");
    for (i = 0; gluster_cmds[i].pattern; i++) {
        if (gluster_cmds[i].disable)
            continue;

        _nbd_out("\t%s\n", gluster_cmds[i].pattern);
        _nbd_out("\t\t%s\n\n", gluster_cmds[i].desc);
    }
    _nbd_out("\n");
}

static int gluster_create_routine(int count, char **options)
{
    return nbd_create_backstore(count, options, NBD_BACKSTORE_GLUSTER);
}

static int gluster_delete_routine(int count, char **options)
{
    return nbd_delete_backstore(count, options, NBD_BACKSTORE_GLUSTER);
}

static int gluster_map_routine(int count, char **options)
{
    return nbd_map_device(count, options, NBD_BACKSTORE_GLUSTER);
}

static int gluster_unmap_routine(int count, char **options)
{
    return nbd_unmap_device(count, options, NBD_BACKSTORE_GLUSTER);
}

static int gluster_list_routine(int count, char **options)
{
    return nbd_list_devices(count, options, NBD_BACKSTORE_GLUSTER);
}


struct cli_cmd gluster_cmds[] = {
    {.pattern = "gluster",
     .call    = gluster_help_routine,
     .desc    = "Display help for gluster commands",
     .disable = true,
    },
    {.pattern = "gluster help",
     .call    = gluster_help_routine,
     .desc    = "Display help for gluster commands",
    },
    {.pattern = "gluster create <VOLUME@GLUSTER_HOST:/FILEPATH> [prealloc] <size SIZE> <host CONTROL_HOST>",
     .call    = gluster_create_routine,
     .desc    = "Create FILEPATH in the VOLUME, prealloc is false as default, and the SIZE is valid\n\t\twith B, K(iB), M(iB), G(iB), T(iB), P(iB), E(iB), Z(iB), Y(iB)",
    },
    {.pattern = "gluster delete <VOLUME@GLUSTER_HOST:/FILEPATH> <host CONTROL_HOST>",
     .call    = gluster_delete_routine,
     .desc    = "Delete FILEPATH from the VOLUME",
    },
    {.pattern = "gluster map <VOLUME@GLUSTER_HOST:/FILEPATH> [nbd-device] [timeout TIME] [readonly] <host CONTROL_HOST>",
     .call    = gluster_map_routine,
     .desc    = "Map FILEPATH to the nbd device, as default the timeout 0, none readonly",
    },
    {.pattern = "gluster unmap <nbd-device> <host CONTROL_HOST>",
     .call    = gluster_unmap_routine,
     .desc    = "Unmap the nbd device",
    },
    {.pattern = "gluster list [map|unmap|create|dead|live|all] <host CONTROL_HOST>",
     .call    = gluster_list_routine,
     .desc    = "List the mapped|unmapped NBD devices or the created|dead|live backstores, all as\n\t\tdefault. 'create' means the backstores are just created or unmapped. 'dead' means\n\t\tthe IO connection is lost, this is mainly due to the nbd-runner service is restart\n\t\twithout unmapping. 'live' means everything is okay for both mapped and IO connection."
    },
    {.pattern = NULL,
     .call    = NULL,
     .desc    = NULL,
    },
};

int cli_cmd_gluster_register(GHashTable *cmds_hash)
{
    return nbd_register_cmds(cmds_hash, gluster_cmds);
}
