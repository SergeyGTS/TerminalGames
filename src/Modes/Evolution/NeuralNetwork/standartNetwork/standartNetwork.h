#ifndef STANDART_NETWROK_H
#define STANDART_NETWORK_H

#include <fstream>
#include <iostream>
#include "standartNeuron.h"
#include <Evolution/Field/evoField.h>
#include <NeuralNetwork/network.h>

class StandartNetwork : public Network
{
    public:
        StandartNetwork();
        void deleteNetwork();
        
        bool test = true;

        #ifdef LOGS
            EvoField field;
        #endif
        
        float useMind(EvoField evoField, int headX, int headY);

        struct NeuronsList {
            StandartNeuron currentNeuron;
            struct NeuronsList *nextNeuron; 
        };
        NeuronsList* firstLayer = new NeuronsList;
        NeuronsList* lastLayer = new NeuronsList;
        NeuronsList* tempLayer = new NeuronsList;
        NeuronsList* tempSecondLayer = new NeuronsList;
        NeuronsList* nextLayer = new NeuronsList;
        NeuronsList* outputLayer = new NeuronsList;

        NeuronsList *layerParentOne;
        NeuronsList *layerParentTwo;

        struct  LayersList {
            int layerId;
            NeuronsList currentNeuronsList;
            struct LayersList *nextLayer;
        };
        LayersList* layersList = new LayersList;
        LayersList* layersListTmp = new LayersList;
        LayersList* layersListSecondTmp = new LayersList;
        LayersList* layersListParentOne = new LayersList;
        LayersList* layersListParentTwo = new LayersList;


        void mergeNetworks(struct LayersList *parentOne, struct LayersList *parentTwo);


        float randFloat;


        void tests();

    private:
        void initNetwork();
        void initLayers();
        void initNeurons();
        void neuronActivity();
        void testNetwork();

};

#endif