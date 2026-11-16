#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	InpName	Hips
End

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	local
		At	chest
		Part	:TearGasContinuing.part
		#Part1	:TearGasFlat.part
		Sound soot_loop 60 60 .45

	End

End


End
##################################

