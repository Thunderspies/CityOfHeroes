#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	220

##################################

Condition
	On	Cycle
	Time	10
	Chance	.8

	Event
		EName	splat
		Type	start
		At	Root
		Splat	Splat_ElectricRing.tga	
		Bhvr	:GlacierRing.bhvr
		LifeSpan	8
	End
End


End			