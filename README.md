# RTOS_Concurrency

Scenario:

A UART/USART port prints the data structure once a second. The structure is complicated - i.e contains many fields filled by different tasks. Let's say that there exist three tasks that publish the results of their calculations in the structure. The task which implements communication by UART/USART calculates the mean value of the results published by tasks.
Let each task generate a 1000 random value in the range of ±1.0. Next a task should calculate mean and mean square values. Both values have to be published in the global structure. Thus UART task can finally calculate the mean of all means and the mean of all means square.

ToDo:
* stage_1 (40 pts.) - Solve the concurrency problem using semaphore
* stage_2 (60 pts.) - Solve the concurrency problem using mutex

Rules:
* don't use queues
* each task should have a different priority
* UART communication protocol is intentionally slowed down (i.e. OsDelay(10) after each character) and additionally it blocks access to the data structure (i.e. taking semaphore/mutex) till the end of print.
