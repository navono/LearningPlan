rem 参数1为sqlite3.exe执行文件路径
rem 参数2为还原后的的slite DB文件的绝对路径，包括db文件名
rem 参数3为sqlite3.exe产生的备份sql文件的绝对路径

set exepath=%1
set arg1=%2
set arg3=%3

%exepath% %arg1% < %arg3%
del %arg3%