rd Test\Debug\resources\html
rd Test\Release\resources\html
rd Debug\resources\html
rd Release\resources\html
mklink /d Test\Debug\resources\html ..\..\..\..\src\html
mklink /d Test\Release\resources\html ..\..\..\..\src\html
mklink /d Debug\resources\html ..\..\..\src\html
mklink /d Release\resources\html ..\..\..\src\html