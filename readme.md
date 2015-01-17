# Ray Tracing #

A highly OOP implementation of Monte-Carlo Ray Tracing rendering 

## Feature ##

* Image display based on Windows GDI
* Load and render .obj objects
* Speed up ray-object collision check with oct-tree 
* Sphere, box and triangle supported
* Soft shadow
* Three types of surface
	- reflection
	- refraction
	- diffusion
* Area light and volume light supported
* Texture
* Complex scene generator for video
* Anti-aliasing
* Field depth

## Dependencies ##

This is an independent project without any third party libraries. 

This project can be successfully built by Visual Studio 2012 and latter. It is not a cross-platform project for now.  

## Results ##

### Anti-aliasing ###
<center>
<div style="display:inline-block;width:40%;" class="imagebox">
<img src="./imgs/noalia.jpg" style=""></img>
<div>Original</div>
</div>
<div style="display:inline-block;width:40%;padding-left:10px;" class="imagebox">
<img src="./imgs/alia.jpg" style=""></img>
<div>Anti-aliased</div>
<p></p>
</div>
</center>

### .obj rendering###

<center>
<div style="display:inline-block;width:45%;" class="imagebox">
<img src="./imgs/rabbit_281.jpg" style=""></img>
<div>diffusing rabbit (281 iters)</div>
</div>
<div style="display:inline-block;width:45%;padding-left:10px;" class="imagebox">
<img src="./imgs/rabbit_937.jpg" style=""></img>
<div>diffusion rabbit (937 iters)</div>
<p></p>
</div>
</center>

Glass rabbit(678 iters, depth of sub-lights: 10）

![](./imgs/crystal_678.jpg)

### light spot and recursive mirror ###

![](./imgs/lightSpot.jpg)

![](./imgs/reflection.jpg)

![](./imgs/reflection.gif)

### Texture lightening ###

![](./imgs/texture_lighting.jpg)

### Field depth ###

![](./imgs/fieldInDepth_balls.jpg)

![](./imgs/fieldInDepth.jpg)

### Full video ###

See [Full Video here](www.youtube.com/watch?v=Nm3q0iIu2xY).

## References

* [https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html](https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html "https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html")
* smallpt project

## Contact

A Chinese version of readme in detail is available. 

You can contact [monocofe@gmail.com](mailto:monocofe@gmail.com) for more information.


