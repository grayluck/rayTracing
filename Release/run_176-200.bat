
for /L %%i in (176,1,200) do (
if exist script/%%i.txt (
	cp script/%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/%%i.bmp
)
)