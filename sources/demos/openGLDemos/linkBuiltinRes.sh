#!/bin/bash
echo "linkBuiltinRes Begin..."
targetPath=$(echo $1 | sed 's/\//\\\\/g')
sourcePath=$(echo $2 | sed 's/\//\\\\/g')
echo "targetPath is : $targetPath"
echo "sourcePath is : $sourcePath"
cmd.exe /c "mklink /d $targetPath $sourcePath"
echo "linkBuiltinRes Finish..."
