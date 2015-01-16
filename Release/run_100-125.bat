
for /L %%i in (100,1,125) do (
if exist script/%%i.txt (
	cp script/%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/%%i.bmp
)
)