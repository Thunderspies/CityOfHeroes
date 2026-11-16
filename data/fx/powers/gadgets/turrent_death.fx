#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	mineatt
		Type	start
		At	root

		Geom	roottokneesadjustment
	End

	Event
		EName 	Prime
		Type	Local
		At	mineatt
		Altpiv	1
		Bhvr	behaviors\vanish.bhvr
		Flags	AdoptParentEntity
		lifespan	90
	End

	Event

		Type	Local
		At	mineatt
		Altpiv	1

		Part2	powers\fireControl\Inferno2.part
		Sound inferno4 80 80 .9
		lifespan	90
	End

	Event
		EName 	Primex
		Type	Local
		At	mineatt
		Altpiv	1
		Part1	Powers\FireControl\TurrentBits.part
		lifespan	90
	End

	Event
		EName 	Primex
		Type	Local
		At	mineatt
		Altpiv	1
		Bhvr	Behaviors\CameraShake03.bhvr
		Part	WeaponFx\SmokeExplosion.part
		Part	WeaponFx\fireExplosion2.part
		lifespan 90
	End
End

#############################################################

End