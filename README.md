# General Aspects Regarding the Source Code

- The comments refer to the section below them and provide a general description of how the program functions.
- I did not include checks to ensure memory was correctly allocated using sequences like `(if(adr_pointer_malloc != NULL))` because I focused on simplifying the logic and ensuring a smooth flow for the program. However, in a more robust system, adding such checks would be recommended.
- I used local variables with short names to temporarily store values of long-named variables. This was done to make the code as clean and readable as possible while still adhering to coding style conventions. At the same time, variable names remain descriptive enough for anyone reading the code.
- The comments are in Romanian, while the variable names are in English, making the code accessible for international readers while maintaining clarity in my native language.

# Aspects Regarding Dynamic Memory Allocation

- I chose to allocate and deallocate memory one element at a time whenever a filter or an image is introduced, progressing step by step. Although doubling the allocated memory each time it fills up or halving it when half is free could be more efficient, this approach would have required tracking the number of free spaces, complicating the code unnecessarily for this project.
- I developed a few auxiliary functions for allocating and deallocating memory, which streamline the code by avoiding redundancy in allocation and deallocation operations.

# Implementing Functions for Core Features

- I documented how each function works by commenting on the source code.
- When functions modified the dimensions of matrices, I opted to allocate new matrices. These functions return the address of the new matrix and delete the old one.
- The formulas used in the code were designed manually ("pen and paper"), starting from a specific case with small matrices, ensuring correctness and clarity.

# Implementing the Interactive Feature

- I decided to use an infinite loop with `while(1)` for the interactive feature. The commands are implemented using functions created in earlier sections, except for functions like `exit`, `load`, `save`, `create filter`, `delete filter`, and `delete image`, which are directly handled in the `main()` function.

# Final Conclusions

- I deliberately limited the scope to using only the fundamental concepts of functions, vectors, matrices, and dynamic memory allocation, without using more complex structures like `struct` for images and filters.
- The project was both an interesting and engaging challenge. It provided a practical application of fundamental programming concepts, enhancing its value as a learning experience.
