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
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:FireHandz.part
		LifeSpan	32
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:FireHandz.part
		LifeSpan	32
	
	End

######################################################3

	Event
		Type	Local
		At	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End
	
	Event
		Type	Local
		At	head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	SpadR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	SpadL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	UarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	UarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End
	
	Event
		Type	Local
		At	Hips
		Part	:HealFirehips.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	LarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	LarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:FireBuildupSparks.part
		LifeSpan	30
	End
End

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

	