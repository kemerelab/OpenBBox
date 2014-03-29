# OpenBBox

The Open Behavior Box was created as an open-source solution for behavioral training and real-time analysis of rats and another types of animals. OpenBBox was design to provide a generic controller for behavioral tasks and supports multiple video streams over the network. 

    git clone https://github.com/kemerelab/OpenBBox

![Open BBox Screenshot](/resources/OpenBBox_Screenshot.png "Open BBox Screenshot")

## Architecture

OpenBBox is a client/server application, that is written using the QT library to achieve a cross-platform solution. Using the server application, called **OpenBBox Manager**, we can dynamically discover new nodes in the network and manage these nodes by controlling behavior tasks, storing information and visualizing video streams. The clients, called OpenBBox Nodes, run on the Texas Instruments BeagbleBone Black (http://beagleboard.org/Products/BeagleBone+Black), an embedded Linux platform capable. With an I/O shield, the Nodes can control multiple peripheral devices to perform behavioral task input/output control as well as capturing, synchronizing, and sending behavioral video streams over the network. **With this architecture we achieve a customizable and low cost solution.**

## Projects 

* OpenBBoxManager
    
      * Server
      * QT Desktop application with GUI
    
* OpenBBoxNode
    
      * Client
      * QT application    

## Workflow

The structure used for developing with git is based on [gitflow](http://nvie.com/posts/a-successful-git-branching-model/).

![Workflow](http://nvie.com/img/2009/12/Screen-shot-2009-12-24-at-11.32.03.png "Workflow")

### Commands

#### Create new feature

    #Update master
    git checkout master
    git pull
    #Go to develop
    git checkout develop
    #Give a name to the feature
    git checkout -b feature/NAME_OF_FEATURE

#### Push to origin

    git push origin feature/NAME_OF_FEATURE
    
### Merge with develop

Always make a **Merge Resquest** to merge the **feature/NAME_OF_FEATURE** to **develop** branch.

## Contributors

The project is supported by the Realtime Neuroengineering Lab at Rice Univesity. 

## Materials

## Versions

## Links

[Realtime Neuroengineering Lab](http://nel.rice.edu/)
