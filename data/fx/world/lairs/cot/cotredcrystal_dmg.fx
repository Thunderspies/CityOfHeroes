#########################################################
## CoT Red Crystal Damage Debuff
#########################################################

FxInfo

#########################################################

Condition
	On 	Cycle
	Time 	20
	#Chance	1
	
	Event
		EName	Prime
		Type	Local
		At	root
		part1	:CotRedCrystalAOERing.part
		part2	:CotRedCrystalAOERing2.part
		part3	:CotRedCrystalSparkles.part
		part4	:CotRedCrystalMist1.part
		Sound cotcrystbad_loop 20 20 .1
		LifeSpan	165
	End
	

#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:RedCrystalRing.bhvr
#		splat	Generic_ring
#		
#		LifeSpan	15
#	End
#
#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:RedCrystalRing.bhvr
#		splat	electricring1copy
#		
#		LifeSpan	15
#	End
End

#########################################################

End