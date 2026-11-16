#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	CustomizeablePowers\MartialArts\FollowUpGlow.part
		Part2	CustomizeablePowers\MartialArts\FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 10
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part2	CustomizeablePowers\MartialArts\FollowUpRing01.part
		Part3	CustomizeablePowers\MartialArts\FollowUpRing01a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 20
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks.part
		Part3	CustomizeablePowers\MartialArts\FollowUpRing02.part
		Part4	CustomizeablePowers\MartialArts\FollowUpRing02a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
	End

End

Condition
	On Time 
	Time 30
	
	Event
		EName	Pow
		Type 	local
		At	Chest
		#Part1	:FollowUpSpecks2.part
		Part2	CustomizeablePowers\MartialArts\FollowUpRingSlow.part
		Part3	CustomizeablePowers\MartialArts\FollowUpRing03.part
		Part4	CustomizeablePowers\MartialArts\FollowUpRing03a.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		
	End

End


Condition
	On Time 
	Time 13

	Event
		EName	pow2
		Type 	Destroy
			
	End

End

Condition
	On Time 
	Time 75

	Event
		EName	all
		Type 	Destroy
			
	End

End

	

End	

	