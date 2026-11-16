#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan		80

########################################################
#Audio
########################################################
Condition
	On 	Time
	Time 	0

	Event
		
		Type	Local
		At	chest
		Sound daemon_loop 66 66 .8
		
	End

End



########################################################
#Hips
########################################################




Condition
	On 	Time
	Time 	0

	Event
		EName 	flash
		Type	Local
		At	chest
		
		Part1	:Pet_smoke.part
		Part4	:PET_SpectreRips.part
		Part5	:PET_SpectreRips.part
		Part5	:PET_SpectreRips.part
		Part5	:PET_SpectreRips.part
	End

	

	

	Event
		EName 	flash
		Type	Local
		At	chest
		
		Part	:PET_SpectreRipsLocal.part
		Part	:PET_SpectreRipsLocal.part
		Part	:PET_SpectreRipsLocal.part
		Part	:PET_SpectreRipsLocal.part
		Part	:PET_SpectreRipsLocal.part
		Part	:PET_SpectreRipsLocal.part
	End

End

End
##################################

