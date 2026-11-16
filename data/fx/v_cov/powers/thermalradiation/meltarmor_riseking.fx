#########################################################
##	FireAura
FxInfo

Lifespan 150

#####  ground burst  #############################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Root
		ChildFX		POWERS\FireControl\Zero_FireWeaponCircle2.fx
			
	End
End

#####  ground fire  #############################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Root
		ChildFX		POWERS\FireControl\FireRing.fx
		Lifespan	140
			
	End
End

	
########################################################################
##SOUND
##########################################

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepR
		Sound Melt2_loop 60.0 60.0 .24
		
	
	End
End

Condition
	On 	Time
	Time 	80

	Event
		Type	local
		At	Root
		Sound ScreamPBAOE 60 60 1.0
		
	
	End
End


##### Melt armor ###################


Condition
	On 	Time
	Time 	4

	Event
		Type	local
		At	WepR
		Sound melt3_loop 60.0 60.0 .22
		
	
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Sound firestreaks 70 70 1.0
	
	End
End


###############################################################################
Condition
	On	Time
	Time	0

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

#	Event
#		Type	local
#		At	root
#		geom	HeatWaveSphereRed
#		bhvr	:Scale.bhvr
#	End

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