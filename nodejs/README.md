### What is it
Nodejs is a js runtime. The typical pattern
in Node.js is to use asynchronous callbacks. Basically you're telling it to do
something and when it's done it will call your function (callback). This is
because Node is single-threaded. While you're waiting on the callback to fire,
        Node can go off and do other things instead of blocking until the
        request is finished.

### How to Run
 - node *.js

### How install a module
 - npm install // and keep all the dependencies in package.json OR
 - npm install express
 
