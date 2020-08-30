#include <stdint.h>
struct rsdp_descriptor{
	char signature[8];
	uint8_t checksum;
	char oem[6];
	uint8_t revision;
	uint32_t rsdtAddr;
}__attribute__((packed));
typedef struct rsdp_descriptor rsdp_descriptor_t;
struct rsdp_descriptor_2{
	rsdp_descriptor_t firstPart;
	uint32_t length;
	uint64_t xsdtAddr;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
}__attribute__((packed));
typedef struct rsdp_descriptor_2 rsdp_descriptor_t_2;


struct sdt_header{
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oem[6];
	char oem_table[8];
	uint32_t oem_revision;
	uint32_t creator;
	uint32_t creator_rev;
}__attribute__((packed));
typedef struct sdt_header sdt_header_t;

struct rsdt{
        sdt_header_t h;
	uint32_t pointers;
}__attribute__((packed));
typedef struct rsdt rsdt_t;

struct generic_address_structure{
	uint8_t address_space;
	uint8_t bit_width;
	uint8_t bit_offset;
	uint8_t access_size;
	uint64_t address;
}__attribute__((packed));
typedef struct generic_address_structure generic_address_structure_t;
struct fadt{
	sdt_header_t header;
	uint32_t firmware_ctrl;
	uint32_t dsdt;
	uint8_t reserved;
	uint8_t preffered_power_management_profile;
	uint16_t sci_interrupt;
	uint32_t smi_command_port;
	uint8_t acpi_enable;
	uint8_t acpi_disable;
	uint8_t s4_bios_req;
	uint8_t pstate_control;
	uint32_t pm1_a_event_block;
	uint32_t pm1_b_event_block;
	uint32_t pm1_a_control_block;
	uint32_t pm1_b_control_block;
	uint32_t pm2_control_block;
	uint32_t pm_timer_block;
	uint32_t gpe_0_block;
	uint32_t gpe_1_block;
	uint8_t pm1_event_length;
	uint8_t pm1_control_length;
	uint8_t pm2_control_length;
	uint8_t pm_timer_length;
	uint8_t gpe_0_length;
	uint8_t gpe_1_length;
	uint8_t gpe_1_base;
	uint8_t cstate_control;
	uint16_t worst_c2_latency;
	uint16_t worst_c3_latency;
	uint16_t flush_size;
	uint16_t flush_stride;
	uint8_t duty_offset;
	uint8_t duty_width;
	uint8_t day_alarm;
	uint8_t month_alarm;
	uint8_t century;
	
	uint16_t boot_architecture_flags;
	
	uint8_t reserved2;
	uint32_t flags;
	
	generic_address_structure_t reset_reg;
	
	uint8_t reset_value;
	uint8_t reserved3[3];
	
	uint64_t x_firmware_control;
	uint64_t x_dsdt;
	
	generic_address_structure_t x_pm1_a_event_block;
	generic_address_structure_t x_pm1_b_event_block;
	generic_address_structure_t x_pm1_a_control_block;
	generic_address_structure_t x_pm1_b_control_block;
	generic_address_structure_t x_pm2_control_block;
	generic_address_structure_t x_pm_timer_block;
	generic_address_structure_t x_gpe_0_block;
	generic_address_structure_t x_gpe_1_block;
}__attribute__((packed));
typedef struct fadt fadt_t;

rsdt_t* init_acpi();
sdt_header_t* find_acpi_header(rsdt_t* rsdt, char sig[4]);

