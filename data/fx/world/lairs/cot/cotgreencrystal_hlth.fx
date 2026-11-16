#########################################################
##	CoT Green Crystal Health Buff
#########################################################

FxInfo

#########################################################

Condition
	On 	cycle
	Time 	20
	#Chance	1
	
	Event
		EName	1a
		Type	Local
		At	root
		part1	:CotGreenCrystalAOERing.part
		part2	:CotGreenCrystalAOERing2.part
		part3	:CotGreenCrystalSparkles.part
		part4	:CotGreenCrystalMist1.part
		Sound cotcrystheal3_loop 60 60 .3
		LifeSpan	165
	End
	
#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:GreenCrystalRing.bhvr
#		splat	Generic_ring
#		
#		LifeSpan	15
#	End
#
#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:GreenCrystalRing.bhvr
#		splat	electricring1copy
#		
#		LifeSpan	15
#	End
End

#########################################################

End