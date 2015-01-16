
for /L %%i in (51,1,75) do (
if exist script/0%%i.txt (
	cp script/0%%i.txt test.txt
	rayTracing
	cp img/output.bmp output/0%%i.bmp
)
)