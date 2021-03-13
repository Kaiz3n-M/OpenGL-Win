# OpenGL-Windows

## Resources

* Best _OpenGL_ tutorial: https://learnopengl.com/
* Best _OpenGL_ specification documentation: http://docs.gl/

## Documentation
### What OpenGL is?
_OpenGL_ is not a Graphic Library. It is a specification, a sets of rules that describe how the APIs that it provides should work. 
We can call _OpenGL_ an **API** (Application Programming Interface), these APIs specifications are then used by graphic cards 
manufacturers to create graphic drivers that implement the _OpenGL_ specification. So _OpenGL_ is not a library, in the sense that 
you don't need a software to run it. It already lives within your graphic card. Indeed, the dirver of your card implements it; 
for exmaple, the NVIDIA card you have will have a driver that has _OpenGL_ API implemented and you can check which version of 
the _OpenGL_ specification it supports. _OpenGL_ is just one of the many specification available for graphics; many OS provides 
native specifications or APIs that can be used instead of _OpenGL_. For example, _Microsoft Windows_ offers **Direct3D**, _Mac OS_ 
uses **Metal**, and we have also have new powerful **Vulkan** APIs. _Vulkan_ and _OpenGL_ are a cross platform specification.
That means the these specification are supported across platforms, so _OpenGl_ and _Vulkan_ run on _Linux_, _Mac OS_, 
_Microsoft Windows_, _Android_, and _iOS_.

People belive that _OpenGL_ is powerful beacuse it is available in multiple platform. That is not the case; _OpenGL_ is a really simplistic
and easy to learn/use API. In contrast, _Direct3D_ and _Vulkan_ are relly powerful and a bit more low level. Micorsoft works with Grphic card 
manufacturer to implement their _Direct3D_ straight into the driver, convicing them to unlock a lot of powerful feature already available already
in the graphic card.

### How does it work?
_OpenGL_ is state-machine, that means that the way you interact with the API it is through states. To clarify, what I mean with this state-machine 
here, is that everything you do in _OpenGL_ is contextual. Let me better exmplain this with an example. Let's assume you were developing a specification
like _OpenGL_ and you want to provide an API that allows developer to draw a triangle in the screen. You probably are going to create an API call like 
this one _draw(float[] vertices, ShapeType shape, int first, int count)_. A developer might call this API like this _draw(vertexArray, TRIANGLE, 0, 3)_. 
So let's describe what this call is supposed to do: 

* _draw_ is the name of the API that has the implementation to draw a triangle on the screen
* _float[] vertices_ is an array of vertices needed that tell the API where to poisition the vertices of the triangle on the screen
* _ShapeType shape_ is the shape we want with the vertices we have
* _int first_ the frist vertex we want to use from the vertexArray
* _int count_ how many vertex we have in there

So with the above example we have described how we could draw a triangle; in one call we provide **all the info** needed to draw a trianle, but that is not
the way _OpenGL_ does it. _OpenGL_ useses a state-machine instead. So to draw a triangle in _OpenGL_ we do the following:

1. We setup the vertex buffer that has the vertices we want to draw
```
    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // Create Vertex Buffer Object
    uint32_t VBO;
    // Create buffer and return buffer handle into VBO
    glGenBuffers(1, &VBO);

    // Bind buffer by VBO handle to the object create by glGenBuffer call
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    // Write inside VBO buffer
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(float), mVertices.data(), GL_STATIC_DRAW);
```
2. Then we call we make a draw call to draw the triangle but without the need to specify the vertices because the _OpenGL_ will use the last 
buffer we have specifed. This is _OpenGL_ the state-machine operates, so everything is contextual in _OpenGL_.
```
    // Draw a triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
```

### How do you code OpenGL?

As I have mentioned before _OpenGL_ is not a Libary, so there is not code that needs to be installed to work with it. So, how do you code agaist 
_OpenGL_ then? Funny thing is that we need a real "phisical code" implementation to work with. _OpenGL_ code does exist int the graphic card driver though, 
so we need a way to get the out of the driver the API calls, so we can start callling them. In this project we use GLAD to map the _OpenGL_ code to C++
and that allows us to call it. We could have mapped that ourselves but it would have taken too long since there are more than 5000 APIs call in modern _OpenGL_.

