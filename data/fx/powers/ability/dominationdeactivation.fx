#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Head
End

lifespan	25
########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event	
#		ENAME	HeadGlow
#		Type	Local
#		At	Head
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#	
#
#	Event	
#		ENAME	RightUarmGlow
#		Type	Local
#		At	SpadR
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	SpadL
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#		
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	WepR
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#
#	Event	
#		ENAME	LeftLarmGlow
#		Type	Local
#		At	WepL
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#
#
#	
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	LarmR
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	LarmL
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#	End
#	
#	
#	Event	
#		ENAME	RightLarmGlow
#		Type	Local
#		At	UarmR
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#
#
#	Event	
#		ENAME	LeftUarmGlow
#		Type	Local
#		At	UarmL
#		Part1	:DominationBoostPower.part
#		Part2	:DominationBoostPowerSmall.part
#		Part3	:DominationDots1.part
#		Part4	:DominationSparklingDots.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		
#		lifespan	2
#		
#	End
#
#End


Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound Dominationdeactivate 60.0 60.0 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:DominationRingDown.part

#		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		lifespan	7
	
	End

End

Condition
	On 	Time
	Time 	8

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:DominationRingDown.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part

		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		lifespan	13
	
	End

End

Condition
	On 	Time
	Time 	4

	Event	
		ENAME	ChestGlow
		Type	Local
		At	ULegL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		lifespan	14
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	ULegR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		lifespan	14
	
	End
End

Condition
	On 	Time
	Time 	7

	Event	
		ENAME	ChestGlow
		Type	Local
		At	LLegL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		lifespan	14
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	lLegR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		lifespan	14
	
	End
End

Condition
	On 	Time
	Time 	11

	Event	
		ENAME	ChestGlow
		Type	Local
		At	FootL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	FootR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	
	End

End

End
##################################

