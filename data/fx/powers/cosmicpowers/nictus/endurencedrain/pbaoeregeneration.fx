#########################################################
## Umbral Aura - Eclipse
#########################################################

FxInfo

LifeSpan	75

## SOUND FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName	9a5
		Type	Local
		At	root
			
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Sound WarShadeRegen2 80 80 .63
	End
End

## EYE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Eyes
		Type	Local 
		At	Root
		ChildFX POWERS\CosmicPowers\Nictus\ActivationEyes\ActivationEyesContinuingFX.fx
	End
End

## AOE FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName	9a5
		Type	Local
		At	root
			
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:DarkTarRingsInBLK.Part
		Part	:DarkTarRingsIn.Part
		Part	:DarkTarRingsInPURPLE.Part
		LifeSpan	60
	End
End

Condition
	On	Time
	Time	10
	
	Event
		EName	RINGS
		Type	Local
		At	Waist
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:SoulDrainHitPuddle2.Part
		Part	:SoulDrainHitTendril2.part
		Part	:SoulDrainHitTendrilBlue.part
		Part	:SoulDrainHitTendrilPurple.part	
		LifeSpan	60
	End

	Event
		EName	RINGS
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:RegenerationRing.part
		LifeSpan	60
	End
End    

######################################################################

End