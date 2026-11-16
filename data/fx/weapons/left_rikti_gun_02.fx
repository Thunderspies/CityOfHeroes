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
		At	WepL
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Rikti_Gun_02
		Sound Riktigun_loop 60 60 .3
	End

	Event
		Ename	Hookup
		Type	local
		At	WepL
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		geom	RiktiNode02

	End

	Event
		Ename	node1
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightPulsing2.part
		PMagnet	node4
	End

	Event
		Ename	node2
		Type	local
		At	Hookup
		altpiv	2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightPulsing2.part
		PMagnet	node3
	End

	Event
		Ename	node3
		Type	local
		At	Hookup
		altpiv	3
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightEnds2.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		Type	local
		At	Hookup
		altpiv	4
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightEnds2.part
		PMagnet	BlastNode
	End


	Event
		Ename	BlastNode
		Type	local
		At	Hookup
		altpiv	5
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightEnds25.part

	End

End

End