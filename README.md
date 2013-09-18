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

The structure used for developing with git is based on [Hubflow](http://datasift.github.io/gitflow/GitFlowForGitHub.html).

>Using the scripts/hubflow.sh you can easily install the hubflow in your machine.

## Contributors 
The project is supported by the Realtime Neuroengineering Lab at Rice Univesity. 

## Materials

## Versions

## Links

[Realtime Neuroengineering Lab](http://nel.rice.edu/)
