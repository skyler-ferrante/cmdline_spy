# cmdline_spy

Works like snoopy, but instead of hooking execv we just poll /proc/ for new processes.
Useful when we have low privilege code execution and we want to privilege escalate.

Sometimes people do things like:
```
echo 'user:newpassword' | chpasswd
```

Also useful to just see what a server is used for.

## Permissions

Requires very little permissions
- Ability to list files in /proc
- Ability to read /proc/
- Should work from very low privilege users, e.g. www-data.

## Drawbacks

Due to it's simplicity this project has multiple drawbacks
- Will not catch processes that end quickly. (USLEEP_TIME controls sleep time)
- Due to a simple hash-set implementation, pids larger than MAX_PROC_ID will cause it to fail
