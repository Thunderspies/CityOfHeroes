#############################################################
## Vengeance_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Eyes
		part1	:LVContinuingGlowLrg01.part
		part2	:LVContinuingGlowSml01.part
		part3	:LVContinuingRays01.part
		part4	:LVContinuingRays02.part
		pmagnet Head
		Sound leaderglow_loop 40 40 .3
		#Lifespan 70

	End

End

Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	Local
		At	Eyes
		part1	:LVContinuingStarLrg01.part
		pmagnet Head
		Sound heall 70 70 .8
		#Lifespan 65

	End

End


End