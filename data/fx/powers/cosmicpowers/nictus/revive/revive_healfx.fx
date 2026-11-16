#########################################################
## Umbral Aura - Stygian Return (Just the Heal FX)
#########################################################

FxInfo

Lifespan 250	

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

## HEAL FX #######################################################

Condition
	On 	Time
	Time	105 #60 Projectile travel time + 45 FramesBeforeHit

	Event
		ENAME   Tendrills
		Type	Posit 
		At	Chest
		Part	Powers/DarknessControl/ChillOfNightTendril.part
		part	:HealingHitMist.part
		Part	:HealingGlow.part
		LifeSpan	120
	End

	Event
		ENAME   Tendrills
		Type	Posit 
		At	Chest
		Part	:SoulDrainHitTendril2.part
		Part	:SoulDrainHitTendrilBlue.part
		Part	:SoulDrainHitTendrilPurple.part	
		LifeSpan	50
	End
	
	Event
		Type	Posit 
		At	Chest
		Part1	:LightRaysHit.part
		Part1	:LightRays.part
		LifeSpan	30
	End
End

#########################################################

End