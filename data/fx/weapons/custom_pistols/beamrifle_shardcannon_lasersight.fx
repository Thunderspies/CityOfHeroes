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
	Triggerbits	TWOHAND
	DoMany		1

#	TOP Laser
	Event
		EName	LaserSightStart
		While	TWOHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		2.42 0.07 -0.42
		End
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareCore01.part
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareGlow01.part
	End

	Event
		EName	LaserSightEnd
		While	TWOHAND
		Flags	OneAtATime
		Type	Local
		At	LaserSightStart
		BhvrOverride
			PositionOffset		5 0 0
		End
		pOther	LaserSightStart
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_BeamGlow01.part
	End

#	Bottom Right Laser
	Event
		EName	LaserSightStart2
		While	TWOHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		2.42 -0.08 0
		End
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareCore01.part
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareGlow01.part
	End

	Event
		EName	LaserSightEnd2
		While	TWOHAND
		Flags	OneAtATime
		Type	Local
		At	LaserSightStart2
		BhvrOverride
			PositionOffset		5 0 0
		End
		pOther	LaserSightStart2
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_BeamGlow01.part
	End

#	Bottom Left Laser
	Event
		EName	LaserSightStart3
		While	TWOHAND
		Flags	OneAtATime
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		2.42 0.2 0
		End
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareCore01.part
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_FlareGlow01.part
	End

	Event
		EName	LaserSightEnd3
		While	TWOHAND
		Flags	OneAtATime
		Type	Local
		At	LaserSightStart3
		BhvrOverride
			PositionOffset		5 0 0
		End
		pOther	LaserSightStart3
		Part1	WEAPONS\Custom_Pistols\SemiAuto01\LaserSight_BeamGlow01.part
	End
End

#########################################################

End