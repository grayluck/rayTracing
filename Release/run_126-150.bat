
for /L %%i in (126,1,150) do (
if exist script/%%i.txt (
	cp script/%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/%%i.bmp
)
)