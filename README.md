# This is to sub and publish events.

## Setting up on debian

```
# Assuming you have mysql db installed and setup.

cd events

chmod u+x setup.bash

./setup.bash

```

## Running

```
make

cd build

./events_api
```