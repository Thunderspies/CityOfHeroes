#########################################################
## FX Geo - Right Pistol - 03
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	DUALHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	DUALHAND
		Flags	OneAtATime
		Bhvr	Behaviors\InheritGroupTint.bhvr
		Type	Local
		At	Origin
		Geom	GEO_WepR_Pistol_2
	End

	Event
		EName	LaserSightStart
		While	DUALHAND
		Flags	OneAtATime
		Bhvr	Behaviors\InheritGroupTint.bhvr
		Type	Local
		At	WeaponDrawn
		AltPiv	1
		Part1	WEAPONS\LaserSight_FlareCore01.part
		Part1	WEAPONS\LaserSight_FlareGlow01.part
	End

	Event
		EName	LaserSightEnd
		While	DUALHAND
		Flags	OneAtATime
		Bhvr	Behaviors\InheritGroupTint.bhvr
		Type	Local
		At	WeaponDrawn
		AltPiv	2
		pOther	LaserSightStart
		Part1	WEAPONS\LaserSight_BeamGlow01.part
	End
End

#########################################################

End