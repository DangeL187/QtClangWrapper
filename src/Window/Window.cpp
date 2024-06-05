#include "Window.hpp"

#include "Application.hpp"

Window::Window(Application* parent): _parent(parent) {}

void Window::resizeEvent(QResizeEvent* event) {
    _parent->updateSize(size());
}
