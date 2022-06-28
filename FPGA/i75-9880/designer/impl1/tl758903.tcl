# Created by Microsemi Libero Software 11.9.2.1
# Tue Jun 28 11:17:04 2022

# (OPEN DESIGN)

open_design "tl758903.adb"

# set default back-annotation base-name
set_defvar "BA_NAME" "tl758903_ba"
set_defvar "IDE_DESIGNERVIEW_NAME" {Impl1}
set_defvar "IDE_DESIGNERVIEW_COUNT" "1"
set_defvar "IDE_DESIGNERVIEW_REV0" {Impl1}
set_defvar "IDE_DESIGNERVIEW_REVNUM0" "1"
set_defvar "IDE_DESIGNERVIEW_ROOTDIR" {W:\W221000\W221131\10-9320\Electrical\FPGA\i75-9880\designer}
set_defvar "IDE_DESIGNERVIEW_LASTREV" "1"


# import of input files
import_source  \
-format "edif" -edif_flavor "GENERIC" -netlist_naming "VHDL" {../../synthesis/tl758903.edn} -merge_physical "yes" -merge_timing "yes"
compile
report -type "status" {tl758903_compile_report.txt}
report -type "pin" -listby "name" {tl758903_report_pin_byname.txt}
report -type "pin" -listby "number" {tl758903_report_pin_bynumber.txt}

save_design
