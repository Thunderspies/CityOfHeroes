#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

#########################################################
Condition
	On Time 
	Time 3
	
	Event
		Type	Local 
		At	chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Frost.part
		Part	:LargeFrost.part
		part1	:bodychillbigBehind.part

		LifeSpan	10
	End
	
	Event
		Type	Local 
		At	CHEST
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbigBehind.part
		part1   :SnowBurst.part
		LifeSpan	20
	End

	Event
		Type	Local 
		At	Hips
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbigBehind.part
		part1   :SnowBurst.part
		LifeSpan	20
	End


	Event
		Type	Local 
		At	UARML
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbigBehind.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

	Event
		Type	Local 
		At	UARMR
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbigBehind.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

	Event
		Type	Local 
		At	LARMR
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbigBehind.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	:icearmor.bhvr
		part1   :bodychillbigBehind.part
		part1   :SnowBurst.part
		LifeSpan	20

	End

		Event
		Type	Local 
		At	WepL
		part1   :Snowfall.part
		LifeSpan	40
	End

		Event
		Type	Local 
		At	WepR
		part1   :Snowfall.part
		LifeSpan	40

	End
End

######################################################3

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
		Sound clawfollow3 100 100 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		Part2	:FollowUpRing01.part
		Part3	:FollowUpRing01a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		Part3	:FollowUpRing02.part
		Part4	:FollowUpRing02a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
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
		Part2	:FollowUpRingSlow.part
		Part3	:FollowUpRing03.part
		Part4	:FollowUpRing03a.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
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

	