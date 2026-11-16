#########################################################
## FX Geo - Right Pistol - 03
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition

	On 	Time
	Time 	0
	On 		Triggerbits
	Triggerbits	DUALHAND
	DoMany		1

#	TOP Laser
	Event
		EName	LaserSightStart
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		1.2 0 0.1
		End
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareCore01.part
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareGlow01.part
	End

	Event
		EName	LaserSightEnd
		While	DUALHAND
		Flags	OneAtATime
		Type	Local
		At	LaserSightStart
		BhvrOverride
			PositionOffset		2 0 0
		End
		pOther	LaserSightStart
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_BeamGlow01.part
	End

End

#########################################################

End