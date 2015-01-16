
for /L %%i in (151,1,175) do (
if exist script/%%i.txt (
	cp script/%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/%%i.bmp
)
)