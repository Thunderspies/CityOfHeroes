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
		Geom	GEO_WepR_Laser_Assault_Rifle_1
		Sound shotgunout 80 80 0.6
	End

	Event
		EName 	Hookup
		While	GUN
		Type	Local
		At	Origin
		Flags	OneAtATime
		Geom	EnergyRifle
		Sound lasergun_loop 80 80 0.25
	End

	Event
		Ename	node1
		While	GUN
		Type	local
		At	Hookup
		Flags	OneAtATime
		altpiv	2
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Part1	WeaponFX\BlueLightPulsing4.part
		PMagnet	node4
	End

	Event
		Ename	node2
		While	GUN
		Type	local
		At	Hookup
		Flags	OneAtATime
		altpiv	3
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Part1	WeaponFX\BlueLightPulsing4.part
		PMagnet	node3
	End

	Event
		Ename	node3
		While	GUN
		Type	local
		Flags	OneAtATime
		At	Hookup
		altpiv	5
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Part1	WeaponFX\BlueLightEnds4.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		While	GUN
		Type	local
		Flags	OneAtATime
		At	Hookup
		altpiv	4
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Part1	WeaponFX\BlueLightEnds4.part
		PMagnet	BlastNode
	End

	Event
		Ename	BlastNode
		While	GUN
		Type	local
		At	Hookup
		Flags	OneAtATime
		altpiv	1
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Part1	WeaponFX\BlueLightEnds45.part
	End
End

#########################################################

End