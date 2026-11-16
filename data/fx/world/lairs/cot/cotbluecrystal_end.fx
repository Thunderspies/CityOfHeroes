#########################################################
## CoT Blue Crystal Endurance Buff
#########################################################

FxInfo

#####################################################################################

Condition
	On 	cycle
	Time 	20
	Chance	1
	
	Event
		EName	1a
		Type	Local
		At	root
		part1	:CotBlueCrystalAOERing.part
		part2	:CotBlueCrystalAOERing2.part
		part3	:CotBlueCrystalSparkles.part
		part4	:CotBlueCrystalMist1.part
		Sound cotcrystalpwr_loop 30 30 .8
		LifeSpan	165
	End

#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:BlueCrystalRing.bhvr
#		splat	Generic_ring
#		
#		LifeSpan	15
#	End
#
#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:BlueCrystalRing.bhvr
#		splat	electricring1copy
#		
#		LifeSpan	15
#	End
End	

#########################################################

End			