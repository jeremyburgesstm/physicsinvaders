# INTRODUCTION

## Physics Invaders

By Jeremy Burgess

This game prototype was produced as an example game (that ended up being pretty fun!) in order to test my ideas regarding building an entity component model myself, as well as to learn a little bit about working with DirectX 10/11. 

Art kindly created by my wife, Frances Denton.

### Middleware/external libraries used:
 * LibRocket - UI rendering and layout
 * Freetype - Font rendering (used within LibRocket)
 * Box2D - Physics
 * Eigen - Vector and Matrix math and abstractions
 * Boost - Miscellaneous awesomeness

# BUILDING

For now, the only supported platform for this is Win32/DirectX.

1. Ensure you have Visual Studio 2010, The DirectX SDK, and Boost installed.
    *  Note that this should work in VS 2012, however due to C++11 features used, earlier versions of the MSVC compiler will not support this codebase.
2. Set the following environment variables:
    *  BOOST_ROOT: Points at the boost install folder.  
    *  DXSDK_DIR: Points at the DirectX SDK install folder.  
3. Open PhysicsInvaders.sln
4. Build & Run!

# NOTES

This section contains various notes about code structure and the nature of this project.

 * This project was built over quite a long period inbetween other things, and the purpose of bits of code (as well as my own opinions about coding style) has changed over that timespan. As a result there are places where it isn't as consistent as I'd like from a production codebase.
   * In order to keep things clean, this is a subset of my own private repository - hence the lack of history.
 * At the end of the day, this is a game. Even though parts of it were built with learning in mind, the final hurdles were very much about actually finishing the project to an extent I was happy with.
 * There are aspects of the codebase which were built the way they were in order to allow for me to experiment with the stuff I wanted to, rather than the stuff I didn't:
    * The "ShouldBeDataDriven" namespace in particular is (undeniably!) pretty untidy. As its name should suggest, if I were ever going to make this into a shippable title or build it into anything larger than it is already, it would be worth data driving (at least) large parts of it. For this reason I've not been too concerned about code duplication and such within it.
 * Core structures:
    * Generic state machine: The application flow (and the game flow) are both driven by a generic state machine, which allows any number of states to be build and chained together in such a way that individual states need to know nothing about the next state they're going to. This structure would allow trivial data driving, once a data definition format was defined. I'm pretty happy with this as a game driving (or even game logic driving) state machine, however for a larger scale application I would consider a hierarchical state machine for driving application flow due to the much richer flow control such a system can provide.
    * Entity component model: One of the purposes of this application was to build my own Entity Component model and build a game on top of it. Having used Unity professionally for the past couple of years, I was curious to see what kinds of compromise I would end up making when building my own version.
        * Component pools are fixed size, and all components are allocated up front. This could be changed to allow dynamic resizing, but I didn't see the value for this project where I knew up front how many of each type of component would be required and memory was unlikely to be a problem.
	    * Updates are 'batched' for want of a better word. In other words, all updates of a given type (for instance, physics update) for a given type of component occur together. As all instances of a given component are allocated at once (see the aforementioned pool structure). These together should mean fairly good locality for both code & memory when performing the game update loop.
    * Game message hub: Bits of the game are tied together through a simple publish/subscribe system - i.e., components subscribe to events they're interested in, and then receive notification when those occur.
    * Action/Invoker: I was interested in whether I could recreate something like C# delegates using Templates. This is my first attempt at doing something like that:
        * The actions I've created are strictly that - actions. Unlike the more general structures at the the heart of C# delegates they are not multicast, and they can't have return values. Making them multicast would be trivial (stick an STL Vector inside each one with a list of Invokers), but was unnecessary for my purposes.
	    * In retrospect I'm not sure they're actually ideal for the purpose at hand - simple interfaces & handler methods would have worked just as well and been simpler to implement, however I wouldn't have learned anything had I done that!
 * Graphics: The rendering elements of this project are all built by me, and were mostly in aid of me learning a bit about something I've not really had much exposure to since University. The actual rendering objects (renderer, quadrenderer, texture etc...) are very simplistic, but do what I needed them to in order to realise the kind of game I wanted to experiment with.. As I note in "NEXT STEPS", one of the most important next steps would be making this support multiple rendering APIs, so that other platforms could be supported. 

# NEXT STEPS

The following is a laundry list of stuff I'd like to do, should I decide to pick up this project again at a later date.

 * Port to Android.
    * Add structural support for multiple renderers/input detection (i.e., RendererD3D can become IRenderer, and so on).
    * Create OpenGL renderer.
    * Perform the port/integration with the JNI. (Learning task)
 * Add invader atomising.
    * It would look awesome if the invaders turned into a bunch of appropriately colour physics driven particles when they died.
    * Could 'examine' the texture and auto-generate the particle objects.
 * Create a particle system (learning task).
    * Lots of places that particles would look great.
    * Look at ways to render particle systems.
    * Potentially look at ribbon particles (i.e., particles extruded along their vector of motion - commonly used for sparks)
    * Potentially create a task based update structure so it can be heavily multithreaded.
 * Add simplistic game event driven audio
    * Sound makes a huge difference to games!
    * Use OpenAL.
 * Add bloom
    * Post-processing should be straightforward to add, would look great.