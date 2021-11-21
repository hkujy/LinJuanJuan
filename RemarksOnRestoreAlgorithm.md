# class structure
## sol
### structure of the solution
1. ordered link: pointer
2. ordered starting time index
### Functions
1. convert the solution representation to the scenario structure


### Heuristic
1. Nei_swap
    - random select two and replace their locations
        - I can randomly select two links and change their
2. Nei_replace
    - generate a complete new solution



2. change a group of links
    - change the number of links start at the same period, and then
    - the group of links may not start at the same period, but only part
3. regenerate a complete new one 
4. partially regenerate some of the links 
5. Greedy ?
6. Repair operators 
    - only delay the projects

### Other ideas
1. I can design a construction algorithm
- first align all the projects to the same period
- then using some greedy method to move the project starting time
