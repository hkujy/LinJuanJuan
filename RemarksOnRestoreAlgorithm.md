# TODO 
1. write the solution to update the neighbour operators 


# Remarks
1. I can reproduce Wang's work. However, there may be some accuracy issue
2. Remark: even for the link with one failure links the recover secquence is not the same
3. To avoid zero denominator, I set the sum of the total count for each operator to be 1
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
    > remark: this is not used in the nei operators but a scout phase for new solutions
3. Nei_move_one_to_right 
    - random select one and move its order to the right 
4. Nei_move_one_to_left
    - random select one and move its order to the left
5. Nei_one_point_cross_over
    - one point cross over, swap before and after 
6. Nei_Insert_One_Random_To_Right
    - randomly select one node and move it to the right
7. Nei_Insert_One_Random_To_Left
    - randomly select one node and move it to the left

##### todo
1. define a proper pair and then move
2. move a group to the left
3. move a group of links to right 
4. Allow a subpath is formed by connecting two links
    - set origin to be the tail node of one link and dest to be the head node of the other link 
    - then run the shortest path method, to see whether, the distance is changed 
    - I can make this greedy ?
5. Different method for updating the adaptive score 
### Other ideas
1. I can design a construction algorithm
- first align all the projects to the same period
- then using some greedy method to move the project starting time
