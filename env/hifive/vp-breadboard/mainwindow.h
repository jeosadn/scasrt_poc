#pragma once

#include "devices/c/all_devices.hpp"
#include "devices/luaEngine.hpp"
#include <gpio/gpio-client.hpp>
#include <QtWidgets/QMainWindow>
#include <cassert>
#include <map>
#include <unordered_map>
#include <list>

#include <mutex> // TODO: FIXME: Create one Lua state per device that uses asyncs like SPI and synchronous pins

namespace Ui {
class VPBreadboard;
}

static constexpr unsigned max_num_buttons = 7;

class VPBreadboard : public QWidget {
	Q_OBJECT
	GpioClient gpio;

	struct SPI_IOF_Request {
		gpio::PinNumber gpio_offs;	// calculated from "global pin"
		gpio::PinNumber global_pin;
		bool noresponse;
		GpioClient::OnChange_SPI fun;
	};
	struct PIN_IOF_Request {
		gpio::PinNumber gpio_offs;	// calculated from "global pin"
		gpio::PinNumber global_pin;
		GpioClient::OnChange_PIN fun;
	};

	// TODO get C-Device factory
	Sevensegment* sevensegment;
	RGBLed* rgbLed;
	OLED_mmap* oled_mmap;
	OLED_iof* oled_iof;
	Button* buttons[max_num_buttons];
	//

	std::mutex lua_access;		//TODO: Use multiple Lua states per 'async called' device
	LuaEngine lua_factory;
	typedef std::string DeviceID;
	std::unordered_map<DeviceID,Device> devices;
	std::unordered_map<DeviceID,SPI_IOF_Request> spi_channels;
	std::unordered_map<DeviceID,PIN_IOF_Request> pin_channels;
	struct DeviceGraphic {
		QImage image;
		QPoint offset;
		unsigned scale;
	};
	std::unordered_map<DeviceID, DeviceGraphic> device_graphics;

	struct PinMapping{
		gpio::PinNumber gpio_offs;	// calculated from "global pin"
		gpio::PinNumber global_pin;
		gpio::PinNumber device_pin;
		std::string name;
		Device* dev;
	};
	std::list<PinMapping> reading_connections;		// Semantic subject to change
	std::list<PinMapping> writing_connections;


	const char* host;
	const char* port;

	bool debugmode = false;
	unsigned moving_button = 0;
	bool connected = false;

	static uint64_t translateGpioToExtPin(gpio::State reg);
	static gpio::PinNumber translatePinToGpioOffs(gpio::PinNumber pin);

	// TODO: Phase these out and decide based on config
	static uint8_t translatePinNumberToSevensegment(uint64_t pinmap);
	static uint8_t translatePinNumberToRGBLed(uint64_t pinmap);

	bool loadConfigFile(std::string file);

public:
	VPBreadboard(std::string configfile,
			const char* host, const char* port,
			std::string additional_device_dir, bool overwrite_integrated_devices = false,
			QWidget* mparent = 0);
	~VPBreadboard();
	void showConnectionErrorOverlay(QPainter& p);
	void paintEvent(QPaintEvent*) override;
	void keyPressEvent(QKeyEvent* e) override;
	void keyReleaseEvent(QKeyEvent* e) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void notifyChange(bool success);
};
