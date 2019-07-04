# initif for windows

## compile command
```
gcc -o initif .\initif_win.c
```

## Installation of the program
Use "task scheduler" for installation of the program.
1. Windows-R
2. Enter "Taskschd.msc"
3. Choice task scheduler library
4. Click Create task
5. Choice General tag
   - Name: initif
   - Check it "Run with highest privileges"
6. Choice trigger tag and Click New
   - Begin the task :"At log on"
7. Choice Actions tag and Click New
   - Action: "Start a program"
   - Program/script: \<initif script PATH\>
8. Choice Conditions tag
   - Uncheck "Start the task only if the computer is on AC power"


