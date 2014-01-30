set PATH=C:\D\dmd2\windows\\bin;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\\\bin;%PATH%
dmd -g -debug -X -Xf"Debug\FirstDApp.json" -deps="Debug\FirstDApp.dep" -c -of"Debug\FirstDApp.obj" main.d
if errorlevel 1 goto reportError

set LIB="C:\D\dmd2\windows\bin\..\lib"
echo. > Debug\FirstDApp.build.lnkarg
echo "Debug\FirstDApp.obj","Debug\FirstDApp.exe_cv","Debug\FirstDApp.map",user32.lib+ >> Debug\FirstDApp.build.lnkarg
echo kernel32.lib/NOMAP/CO/NOI >> Debug\FirstDApp.build.lnkarg

"C:\Program Files (x86)\VisualD\pipedmd.exe" -deps Debug\FirstDApp.lnkdep C:\D\dmd2\windows\bin\link.exe @Debug\FirstDApp.build.lnkarg
if errorlevel 1 goto reportError
if not exist "Debug\FirstDApp.exe_cv" (echo "Debug\FirstDApp.exe_cv" not created! && goto reportError)
echo Converting debug information...
"C:\Program Files (x86)\VisualD\cv2pdb\cv2pdb.exe" "Debug\FirstDApp.exe_cv" "Debug\FirstDApp.exe"
if errorlevel 1 goto reportError
if not exist "Debug\FirstDApp.exe" (echo "Debug\FirstDApp.exe" not created! && goto reportError)

goto noError

:reportError
echo Building Debug\FirstDApp.exe failed!

:noError
