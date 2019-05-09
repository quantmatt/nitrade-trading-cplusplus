#pragma once
namespace Nitrade {
	class Strategy
	{
	public:
		Strategy() {}
		virtual ~Strategy() {}

		virtual void onInit() {}
		virtual void onBar() {}
	};
}
