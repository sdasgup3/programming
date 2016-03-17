### Tips
- The follwoing shows the presence of tabs with ^I and  line endings with $ both are vital to ensure that dependencies 
end properly and tabs mark the action for the rules so that they are easily identifiable to the make utility.
`cat -e -t Makefile`
- Cancelling implicit rules
    - You can override a built-in implicit rule (or one you have defined yourself) by defining a new pattern rule with the same target and prerequisites, but a different recipe. When the new rule is defined, the built-in one is replaced. The new rule’s position in the sequence of implicit rules is determined by where you write the new rule.
    - You can cancel a built-in implicit rule by defining a pattern rule with the same target and prerequisites, but no recipe. For example, the following would cancel the rule that runs the assembler:  %.o : %.s
-   To prevent deletion of temporary files (as determined by Makefile system), add `PRECIOUS: %.o`
-   make VERBOSE=1. In cmake, You can set CMAKE_VERBOSE_MAKEFILE to ON.

## Debuging
-  $(warning dsand $(LCC))
-  @echo $(LCC)
