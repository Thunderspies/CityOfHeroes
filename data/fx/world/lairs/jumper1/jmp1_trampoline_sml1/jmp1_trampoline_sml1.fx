#########################################################
##	jmp1_trampoline_sml1
##
FxInfo

Input  
	InpName	Origin
End


Condition
	On 	Time
	Time 	0
##	Chance	.8

	Event
		EName 	Clouds
		Type	start 
		At	origin
		Bhvr	:scaleup.bhvr
		Part	:jmp1_tramp_sml_main.part
		Part	:jmp1_fil1.part
		Part	:jmp1_tramp_sml_rings.part
		Part	:jmp1_ringglow1.part
	End

End


End	


			