#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0
	

########################################################################
##SOUND
##########################################


	Event
		EName	InitialSwordFire
		Type	start 
		At	WepR
		Sound Firehand_loop 100.0 100.0 .45
		
	
	End

###############################################################################

	Event
		EName	Pivot
		Type	Local
		At	head
		
		part2	:FireCorea.part

	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		
		part3	:FireCoreTrail1a.part
	End

	Event
		EName	Chest
		Type	Local
		At	back
		
		part2	:FireCore4a.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		
		part3	:FireCoreTrail2a.part
		Part4	:FireCoreSparksa.part
	End
	
	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:FireCore1a.part
		part2	:FireCoreTraila.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:FireCore1a.part
		part2	:FireCoreTraila.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	:FireCore1Feeta.part
		part2	:FireCoreTrailFeeta.part
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	:FireCore1Feeta.part
		part2	:FireCoreTrailFeeta.part
	End

###############################################################################

	Event
		Type	local
		At	root
		geom	HeatWaveSphereRed
		bhvr	:Scale.bhvr
	End

	Event
		Type	Local 
		At	Root
		part   :Steam.part
	End
End


Condition
	On	cycle
	Time	10
	Chance	.2

	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	7
	Chance	.2

	Event
		Type	local 
		At	UARML
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20

	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	8
	Chance	.2

	Event
		Type	local 
		At	UARMR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20

	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	10
	Chance	.2

	Event
		Type	local 
		At	WepL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
		
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	9
	Chance	.2

	Event
		Type	local 
		At	WepR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
		
	End

End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	7
	Chance	.2

	Event
		Type	local 
		At	LLEGL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
	
End


Condition
	On	cycle
	Time	8
	Chance	.2
	
	Event
		Type	local 
		At	LLEGR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	9
	Chance	.2

	Event
		Type	local 
		At	ULEGL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
	
End


Condition
	On	cycle
	Time	8
	Chance	.2
	
	Event
		Type	local 
		At	ULEGR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End

Condition
	On	cycle
	Time	10
	Chance	.2
	
	Event
		Type	local 
		At	LArmL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	8
	Chance	.2
	
	Event
		Type	local 
		At	LArmR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End

Condition
	On	cycle
	Time	8
	Chance	.2
	
	Event
		Type	local 
		At	SpadL
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End


Condition
	On	cycle
	Time	7
	Chance	.2
	
	Event
		Type	local 
		At	SpadR
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDripWellupInvert.part
		part   :MetalDripWellup_Invert.part
		part   :MetalDripWellup.part
		LifeSpan	20
	End
End


Condition
	On	cycle
	Time	10
	Chance	.1
	Event
		Type	local 
		At	neck
		BHVR	Behaviors/GenericParticleFade2.bhvr
		part   :MetalDrips.part
		part   :MeltingMetal.part
		LifeSpan	20
				
	End
End

End				