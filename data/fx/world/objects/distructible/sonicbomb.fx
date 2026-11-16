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
	On 	cycle
	Time 	30
	Chance	1

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	:splat2.bhvr
		
		splat	generic_Ring.tga

		#Part1	:GasFlat.part
		#Part	:TearGas01.part
		#Part1	:TearGasFlat.part
		Sound miasma_loop 50 50 .25

		LifeSpan	20
	End

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	:splat3.bhvr
		
		splat	ThinRing.tga

		LifeSpan	20
	End

End


End
##################################

