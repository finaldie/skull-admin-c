skull-admin-c
=============

Skull Admin module (C version)

## Installation
```
cd $SKULL_PROJ_ROOT/src/modules
git clone git@github.com:finaldie/skull-admin-c.git admin
cp admin/idl/admin.proto ../../config
skull module --conf-gen
skull workflow -add # then input concurrent: no, idl: admin, port: yes -> 7759
skull module -add   # then input the module name: "admin", workflow: "1"
skull build
skull deploy
skull start
```

## Usage
Currently, it only support `show` command, after start `skull`,
then try the `show` command by `telnet 0 7759`, for example:
```
telnet 0 7759
Trying 0.0.0.0...
Connected to 0.
Escape character is '^]'.
show
skull.core.g.global.connection: 2.000000
skull.core.g.global.response: 8.000000
skull.core.g.global.latency: 1772.000000
skull.core.g.global.request: 9.000000
skull.core.t.worker.accept.worker0: 2.000000
skull.core.t.worker.latency.worker0: 1772.000000
skull.core.t.worker.connection.worker0: 2.000000
skull.core.t.worker.response.worker0: 8.000000
skull.core.t.worker.request.worker0: 9.000000
skull.user.s.module.response: 8.000000
skull.user.s.module.request: 8.000000
```
