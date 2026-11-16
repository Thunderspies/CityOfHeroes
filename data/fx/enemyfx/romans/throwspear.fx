#############################################################
## Costume Anim Entity - Pilum 01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	Mystic
		bhvr	:SpearFadeIn.bhvr
		Geom	GEO_WepR_RomanPilum_01
		
		Lifespan 24
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	Prime
		Type	Start
		At	Mystic
		bhvr	:SpearProjectile.bhvr
		Geom	GEO_RomanPilum_Projectile
		Magnet	Target
		LookAt	Target
	End
End
############################################################
Condition
	On	Time
	Time	9
	Random	1

	Event
		EName	Sound1
		Type	Start
		At	Chest
		Sound Roman_soldier_Pilum_throw 80 80 .65
	End

	Event
		EName	Sound3
		Type	Start
		At	Chest
		Sound Roman_soldier_Pilum_throw_2 80 80 .65
	End

End
#############################################################
Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

#############################################################

End