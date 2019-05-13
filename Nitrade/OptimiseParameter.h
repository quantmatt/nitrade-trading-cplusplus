#pragma once
#include <iostream>
#include <memory>

namespace Nitrade {
	class OptimiseParameter
	{
	private:
		std::string _name;
		double _min;
		double _max;
		double _step;
		double _defaultValue;

	public:
		OptimiseParameter() = default;
		OptimiseParameter(std::string name, double min, double max, double step, double defaultValue) :
			_name(name), _min(min), _max(max), _step(step), _defaultValue(defaultValue) {}

		virtual ~OptimiseParameter() = default;

		std::unique_ptr<double[]> getValues();


	};
}

