#pragma once

#include "Core/Timestep.h"

/**
 * Base class for application layers.
 * 
 * Layers are used to organize different parts of the application, such as rendering, UI, and game logic.
 * Each layer can be attached or detached from the application, and can receive updates and UI rendering calls.
 * 
 * Derived classes can override the virtual methods to implement specific behavior for each layer.
 */
class Layer {
public:
	/**
	 * Virtual destructor to ensure proper cleanup of derived classes.
	 */
	virtual ~Layer() = default;

	/**
	 * Called when the layer is attached to the application. This is where you can initialize resources or set up state for the layer.
	 * By default, this method does nothing, but derived classes can override it to perform specific actions when the layer is attached.
	 * 
	 * @note This method is called once when the layer is added to the application, and will not be called again unless the layer is detached and re-attached.
	 */
	virtual void OnAttach() {}

	/**
	 * Called when the layer is detached from the application. This is where you can clean up resources or reset state for the layer.
	 * By default, this method does nothing, but derived classes can override it to perform specific actions when the layer is detached.
	 * 
	 * @note This method is called once when the layer is removed from the application, and will not be called again unless the layer is re-attached.
	 */
	virtual void OnDetach() {}

	/**
	 * Called every frame to update the layer. The `Timestep` parameter provides the time elapsed since the last update, which can be used for time-based calculations.
	 * By default, this method does nothing, but derived classes can override it to implement specific behavior that needs to be updated every frame, such as game logic, animations, or physics.
	 * 
	 * @param ts The time elapsed since the last update, in seconds. This can be used to ensure that updates are consistent regardless of frame rate.
	 * 
	 * @note This method is called every frame while the layer is attached to the application, and will not be called if the layer is detached.
	 */
	virtual void OnUpdate(Timestep ts) {}

	/**
	 * Called every frame to render the layer's UI.
	 * By default, this method does nothing, but derived classes can override it to implement specific UI rendering behavior using a library like ImGui.
	 * 
	 * @note This method is called every frame while the layer is attached to the application, and will not be called if the layer is detached.
	 */
	virtual void OnUIRender() {}
};