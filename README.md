My submission to Games Job Fair 2023 Spring Unreal Coding Challenge.

It was revied by Ari Arnbjornsson from Epic Games and was one of the three winners of the contest.

Following the review of the codebase by Ari and some of my own thoughts, some changes have been made to the original code:

* Subsystems were introduced istead of singleton managers
* Issues with garbage-collected pointers were fixed
* Issues with timers not being cleared were fixed
* Project structure was simplified, removing most of modules in favor of one module (the way Lyra does)
* Object initialization was simplified, removing deferred initialization methods
* Game messaging system was introduced instead of coupling various systems to the game state
* Native tags were introduced
