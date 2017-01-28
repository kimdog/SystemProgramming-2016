#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x5c0c0377, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xcb70bcec, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0xed26343e, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0xbd74f862, __VMLINUX_SYMBOL_STR(my_print_queue) },
	{ 0xb48fade, __VMLINUX_SYMBOL_STR(kim_myQueue) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "6390D9B529F94642E8700DA");
