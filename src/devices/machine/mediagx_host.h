// license: BSD-3-Clause
// copyright-holders: Angelo Salese

#ifndef MAME_MACHINE_MEDIAGX_HOST_H
#define MAME_MACHINE_MEDIAGX_HOST_H

#pragma once

#include "pci.h"

class mediagx_host_device : public pci_host_device
{
public:
	template <typename T> mediagx_host_device(
		const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock,
		T &&cpu_tag, int ram_size
	) : mediagx_host_device(mconfig, tag, owner, clock)
	{
		set_ids(0x10780001, 0x00, 0x060000, 0x00);
		//set_multifunction_device(true);
		m_host_cpu.set_tag(std::forward<T>(cpu_tag));
		set_ram_size(ram_size);
	}

	mediagx_host_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	void set_ram_size(int ram_size) { m_ram_size = ram_size; }

protected:
	virtual void device_start() override;
	virtual void device_reset() override;

	virtual void map_extra(uint64_t memory_window_start, uint64_t memory_window_end, uint64_t memory_offset, address_space *memory_space,
						   uint64_t io_window_start, uint64_t io_window_end, uint64_t io_offset, address_space *io_space) override;

	virtual space_config_vector memory_space_config() const override;
	virtual void config_map(address_map &map) override;
private:
	enum
	{
		//AS_PCI_MEM = 1,
		AS_PCI_IO = 2
	};
	required_device<cpu_device> m_host_cpu;
	std::vector<uint32_t> m_ram;
	address_space_config  m_superio_space_config;

	void superio_map(address_map &map);
	u8 superio_if_r(offs_t offset);
	void superio_if_w(offs_t offset, u8 data);
	u8 m_superio_index = 0;
	bool m_superio_lock = false;

	struct {
		u8 gcr = 0;
	}m_superio;

	int m_ram_size = 0;
	u8 m_pci_control[2]{};
	u8 m_pci_arbitration[2]{};

	void gxbase_map(address_map &map);
};

DECLARE_DEVICE_TYPE(MEDIAGX_HOST, mediagx_host_device)

#endif