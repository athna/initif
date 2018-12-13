set PSExecutionPolicyPreference=Bypass
powershell (get-date).minute*60+(get-date).second+(get-date).Millisecond/1000 >> C:\CyberRange\initif\result.txt
C:\CyberRange\initif\initif.exe >> C:\CyberRange\initif\result.txt

powershell (get-date).minute*60+(get-date).second+(get-date).Millisecond/1000 >> C:\CyberRange\initif\result.txt