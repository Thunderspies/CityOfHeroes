#########################################################
## FX Geo - Pulse Rifle - 02
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 		Triggerbits
	Triggerbits	GUN
	DoMany		1

	Event
		EName	WeaponDrawn
		While	GUN
		Flags	OneAtATime
		Type	Local
		At	Origin
		Geom	GEO_WepR_Laser_Pistol_03
	End


	Event
		EName 	Hookup
		Type	Local
		While	GUN
		Flags	OneAtATime
		At	WeaponDrawn
		Bhvr	Behaviors/Weapon_Fade.bhvr
		BhvrOverride
		#	StartColor	50 50 50
		#	Alpha		1
			PositionOffset	0.0 0 0.25
		End
		Geom	FX_Pistol_Offsets
	End

################################################

	Event
		Ename	node1
		Type	local
		At	Hookup
		altpiv	1
		While	GUN
		Flags	OneAtATime
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightPulsing1.part
		PMagnet	node3
	End

	Event
		Ename	node2
		Type	local
		At	Hookup
		altpiv	2
		While	GUN
		Flags	OneAtATime
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightPulsing1.part
		PMagnet	node4
	End

	Event
		Ename	node3
		Type	local
		At	Hookup
		altpiv	3
		While	GUN
		Flags	OneAtATime
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds1.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		Type	local
		At	Hookup
		altpiv	4
		While	GUN
		Flags	OneAtATime
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds1_Pist.part
		PMagnet	BlastNode
	End


	Event
		Ename	node5
		Type	local
		At	Hookup
		altpiv	5
		While	GUN
		Flags	OneAtATime
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds1.part
		PMagnet	BlastNode
	End


	Event
		Ename	BlastNode
		Type	local
		At	Hookup
		altpiv	6
		While	GUN
		Flags	OneAtATime
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds15.part

	End

#########################################################

End

#########################################################

End