transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/metr0/code/Digital-Logic-Circuits-1/Ripple-Carry-Adder {C:/Users/metr0/code/Digital-Logic-Circuits-1/Ripple-Carry-Adder/rca_36b.v}

vlog -vlog01compat -work work +incdir+C:/Users/metr0/code/Digital-Logic-Circuits-1/Ripple-Carry-Adder {C:/Users/metr0/code/Digital-Logic-Circuits-1/Ripple-Carry-Adder/tb_rca36.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cyclonev_ver -L cyclonev_hssi_ver -L cyclonev_pcie_hip_ver -L rtl_work -L work -voptargs="+acc"  tb_rca_36b

add wave *
view structure
view signals
run -all
