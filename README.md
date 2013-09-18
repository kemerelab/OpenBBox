#OpenBBox

The Open Behavior Box was created as an open-source solution for behavior training and analyze of rats and another types of animals. The OpenBBox was design to provide a generic controller to behavior task and support to multiple video stream over the network. 

    git clone https://github.com/nelrice/OpenBBox.git

## Architecture

The OpenBBox is a client/server application, that is write in QT to achieve a cross-platform solution. Using the server application, called **OpenBBox Manager**, we can dynamically discovery new nodes in the network and manage this nodes by controlling behavior task, storing information and visualizing video streams. The clients, called OpenBBOx Nodes, are an embedded Linux platform capable of controlling multiple peripheral devices to performance a task and send video stream over the network. **With this architecture we achieve a customizable and low cost solution.**

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
    git checkout -b features/NAME_OF_FEATURE

#### Push to origin

    git push origin features/NAME_OF_FEATURE
    
### Merge with develop

Always make a **Merge Resquest** to merge the **features/NAME_OF_FEATURE** to **develop** branch.

## Contributors

The project is supported by the Realtime Neuroengineering Lab at Rice Univesity. 

## Materials

## Versions

## Links

[Realtime Neuroengineering Lab](http://nel.rice.edu/)
