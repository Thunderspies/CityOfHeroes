#########################################################
## Umbral Aura - Stygian Circle
#########################################################

FxInfo

LifeSpan	135

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

######################################################################

Condition
	On	Time
	Time	40
	
	Event
		EName	RINGS
		Type	Local
		At	Waist
		Bhvr	Behaviors/GenericParticleFade.bhvr

		Part	:SoulDrainHitPuddle2.Part
		Part	:SoulDrainHitTendril2.part
		Part	:SoulDrainHitTendrilBlue.part
		Part	:SoulDrainHitTendrilPurple.part	
	End

	Event
		EName	RINGS
		Type	Local
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		part1	:RegenerationRing.part
	End
End    

Condition
	On	Time
	Time	0

	Event
		EName	9a5
		Type	Local
		At	root
		Bhvr	Behaviors/GenericParticleFade.bhvr
			
		Part	:DarkTarRingsIn.Part
		Part	:DarkTarRingsInPURPLE.Part
		Part	:DarkTarRingsInBLUE.Part
		Sound WarShadeRegen 80 80 .8
		LifeSpan	110
	End

End

End	

