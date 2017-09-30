#pragma once
namespace uc {

	class ISink
	{
	public:
		virtual void onNotify(int nType) const = 0;
	};

	class IMgr
	{
	public:
		void setMgrSink(const ISink *sink){ mlpSink = sink; }

		virtual int init() = 0;
		virtual void destroy() = 0;

	protected:
		IMgr(){ mlpSink = (ISink *)0; }

		const ISink *mlpSink;
	};
}
