dir /s/b "%~dp0java\src\*.java"> "%~dp0java\sourcefiles.list
javac -h "%~dp0J3D" -d "%~dp0java/bin" @"%~dp0java/sourcefiles.list"
jar --create --verbose --file "%~dp0x64/Debug/J3D.jar" -e "j3d/Window" -C "%~dp0java/bin" .