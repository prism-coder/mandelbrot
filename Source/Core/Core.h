#pragma once

#include <memory>

/**
 * Smart pointer aliases and factory functions.
 * 
 * These are used to create and manage dynamic memory in a more convenient and safer way.
 */
template<typename T>
using Scope = std::unique_ptr<T>;

/**
 * Factory function for creating a `Scope` (unique pointer).
 * 
 * This function forwards the provided arguments to the constructor of `T` and returns a unique pointer to the created object.
 * Using this function helps to avoid the need to explicitly specify the type when creating a unique pointer, as it can be deduced from the arguments.
 * 
 * @example auto myObject = CreateScope<MyClass>(constructorArg1, constructorArg2);
 */
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

/**
 * Shared pointer aliases and factory functions.
 * 
 * These are used when multiple parts of the code need to share ownership of a resource.
 * The resource will be automatically deallocated when the last owner goes out of scope.
 */
template<typename T>
using Ref = std::shared_ptr<T>;

/**
 * Factory function for creating a `Ref` (shared pointer).
 * 
 * This function forwards the provided arguments to the constructor of `T` and returns a shared pointer to the created object.
 * Similar to `CreateScope`, this function allows for type deduction and simplifies the creation of shared pointers.
 * 
 * @example auto mySharedObject = CreateRef<MyClass>(constructorArg1, constructorArg2);
 */
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}