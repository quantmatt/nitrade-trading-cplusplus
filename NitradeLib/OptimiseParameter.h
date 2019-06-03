#pragma once
#include <iostream>
#include <memory>
#include <vector>

namespace Nitrade {
	class OptimiseParameter
	{
	private:
		std::string _name{""};
		double _min{ 0 };
		double _max{ 0 };
		double _step{ 0 };
		double _defaultValue{ 0 };

	public:
		OptimiseParameter() = default;
		OptimiseParameter(std::string name, double min, double max, double step, double defaultValue) :
			_name(name), _min(min), _max(max), _step(step), _defaultValue(defaultValue) {}

		virtual ~OptimiseParameter() = default;

		std::vector<double> getValues();
		std::string getName();


	};
}

