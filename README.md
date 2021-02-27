<p>The GFW or General Purpose C++ Framework, is a simpre lighweight base utility for C++.
It provides the basic functionality for any kind of C++ project.</p>
These include:

<ul>
<li>Strings</li>
<li>Lists (Arrays, Linked lists, Trees, Hash maps etc</li>
<li>XML Parser</li>
<li>Timers</li>
<li>Self created script language</li>
<li>Lexer</li>
<li>Some DSP functions</li>
<li>An Object Modeled Engine(Just the idea on progress)</li>
<li>Image Processing (Just the idea on progress). I already have this
    but as part of an older library I developed.</li>
<li>Memory Management</li>
<li>Math Functions</li>
<li>Some network functions such downloads and uploads</li>
<li>Built-from-scratch flexible Virtual Machine with an Assambler compiler for it</li>
<li>File Management</li>
<li>Dinamic Library Management(using DLL(Windows) or SO(Linux) functions)</li>
</ul>

<p>This project still not completed. I need to finish some modules and add the new one ideas.
Still the purpose has been to create my projects using it for mosts if not all kind of functionality.
It has been grown as much I need a new function to work. So I find it suitable.</p>

<h2>Requirement</h2>

<p>You need QT Creator to handle .PRO files and to compile it. Use GCC or g++ to compile it, mingw compiler for Windows
or Visual C++</p>
<h2>Installation</h2>

<p>Place the project on a folder. Named it GFW. On the libgfw.pro file set the directory where the project is placed.
If you want CURL built in so then set the CURL directory. And set GFW_GUDA (CONFIG+=GFW_GUDA) in configuration on PRO file.
Add configuration GFW_STATICLIB if you want it as static library or leave it without it for Dinamic Library</p>
