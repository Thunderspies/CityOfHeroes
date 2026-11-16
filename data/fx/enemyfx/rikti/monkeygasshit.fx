#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Root
End

######################################################################


Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At	Root
		Sound grexplo4 60 60 .66
		End
End



Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		Part	:MonkeyGas01.part
		Part1	:MonkeyGasFlat.part
		Part	:TearGas01.part
		Part1	:TearGasFlat.part
		Sound miasma_loop 50 50 .25

	
	End

End


End
##################################

