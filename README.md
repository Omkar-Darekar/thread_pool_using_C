# thread_pool_using_C
Creating threads can be quite expensive. Typically each thread is going to do essentially the same thing so it’s good to keep reusing them. Threads are actually quite heavy and creating or destroying threads takes time away from what you’re trying to accomplish.  Another benefit of thread pools is they keep the system from becoming overloaded. They allow a limit to the number of threads and tasks are queued and only run when a thread is available.

Note =========>>>>>>>

Its the Socket Programming Code.
Make sure you having lpthread library .
Check you PORT number and other details in both the CODE.

Command to RUN code ---

Server.c ------------>>>>

gcc Server.c -o Server -lpthread

./Server



Client.c ----------->>>>

gcc Client.c -o Client -lpthread

./Client

