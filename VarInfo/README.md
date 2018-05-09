Things to do,

Variable level fault injection.

Build dependency graph based on variable name.

How to do that, maybe a challenging tasks.


Steps that I approach to  the problem is as follows:-

- Use debug information to find the variable declaration.
- How to track whether they use this particular variable- maybe using def-use chain
- What is def-use chain


Step 1: Error injection on targeting variable.
How to specify which variable to inject error??
How to dynamically do that??
Given a pass that extract all variable and their information??

- Pass that collect variable information and context of the declaration. output into a json file
- Automated script that run the compilation from start to end.




