# Todo list 
1. add a variable to check the performance of each operators
2. check whether is improved or not


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
2. Nei_new
    - generate a completely new solution
3. Nei_move_one_to_right 
    - random select one and move its order to the right 
4. Nei_move_one_to_left
    - random select one and move its order to the left
5. Nei_one_point_cross_over
    - one point cross over, swap before and after 

##### todo
1. random move: move one to a random positions 
2. move a group to the left
3. move a group of links to right 
4. partically regenerated part of the links
5. Allow a subpath is formed by connecting two links
    - set origin to be the tail node of one link and dest to be the head node of the other link 
    - then run the shortest path method, to see whether, the distance is changed 
    - I can make this greedy ?

### Other ideas
1. I can design a construction algorithm
- first align all the projects to the same period
- then using some greedy method to move the project starting time
