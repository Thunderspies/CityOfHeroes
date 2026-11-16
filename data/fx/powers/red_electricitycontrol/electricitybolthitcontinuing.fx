#########################################################
##	EBlast
########################################################
FxInfo

Input  
	Inpname	Chest
End

#LifeSpan	120

#################################################################
##Hands
#################################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		Sound eleccage_loop 70 70 .2
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 390
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	:ElectricityGlow.part
		Part2	:ElectricityGlowStar.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound elecballhit2 50 50 .33
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
		#Sound	eleccage_loop 70 60 .1
	End
End

	Condition
	On 	Time
	Time 	21
	Event
		EName 	HandGlow1
		Type	Destroy
				
	End

End



End
