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
		Type	Local
		At	WepR
		Sound Riktigun_loop 60 60 .3
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepR_Rikti_Gun_01
	End

	Event
		Ename	Hookup
		Type	local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		geom	RiktiNode03

	End

	Event
		Ename	node1
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightPulsing.part
		Part2	WeaponFX/GreenLightPulsing12.part

		PMagnet	node4
	End

	Event
		Ename	node2
		Type	local
		At	Hookup
		altpiv	2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightPulsing.part
		Part2	WeaponFX/GreenLightPulsing12.part

		PMagnet	node3
	End

	Event
		Ename	node3
		Type	local
		At	Hookup
		altpiv	3
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightEnds.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		Type	local
		At	Hookup
		altpiv	4
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightEnds.part
		PMagnet	BlastNode
	End


	Event
		Ename	BlastNode
		Type	local
		At	Hookup
		altpiv	5
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	WeaponFX/GreenLightPulsing1.part

	End

End

End