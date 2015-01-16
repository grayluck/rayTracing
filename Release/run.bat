
for /L %%i in (0,1,9) do (
if exist script/00%%i.txt (
	cp script/00%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/00%%i.bmp
)
)
for /L %%i in (10,1,25) do (
if exist script/0%%i.txt (
	cp script/0%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/0%%i.bmp
)
)