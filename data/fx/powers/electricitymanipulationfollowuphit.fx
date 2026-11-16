#########################################################
##	template

FxInfo

Condition
	On 	Time
	Time 	0

	Event
		EName	Sparkfar
		Type 	local
		At	Chest
		
		Part	:EnergyCoreElectricity.part
		Part	:EnergyCoreElectricityInvert.part
		Part	:EnergyCoreElectricity_White.part
		Part	:EnergyCoreElectricityInvert_White.part


		Lifespan	30
		LifeSpanJitter	10
		
	End
End

#######################################################################
##Sparks
#############################################

Condition
	On Time 
	Time 2

	Repeat	2
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkSmall.fx

		Lifespan	120
		LifeSpanJitter	80
		
	End
End

Condition
	On Time 
	Time 2

	Repeat	2

	Event
		HardwareOnly
		EName	Sparkfar2
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkSmall.fx

		Lifespan	120
		LifeSpanJitter	80
		
	End

End

Condition
	On Time 
	Time 2

	Repeat	2

	Event
		HardwareOnly
		EName	Sparkfar2
		Type 	Start
		At	Chest
		ChildFX	V_COV\PhysicsEnabled\ElectricSparkSmall.fx

		Lifespan	120
		LifeSpanJitter	80
		
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
		Bhvr	behaviors/GenericParticleFade.bhvr
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
		Bhvr	behaviors/GenericParticleFade.bhvr
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
		Bhvr	behaviors/GenericParticleFade.bhvr
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
		Bhvr	behaviors/GenericParticleFade.bhvr
		
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

	