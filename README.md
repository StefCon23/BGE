Conor Stefanini
C11443118

I have made an animat of a bird (seagull) that flaps its wings.

The head and the body are spheres
The body has a radius of 5 and the head has a radius of 3.33.
The connection between the head and the body is a fixed constraint.

The legs are cylinders with a radius of 0.5 and a length of 3.33.
The legs are connected to the body by fixed constraints.

The shoulders are cylinders with a radius of about 1.03 and a length of 1.
The powersource is in the seagull's shoulders which the wings are connected to with hinge joints.

The shoulders are connected to the body with hinge constraints.
The shoulders don't rotate from the perspective of the body and rotate about the y axis of the cylinders of the shoulders.
The shoulder cylinders are rotated 90 degrees about the world's x axis, this means the the rotation about the cylinders y axis results in a rotation about the world's z axis.

The wings are connected to the edge of the shoulders by a hinge constraint that rotates about the world's z axis of both the shoulder and wings.

![Front View](https://raw.githubusercontent.com/ConorStefanini/BGE/Assignment/Logo/roughSeagull.png)

