#pragma once

/**
 * Represents a time step in seconds. This is used for calculating the time between frames and for updating game logic.
 * It provides utility functions to get the time in different units (hours, minutes, seconds, milliseconds).
 */
class Timestep {
public:
	/**
	 * Constructs a Timestep with the given time in seconds.
	 * If no time is provided, it defaults to 0.0f.
	 * 
	 * @param time The time in seconds.
	 */
	Timestep(float time = 0.0f)
		: m_Time(time)
	{}

	/// @brief Allows implicit conversion to float, returning the time in seconds.
	operator float() const { return m_Time; }

	/// @brief Returns the time in hours.
	float GetHours() const { return m_Time / 3600.0f; }

	/// @brief Returns the time in minutes.
	float GetMinutes() const { return m_Time / 60.0f; }

	/// @brief Returns the time in seconds.
	float GetSeconds() const { return m_Time; }

	/// @brief Returns the time in milliseconds.
	float GetMilliseconds() const { return m_Time * 1000.0f; }
private:
	/// @brief The time in seconds.
	float m_Time;
};