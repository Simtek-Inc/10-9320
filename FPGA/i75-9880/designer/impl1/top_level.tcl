# Created by Microsemi Libero Software 11.9.2.1
# Tue Jun 28 11:44:29 2022

# (NEW DESIGN)

# create a new design
new_design -name "top_level" -family "ProASIC3"
set_device -die {A3PN250} -package {100 VQFP} -speed {STD} -voltage {1.5} -IO_DEFT_STD {LVCMOS 3.3V} -RESERVEMIGRATIONPINS {1} -RESTRICTPROBEPINS {1} -RESTRICTSPIPINS {0} -TARGETDEVICESFORMIGRATION {UM4X4M1N} -TEMPR {COM} -UNUSED_MSS_IO_RESISTOR_PULL {None} -VCCI_1.5_VOLTR {COM} -VCCI_1.8_VOLTR {COM} -VCCI_2.5_VOLTR {COM} -VCCI_3.3_VOLTR {COM} -VOLTR {COM}


# set default back-annotation base-name
set_defvar "BA_NAME" "top_level_ba"
set_defvar "IDE_DESIGNERVIEW_NAME" {Impl1}
set_defvar "IDE_DESIGNERVIEW_COUNT" "1"
set_defvar "IDE_DESIGNERVIEW_REV0" {Impl1}
set_defvar "IDE_DESIGNERVIEW_REVNUM0" "1"
set_defvar "IDE_DESIGNERVIEW_ROOTDIR" {C:\_SWork\Local_Repos\10-9320\FPGA\i75-9880\designer}
set_defvar "IDE_DESIGNERVIEW_LASTREV" "1"

# set working directory
set_defvar "DESDIR" "C:/_SWork/Local_Repos/10-9320/FPGA/i75-9880/designer/impl1"

# set back-annotation output directory
set_defvar "BA_DIR" "C:/_SWork/Local_Repos/10-9320/FPGA/i75-9880/designer/impl1"

# enable the export back-annotation netlist
set_defvar "BA_NETLIST_ALSO" "1"

# set EDIF options
set_defvar "EDNINFLAVOR" "GENERIC"

# set HDL options
set_defvar "NETLIST_NAMING_STYLE" "VHDL93"

# setup status report options
set_defvar "EXPORT_STATUS_REPORT" "1"
set_defvar "EXPORT_STATUS_REPORT_FILENAME" "top_level.rpt"

# legacy audit-mode flags (left here for historical reasons)
set_defvar "AUDIT_NETLIST_FILE" "1"
set_defvar "AUDIT_DCF_FILE" "1"
set_defvar "AUDIT_PIN_FILE" "1"
set_defvar "AUDIT_ADL_FILE" "1"

# import of input files
import_source  \
-format "edif" -edif_flavor "GENERIC" -netlist_naming "VHDL" {../../synthesis/top_level.edn}

# save the design database
save_design {top_level.adb}


compile
report -type "status" {top_level_compile_report.txt}
report -type "pin" -listby "name" {top_level_report_pin_byname.txt}
report -type "pin" -listby "number" {top_level_report_pin_bynumber.txt}

save_design
