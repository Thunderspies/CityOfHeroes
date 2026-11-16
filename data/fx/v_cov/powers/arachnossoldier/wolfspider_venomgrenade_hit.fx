#############################################################
## Arachnos Soldier - Wolf Spider - Frag Grenade
#############################################################

FxInfo

LifeSpan 200

ClampMinScale 1.0 1.0 1.0
ClampMaxScale 1.0 1.0 1.0

## FIRE FX ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName   RadiusFxScale
		Type	Local
		At	Origin
		Bhvr	powers\firecontrol\Inferno.bhvr
	End

	Event
		Type	start
		At	RadiusFxScale
		bhvr    WEAPONS\Bow\TrickArrow\Infernobubble.bhvr
		Part	WEAPONS\Bow\TrickArrow\PoisonYellow2.part
		Part	WEAPONS\Bow\TrickArrow\PoisonGlows.part
		Sound VenomGrenade 70 70 0.97
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Origin
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	50
		End
		Lifespan 1
	End
End

#############################################################

End