#include"../../common/CommonDxSupport.hpp"
#include"D3DClass.hpp"
#include<vector>

namespace aMazing
{
	class DeviceManager
	{
	private:
		//private POD object for device management.
		struct DeviceManageObject
		{
			D3DClass* d3dInstance;
		};
	public:
		DeviceManager() = delete;
		static const unsigned int getManagedDeviceCount()
		{
			return managedDevices.size();
		}

		static const ID3D11Device* getCorrespondDevice()
		{
			int index = 0;
			return managedDevices[index].d3dInstance->getDevice();
		}
		static const ID3D11DeviceContext* getCorrespondContext()
		{
			int index = 0;
			return managedDevices[index].d3dInstance->getContext();
		}
	private:
		static std::vector<DeviceManageObject> managedDevices;
	};
}