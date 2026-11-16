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
		#Part1	:InfuseLight2.part
		Part1	:ScreechHitglow.part
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
	Time 30

	Event
		EName	All
		Type	Destroy
	End
End


End			