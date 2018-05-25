set PSExecutionPolicyPreference=Bypass
powershell (get-date).minute*60+(get-date).second+(get-date).Millisecond/1000 >> C:\CyberRange\initif\result.txt
powershell C:\CyberRange\initif\initif_win.ps1

powershell C:\CyberRange\initif\check_ip_addr.ps1
powershell (get-date).minute*60+(get-date).second+(get-date).Millisecond/1000 >> C:\CyberRange\initif\result.txt