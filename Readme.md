## **Instructions to execute:**

```
make && ./run
```
### **Implementaion details:**
shell.c:
```
Takes the arguments and calls a function to start.
```
processing.c:
```
Contains a loop which calls function which seperates arguments with colon and spaces.
Then calls command_execute for execution of commands.
```

parsing.c:
```
It divides the arguments based on the delimitor given.
```

commands.c:
```
It contains functions for the execution of builtin commands such as ls,echo,pinfo,cd.
```

fork.c:
```
It creates child process and executes command in either in foreground and background as mentioned.
```

shell.h:
```
It contains all declarations of functions and global variables. 
```

## **Commands implemented:**
`setenv var [value]` :
```
If environment variable var does not exist, then shell creates it. shell sets set the value of var to value, or to the empty string if value is omitted.
```
`unsetenv var` : 
   ```
Shell destroys the environment variable var.
```

`jobs` : 
```
Prints a list of all currently running jobs along with their pid
```

`kjob jobNumber signalNumber` :
```
Takes the job id of a running job and sends a signal value to that process
```

`fg jobNumber` : 
```
Brings a running or a stopped background job with given job number to foreground
```
`bg jobNumber` : 
```
Changes a stopped background job to a running background job.
```
`overkill` : 
```
Kills all background process at once.
```
`quit` : 
```
Exits the shell.
```

> All the other linux commands like ls,echo,cat,pinfo,cd etc function normally.