#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On	Time 
	Time	0
	
	Event
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:EndurenceTendrils.part
		Lifespan	30
	End

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
	End
End

Condition
	On	Time 
	Time	10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Part1	:FollowUpSpecks.part
		Part2	:FollowUpRing01.part
		Part3	:FollowUpRing01a.part
	End
End

Condition
	On	Time 
	Time	20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Part1	:FollowUpSpecks.part
		Part3	:FollowUpRing02.part
		Part4	:FollowUpRing02a.part
	End
End

Condition
	On	Time 
	Time	27
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		#Part1	:FollowUpSpecks2.part
		Part2	:FollowUpRingSlow.part
		Part3	:FollowUpRing03.part
		Part4	:FollowUpRing03a.part
	End
End

#############################################################

Condition
	On	Time 
	Time	13

	Event
		EName	pow2
		Type 	Destroy
	End
End

Condition
	On	Time 
	Time	75

	Event
		EName	all
		Type 	Destroy
	End
End

	
#############################################################

End	