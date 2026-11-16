#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Laser_Assault_Rifle_1
		Sound shotgunout 80 80 .6
	End

	Event
		EName 	Hookup
		Type	Local
		At	WepR
		Geom	EnergyRifle
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound lasergun_loop 80 80 .25
	End

################################################

	Event
		Ename	node1
		Type	local
		At	Hookup
		altpiv	2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightPulsing4.part
		PMagnet	node4
	End

	Event
		Ename	node2
		Type	local
		At	Hookup
		altpiv	3
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightPulsing4.part
		PMagnet	node3
	End

	Event
		Ename	node3
		Type	local
		At	Hookup
		altpiv	5
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds4.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		Type	local
		At	Hookup
		altpiv	4
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds4.part
		PMagnet	BlastNode
	End


	Event
		Ename	BlastNode
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/BlueLightEnds45.part

	End

End


End
