#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

#lifespan	60
########################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Head
		
		#Part1	:RangePower.part
		#Part2	:RangePowerSmall.part
		Part3	:AbilityDotsA.part
		Part4	:AbilitySparklingDotsA.part
		Sound phase_loop 50 50 .3
	End

	Event
		EName	AuraRings
		Type	Local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	Powers/Empathy/BoostRangeRing.part
		part	Powers/Empathy/BoostRangeRing2.part
		part	Powers/Empathy/BoostRangeRingUp.part
		part	Powers/Empathy/BoostRangeRaysThin.part
		
	End
End

End
##################################

