#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	30

Input  
	InpName	head
End

###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		geom	FrontFacingLocalPivot01
		Part2	:ScreechHitglow.part
		Part1	:DisruptionRing.part
	End

	Event
		EName	RingsAndParticles2
		Type	local
		At	Head
		geom	FrontFacingLocalPivot01
		Part1	:ScreechHitBurst.part
	End

	Event
		EName	RingsAndParticles
		Type	start
		At	faceringsRigging
		   altpiv	1		
		Part1	:ScreechHitRingz.part
		Part2	:ScreechHitRings.part

	End

		
End

Condition
	On Time
	Time 15

	Event
		EName	RingsAndParticles
		Type	Destroy
	End

End

Condition
	On Time
	Time 20

	Event
		EName	RingsAndParticles2
		Type	Destroy
	End

End

Condition
	On Time
	Time 30

	Event
		EName	All
		Type	Destroy
	End
End


End			