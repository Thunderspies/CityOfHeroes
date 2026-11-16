#############################################################
## Arachnos Soldier - Bane Spider - Web Grenade
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound grtoss1 60 60 .7
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	start
		At	WepL
		geom	grenade
		bhvr	behaviors\MorterLobbprojectile2.bhvr
		Part1	WeaponFx\BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	powers\firecontrol\Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	start
		At	RadiusFxScale
		bhvr    WEAPONs\Bow\TrickArrow\stickyBubble.bhvr
		Part	WEAPONs\Bow\TrickArrow\StickyYellow2.part
		Part	WEAPONs\Bow\TrickArrow\StickyGlows.part
		LifeSpan 30
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root

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