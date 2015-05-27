NWT/PI : the easiest way to use the Raspberry Pi without X Window :
C++ simplified API like the Java JOGL::NEWT ; put the Dispmanx C API tricky use to the background, eg :

```
// 1. Choose a cap btw RGB565, RGB888 and RGBA888 :
OEGLCapabilities * caps = new OEGLCapabilities(OEGLCapabilities::RGB565);

// 2. Set / unset EGL Surface attributes : 
caps->setAttribute(EGL_DEPTH_SIZE,24);

// 3. Choose whether native window background will be transparent or not :
unsigned char alphaLevel = 0;

// 4. Create your Native Window :
OEGLWindow * window = new OEGLWindow("MyDrawable", width, height, alphaLevel, caps);

MyGlObject * Triangle = new MyGlObject();

// END. Draw and refresh your screen :
Triangle->drawScene();

window->swapBuffers();
```

**Happy to use my code ? Please, leave a comment at the link below, thank you**

**Article + "NWTPI in action" picture :**
http://vslash.com/works/native-window-toolkit-api_the-raspberry-pi-without-x-window.html

![http://www.vslash.com/images/nwtpi_overview.png](http://www.vslash.com/images/nwtpi_overview.png)
