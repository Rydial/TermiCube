#ifndef ENTITYCOMPONENTS_H
#define ENTITYCOMPONENTS_H


namespace TC::EC { /* Entity Component */

    /* Forward Declarations */
    struct Velocity;
    

    /* Type Aliases */
    using Vel = Velocity;


    struct Position {
        size_t z, y, x;
        /* Overloaded Operators */
        Position operator+(const Vel &vel) const;
    };
    /* Type Aliases */
    using Pos = Position;


    struct Sprite {
        size_t id;
    };


    struct Velocity {
        int z, y, x;
    };

}


#endif // ENTITYCOMPONENTS_H