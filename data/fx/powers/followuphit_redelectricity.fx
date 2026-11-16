#########################################################
##	template

FxInfo


Condition
	On Time 
	Time 0

	Repeat	8
	
#	Event
#		EName	Sparkfar
#		Type 	Start
#		At	Chest
#		ChildFX	V_COV\PhysicsEnabled\Red_ElectricSparkHit.fx
#
#		Lifespan	190
#		
#	End
		
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\RedElectricSparkSmall.fx

		Lifespan	90
		
	End
End

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part1	:FollowUpGlow.part
		Part2	:FollowUpSpecks2.part
		Sound elecfollow2 50 50 .8
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	Pow2
		Type 	local
		At	Chest
		Part	Powers\ElectricityControl\Red_ElectricityArchLarge2WhiteA.part
		Part	:Red_ElectricityArchLarge2A.part
		Part	:Red_ElectricityArchLargeA.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	37
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

	