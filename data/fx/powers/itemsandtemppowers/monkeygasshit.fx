#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Root
End

LifeSpan	70

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StoneDustRingHit
		Type	start
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		Part	:TearGas01.part
		Part1	:TearGasFlat.part
		#Part2	:FaultDust2Later.part
		#Part3	:FaultDustFlatLater.part
		#Part4	:FaultRadiusRocks.part
		#Part	:FaultDustCenter.part
		#Part	:FaultDust2Center.part
		#Sound	miasma_loop 50 40 .34
		LifeSpan	30
	End

End


End
##################################

