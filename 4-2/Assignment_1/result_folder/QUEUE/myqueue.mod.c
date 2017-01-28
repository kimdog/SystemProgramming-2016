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
	{ 0x48ee012f, __VMLINUX_SYMBOL_STR(my_destroyQueue) },
	{ 0x8f472c8, __VMLINUX_SYMBOL_STR(is_myQueue_exist) },
	{ 0xb48fade, __VMLINUX_SYMBOL_STR(kim_myQueue) },
	{ 0xa315faa6, __VMLINUX_SYMBOL_STR(my_createQueue) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "99F509327C327CFFB3C36C0");
