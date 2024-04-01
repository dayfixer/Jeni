// Get driver version and class for devices with specific interface GUIDs using Windows SetupAPI

#include <iostream>
#include <vector>
#include <string>

#include <windows.h>
#include <initguid.h>
#include <devpkey.h>      // need Windows SDK
#include <setupapi.h>

#pragma comment(lib, "setupapi.lib")

#ifdef INITGUID

// {d1304ec6-1117-41c6-a270-6e87d76d87a5}
DEFINE_GUID(GUID_DEVINTERFACE_XDMA, 0xd1304ec6, 0x1117, 0x41c6, 0xa2, 0x70, 0x6e, 0x87, 0xd7, 0x6d, 0x87, 0xa5);

// {813cd898-e3b5-4efc-ae7b-6da6aa0720c2}
DEFINE_GUID(GUID_DEVINTERFACE_Coax, 0x813cd898, 0xe3b5, 0x4efc, 0xae, 0x7b, 0x6d, 0xa6, 0xaa, 0x07, 0x20, 0xc2);
#endif

bool printInfo(const GUID* guid)
{
	auto GetDevicePropString = [](HDEVINFO hDevInfo, SP_DEVINFO_DATA& devInfo, const DEVPROPKEY& key)->std::wstring {
		DEVPROPTYPE type = 0;
		DWORD cb = 0;
		SetupDiGetDevicePropertyW(hDevInfo, &devInfo, &key, &type, nullptr, 0, &cb, 0);
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER && cb > 0)
		{
			std::vector<BYTE> buf(cb);
			if (SetupDiGetDevicePropertyW(hDevInfo, &devInfo, &key, &type, buf.data(), cb, &cb, 0))
			{
				if (type == DEVPROP_TYPE_STRING)
				{
					return std::wstring(reinterpret_cast<const wchar_t*>(buf.data()));
				}
			}
		}
		return L"";
		};

	HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, nullptr, nullptr, DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		std::wcerr << L"SetupDiGetClassDevsW failed: " << GetLastError() << std::endl;
		return false;
	}

	for (DWORD index = 0;; ++index)
	{
		SP_DEVINFO_DATA DeviceInfoData;
		ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		if (!SetupDiEnumDeviceInfo(hDevInfo, index, &DeviceInfoData))
		{
			if (GetLastError() == ERROR_NO_MORE_ITEMS)
				break;
			std::wcerr << L"SetupDiEnumDeviceInfo failed: " << GetLastError() << std::endl;
			break;

		}
		auto drvCls1 = GetDevicePropString(hDevInfo, DeviceInfoData, DEVPKEY_Device_Class);
		auto drvVer1 = GetDevicePropString(hDevInfo, DeviceInfoData, DEVPKEY_Device_DriverVersion);
		std::wcout << L"drvCls1: " << drvCls1 << L"  drvVer1: " << drvVer1 << std::endl << std::endl;
	}
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return false;

}

int main()
{
	printInfo(&GUID_DEVINTERFACE_XDMA);
	printInfo(&GUID_DEVINTERFACE_Coax);
}