1. Process i should send message first and then receive message, otherwise,
   it will block.

2. If the program runs with only one process, then the only process will
   send message to itself. In my Linux box, the program hang up just as it
   is a dead lock.
