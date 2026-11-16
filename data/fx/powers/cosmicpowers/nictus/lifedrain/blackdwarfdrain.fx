#########################################################
## Umbral Blast - Essence Drain
#########################################################

FxInfo

LifeSpan	60

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime
		Type	Start 
		At	T_chest
		Sound WarShadeLifeDrain 80 80 .85
		
	End
End

## PROJECTILE ######################################################

Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime
		Type	Start 
		At	T_chest
		Bhvr	:MultipleSlowProjectile.bhvr
		Magnet	WepL
		LookAt	WepL
		Part	:BlackEnergyTrailOutline.part
		Part	:Sparks.part
		Part	:BlackEnergyTrailTendril.part
		Part	:PurpleEnergyTrailTendril.part
		Part	:BlueEnergyTrailTendril.part
		Part	:HealingEnergy.part
		Part	:HealingEnergy.part
		Part	:HealingEnergy.part		
	End
End

## ON-HIT FX ######################################################

Condition
	On PrimeHit

	Event
		EName	SwordFlash2
		Type	Local
		At	chest
		Part	:HealGlow.part
		LifeSpan 15
	End

	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	:PeaceHandsFlash.part
		Part	:QuickBlastHandStreaks.part
		Part	:QuickBlastHandGlow.part
		Part	:QuickBlastHandRing.part
		Part	:QuickBlastHandStreaksBlue.part
		LifeSpan 5
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End

########################################################

End