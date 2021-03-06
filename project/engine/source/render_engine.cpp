#include "render_engine.hpp"

#include <vector>
#include <algorithm>
#include <utility>

#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "director.hpp"
#include "load_manager.hpp"
#include "camera.hpp"
#include "light.hpp"


bool RenderEngine::loaded = false;
std::map< std::string, RenderElementPtr > RenderEngine::renderElements;
RenderGridPtr RenderEngine::renderGrid = NULL_PTR;
std::map< std::string, int > RenderEngine::renderElementsPriority;

void RenderEngine::setup(){
    INFO("Setup RenderEngine...");

    setupOpenGL();

    loaded = true;
    INFO("RenderEngine set!");
}

bool renderElementCompare(const std::pair<std::string, int> & a, const std::pair<std::string, int> & b){
    if(a.second == b.second){
        return a.first < b.first;
    }
    return a.second < b.second;
}

void RenderEngine::render(){
    renderGrid->clean();

    INFO("Render Engine: rendering objects...");

    ASSERT(loaded, "You dind't load the rendering engine!");

    std::vector< std::pair<std::string, int> > elements(renderElementsPriority.begin(), renderElementsPriority.end());
    std::sort(elements.begin(), elements.end(), renderElementCompare);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < elements.size(); i++){
        if(elements[i].second < 0) continue;

        RenderElementPtr element = renderElements[elements[i].first];

        INFO("Rendering Objects from Render Element " << elements[i].first << "...");

        INFO("Setup Enviroment...")
        element->setupEnviroment();
        INFO("Render Pass...")
        element->renderPass();
        INFO("Tear Down Enviroment...")
        element->tearDownEnviroment();
        }
}

void RenderEngine::addRenderElement(std::string name, RenderElementPtr renderElement, int priority){
    INFO("Adding render element " << name << " to the render engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    INFO("Setting up Render Element...");
    renderElement->setup();

    INFO("Loading the Render Element Shader...");
    renderElement->loadShader();

    renderElements[name] = renderElement;
    renderElementsPriority[name] = priority;
    INFO("Render element " << name << " added!");
}

RenderElementPtr RenderEngine::getRenderElement(std::string name){
    ASSERT(loaded, "You dind't load the rendering engine!");

    if(renderElements.find(name) != renderElements.end()){
        return renderElements[name];
    }

    ASSERT(false, "Render Element " << name << " not found!");
    return NULL_PTR;
}

void RenderEngine::removeRenderElement(std::string name){
    INFO("Removing Render Element " << name << " from Render Engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    for(auto it = renderElements.begin(); it != renderElements.end(); it++){
        if(it->first == name){
            renderElements.erase(it);
            renderElementsPriority.erase(renderElementsPriority.find(name));
            INFO("Render Element Removed!");
            return;
        }
    }

    DEBUG("Could not find Render Element!");
}

void RenderEngine::setRenderGrid(RenderGridPtr _renderGrid) {
    _renderGrid->initialize();
    renderGrid = _renderGrid;
}

RenderGridPtr RenderEngine::getRenderGrid() {
    return renderGrid;
}

void RenderEngine::resetRenderEngine() {
    renderElements.clear();
    renderElementsPriority.clear();
    renderGrid = NULL_PTR;
}
/* Private Methods */

void RenderEngine::setupOpenGL(){
    INFO("Setup OpenGL...");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
}

void RenderEngine::toggleElementsForMenu() {
     for(auto it = renderElements.begin(); it != renderElements.end(); it++){
         if(it->first != "text"){
             renderElementsPriority[it->first] = -renderElementsPriority[it->first];
             INFO("Render Element Toggled");
        }
     }
}
