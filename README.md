skull-admin-c (Deprecated)
=============

Skull Admin module (C version)

## Installation
```
cd $SKULL_PROJ_ROOT/src/modules
git clone git@github.com:finaldie/skull-admin-c.git admin
cp admin/idl/admin.proto ../../idls/workflow
cd admin && skull module --conf-gen
skull workflow -add # then input concurrent: no, idl: admin, port: yes -> 7759
skull module -add   # then input the module name: "admin", workflow: "1"
skull build
skull deploy
```

## Usage
Currently, it only support `show` command, after start `skull`,
then try the `show` command by `telnet 0 7759`, for example:
```
telnet 0 7759
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.

help
commands help:
 - help
 - show

show
skull.core.g.global.connection: 1.000000
skull.core.g.global.response: 1.000000
skull.core.g.global.latency: 347.000000
skull.core.g.global.request: 4.000000
skull.core.t.worker.accept.worker0: 1.000000
skull.core.t.worker.latency.worker0: 347.000000
skull.core.t.worker.connection.worker0: 1.000000
skull.core.t.worker.response.worker0: 1.000000
skull.core.t.worker.request.worker0: 4.000000
```
