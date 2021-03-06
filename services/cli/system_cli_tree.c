/*
 * uMeshFw Command Line Interface configuration tree
 *
 * Copyright (C) 2017, Marek Koza, qyx@krtko.org
 *
 * This file is part of uMesh node firmware (http://qyx.krtko.org/projects/umesh)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/** @todo move */
#define INTERFACE_POWER
/*#define TESTS*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "u_assert.h"
#include "u_log.h"
#include "hal_module.h"

#include "system_cli_tree.h"

#include "ucli_interface_mac.c"
//~ #include "config_tree/ucli_umesh_ke.c"
#include "ucli_umesh_l2_keymgr.c"
#include "ucli_umesh_l2_filetransfer.c"
#include "ucli_umesh_nbtable.c"
#include "ucli_umesh.c"
#if defined(TESTS)
	#include "ucli_tools_tests.c"
#endif
#include "ucli_tools_crypto.c"
#include "ucli_device_power.c"
#include "ucli_system_log.c"
#include "ucli_system_memory.c"
#include "ucli_system_processes.c"
#include "ucli_system.c"
#include "ucli_files.c"
#include "device_sensor.c"
#include "device_cellular.c"

#include "service_data_process.h"
#include "device_uxb.h"
#include "system_bootloader.h"


const struct treecli_node *system_cli_tree = Node {
	Name "/",
	Subnodes {
		Node {
			Name "files",
			Commands {
				Command {
					Name "copy",
					.exec = ucli_files_copy,
				},
				End
			},
		},
/*
		&(struct treecli_node) {
			.name = "umesh",
			.subnodes = &ucli_umesh_nbtable,
		},
*/
		Node {
			Name "tools",
			Subnodes {
				#if defined(TESTS)
				Node {
					Name "tests",
					Commands {
						Command {
							Name "ftreceive",
							Exec ucli_tools_tests_ftreceive,
						},
						Command {
							Name "ftsend",
							Exec ucli_tools_tests_ftsend,
						},
						Command {
							Name "all",
							Exec ucli_tools_tests_all,
						},
						Command {
							Name "l2send",
							Exec ucli_tools_tests_l2send,
						},
						Command {
							Name "l2receive",
							Exec ucli_tools_tests_l2receive,
						},
						End
					},
				},
				#endif
				Node {
					Name "crypto",
					Commands {
						Command {
							Name "benchmark",
							Exec ucli_tools_crypto_benchmark,
						},
						End
					},
				},
				End

			}
		},
		Node {
			Name "device",
			Subnodes {
				#if defined(INTERFACE_POWER)
				Node {
					Name "power",
					Commands {
						Command {
							Name "print",
							Exec ucli_device_power_print,
						},
						End
					},
				},
				#endif
				Node {
					Name "sensor",
					Commands {
						Command {
							Name "print",
							Exec device_sensor_print,
						},
						End
					},
				},
				Node {
					Name "cellular",
					Commands {
						Command {
							Name "print",
							Exec device_cellular_print,
						},
						End
					},
					DSubnodes {
						DNode {
							Name "cellularN",
							.create = device_cellular_cellularN_create,
						}
					}
				},
				Node {
					Name "uxb",
					Commands {
						Command {
							Name "print",
							Exec device_uxb_print,
						},
						End
					},
				},
				End
			},
		},
		Node {
			Name "service",
			Subnodes {
				Node {
					Name "data-process",
					Commands {
						Command {
							Name "print",
							Exec service_data_process_print,
						},
						Command {
							Name "export",
							Exec service_data_process_print,
						},
						End

					},
					Subnodes {
						Node {
							Name "sensor-source",
							Commands {
								Command {
									Name "add",
									Exec service_data_process_sensor_source_add,
								},
								Command {
									Name "print",
									Exec service_data_process_sensor_source_print,
								},
								Command {
									Name "export",
									Exec service_data_process_sensor_source_export,
								},
								End
							},
							DSubnodes {
								DNode {
									Name "nodeN",
									.create = service_data_process_sensor_source_create,
								},
								End
							},
						},
						Node {
							Name "statistics-node",
							Commands {
								Command {
									Name "add",
								},
								Command {
									Name "print",
								},
								Command {
									Name "export",
								},
								End
							},
							DSubnodes {
								DNode {
									Name "nodeN",
									.create = service_data_process_statistics_node_create,
								},
								End
							},
						},
						Node {
							Name "log-sink",
							Commands {
								Command {
									Name "add",
									Exec service_data_process_log_sink_add,
								},
								Command {
									Name "print",
								},
								Command {
									Name "export",
								},
								End
							},
							DSubnodes {
								DNode {
									Name "nodeN",
									.create = service_data_process_log_sink_create,
								},
								End
							},
						},
						End,
					}
				},
				End

			}
		},
		Node {
			Name "interface",
		},
		Node {
			Name "system",
			Subnodes {
				Node {
					Name "memory",
					Commands {
						Command {
							Name "print",
							Exec ucli_system_memory_print,
						},
						End
					},
				},
				Node {
					Name "processes",
					Commands {
						Command {
							Name "print",
							Exec ucli_system_processes_print,
						},
						Command {
							Name "monitor",
							Exec ucli_system_processes_monitor,
						},
						End
					},
				},
				Node {
					Name "log",
					Commands {
						Command {
							Name "print",
							Exec ucli_system_log_print,
						},
						End
					},
				},
				Node {
					Name "bootloader",
					Subnodes {
						Node {
							Name "config",
							Commands {
								Command {
									Name "load",
									Exec system_bootloader_config_load,
								},
								Command {
									Name "save",
									Exec system_bootloader_config_save,
								},
								Command {
									Name "print",
									Exec system_bootloader_config_print,
								},
								End
							},
							Values {
								Value {
									Name "host",
									.set = system_bootloader_config_host_set,
									Type TREECLI_VALUE_STR,
								},
								Value {
									Name "fw-working",
									.set = system_bootloader_config_fw_working_set,
									Type TREECLI_VALUE_STR,
								},
								Value {
									Name "fw-request",
									.set = system_bootloader_config_fw_request_set,
									Type TREECLI_VALUE_STR,
								},
								Value {
									Name "console-speed",
									.set = system_bootloader_config_console_speed_set,
									Type TREECLI_VALUE_UINT32,
								},
								End
							},
						},
						End
					}
				},
				End
			},
			Commands {
				Command {
					Name "version",
					Exec ucli_system_version,
				},
				Command {
					Name "reboot",
					Exec ucli_system_reboot,
				},
				Command {
					Name "poweroff",
					Exec ucli_system_poweroff,
				},
				End
			},
		},
		End
	}
};

