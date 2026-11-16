#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	chest
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	local
		At	chest
		Part	:TearGasContinuing2.part
		#Part1	:TearGasFlat.part

		Sound miasma_loop 60 60 .38
				
	End

End


End
##################################

