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

opt -view-cfg instprog.bc 

dot -Tpdf /tmp/cfg_Z9passParamiPi-1ed794.dot -o out.pdf


Given a variable for fault injection 
Steps:
1. Track alloca instruction, if the variable name matches with the target variable, track its user using def-use chain
2. 

# Fault Modelling
Our goal is to inject error on a target variable. 
 
 ## Implementation
  Blocking re-assignment of a variable. 
   When a variable is declared, retrieved all user of the target variable using def-use chain.
   
  ##### Find the first `StoreInst` from user instruction.
  ##### Retrieved the value of the `source` operand and random bitflip the value.( bitflip can be single/double depends on user choice).
  ##### For all `StoreInst` in the user list, set the source operand to the random bitflip value.
   
   
   # Array Fault Injection 
   
   ## Static initialization
  consider a simple example in C:
   double a[10]={1,2,3,4,5,6,7,8,9,10};
   
   This translate to LLVM IR as follow:-
   
   %a =alloca[10 X double]
   
   %0 = bitcast[10 X double]* %a to i8*
   
   call llmv.memcopy(%0,@str) 
   
   basically initialize to some constant which is {1.. 10}
   
   ## fault Implementation
    
   %a =alloca[10 X double]
   call initializeRandomValue(%a)
   
   %0 = bitcast[10 X double]* %a to i8* // removed
   
   call llmv.memcopy(%0,@str) // removed
   
   


