# deque
Blocking double ended queue for multithreaded C.

the test program creates an hypercube of nodes labeled [a\_1, a\_2,
..., a\_n] where a\_i represents dimension i and a\_i belongs to the set
{0, 1} 

Each node is managed by a different thread, and so we have 2^n nodes,
labeled as [0, 0, 0, 0, ..., 0] to [1, 1, 1, 1, 1,..., 1]

each thread is atached to a queue that receives and stores messages
(messages are labeled from \<M-00> to <M-N> in which N is the number
of messages to input to the system)

N input messages are queued to node [00] (label is the binary vector
represented above, but in decimal form) and each thread processes each
message by getting it from its reading queue and feeding it to the
corresponding queue in a round robing fashion.

Let's suppose a three dimensional cube.  It has nodes [00]..[07] (the last
represents three one bits)

node [00] feeds in sequence to queues [01], [02] and [04].
node [01] feeds in sequence to queues [03], [05]
node [02] feeds in sequence to queues [03], [06]
node [04] feeds in sequence to queues [05], [06]
node [03] feeds in sequence to queue [07]
node [05] feeds in sequence to queue [07]
node [06] feeds in sequence to queue [07]
and finally node [07] just gets the message (which should be misordered
due to the different paths followed by the differnt messages and the
scheduling of threads)

An EOF message is allowed by the possibility of enqueuing a NULL pointer
to a queue and this marks the end of the data... but for each node to
finish, there are as EOF messages as nodes feed to the node requested, so

node [00] will receive only one EOF
nodes [01], [02], [04] will receive only one EOF
nodes [03], [05], [06] will receive two EOFs.
node [07] will receive three EOFs.

the main routine calculates and creates a hypercube with the data paths,
the number of eofs that is willing to receive each thread and then creates
the thread passing each one the corresponding node of the hypercube.

Once done, the main routine puts N messages in the initial node of the
hypercube, and follows it with an NULL message marking the END OF DATA.

As the threads start, they print a message with the thread number and BEGIN
label, followed by all messages they process, message processing is done
by printing the thread number, message id, and queue this thread is
enqueuing the message to.

As each thread receives each EOF, it marks it on screen.

thread messages are indented a number of spaces equivalent to two spaces
per thread id, so each thread messages are aligned in the same column to
make them easily visible on screen.

an example of run can be:

    test_queue -n 3

for a 3D binary cube.

    test_queue -n 8

for an 8D binary hypercube.
