#########################################################
## Umbral Aura - Eclipse (Endurance Drain)
#########################################################

FxInfo

LifeSpan	60

## SOUND FX ######################################################

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	chest
		Bhvr	:MultipleSlowProjectile.bhvr
		Magnet	T_WepR
		LookAt	T_WepR
		Sound WarShadeEnduranceDrain 80 80 .88
	End
End

## PROJECTILES ######################################################

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	Chest
		Bhvr	:MultipleSlowProjectile.bhvr
		Magnet	T_Chest
		LookAt	T_Chest

		Part	:BlackEnergyTrailOutline.part
		Part	:Sparks.part
		Part	:BlackEnergyTrailTendril.part
		Part	:PurpleEnergyTrailTendril.part
		Part	:BlueEnergyTrailTendril.part
		Part	:EndurenceEnergy.part
		Part	:EndurenceEnergy.part
		Part	:EndurenceEnergy.part

		Lifespan	45
	End
End

## CLEAN UP ######################################################

Condition
	On PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

########################################################

End			