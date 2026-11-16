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
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound domination1_loop 40.0 40.0 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 280
	End
End




Condition
	On 	Time
	Time 	0

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End
		
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End

	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End


	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	LarmR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	LarmL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End
	
	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	UarmR
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		Part1	:DominationBoostPower.part
		Part2	:DominationBoostPowerSmall.part
		Part3	:DominationDots1.part
		Part4	:DominationSparklingDots.part
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		
	End

End


Condition
	On 	Death


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	hips
		
		Childfx	POWERS\Ability\DominationDeActivation.fx
		LifeSpan	100
	End

End

End
##################################

