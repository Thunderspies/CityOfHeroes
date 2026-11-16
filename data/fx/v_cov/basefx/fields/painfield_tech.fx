#########################################################
##	CoT Red Crystal Damage Debuff
##
FxInfo

Input  
	InpName	Root
End

#####################################################################################



Condition
	On 	Cycle
	Time 	30
	Chance	.6
	
	Event
		EName	Prime
		Type	Start
		At	root
		#part1	:PainFieldAOERing.part
		part1	:PainFieldAOERing5.part
		part2	:PainFieldAOERing3.part
		Sound heal3b 60 60 .3
		LifeSpan	165
	End

End

Condition
	On 	Cycle
	Time 	20
		
	Event
		EName	Prime
		Type	Start
		At	root
		BhvrOverride
			PositionOffset 0 2.7 0
		End
		part3	:PainFieldSparks.part
		part4	:PainFieldGlow1.part
		LifeSpan	65
	End

End

Condition
	On 	Cycle
	Time 	10
	
	Event
		EName	ring
		Type	start
		At	Root
		geom	MegaRing2
		bhvr	:BlastRingScale1.bhvr
		LifeSpan		2
	End

End

Condition
	On 	Cycle
	Time 	60
	Chance	.9
	
		
	Event
		EName	Prime
		Type	Start
		At	root
		BhvrOverride
			PositionOffset 0 4 0
		End
		part3	:PainFieldFlash.part
		LifeSpan		5
	End

	Event
		EName	ring
		Type	start
		At	Root
		geom	MegaRing2
		bhvr	:BlastRingScale2a.bhvr
		LifeSpan		2
	End

	Event
		EName	sphere
		Type	start
		At	Root
		geom	CosmicPeaceBringerShield #MegaRing2
		bhvr	:BlastRingScale2.bhvr
		LifeSpan		2
	End

	Event
		EName	sphere2
		Type	start
		At	Root
		geom	CosmicPeaceBringerShield #MegaRing2
		bhvr	:BlastRingScale2.bhvr
		LifeSpan		2
	End

End



End	


			