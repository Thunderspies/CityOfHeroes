#########################################################
##	jack
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	LarmR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		geom	RiktiNode04
		Sound Riktigun_loop 60 60 .45
	End

	Event
		Ename	node1
		Type	local
		At	Hookup
		altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	:Rikti_GunArm_ScrollingLightPulses01.part
		PMagnet	node4
	End

	Event
		Ename	node2
		Type	local
		At	Hookup
		altpiv	2
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	:Rikti_GunArm_ScrollingLightPulses01.part
		PMagnet	node3
	End

	Event
		Ename	node3
		Type	local
		At	Hookup
		altpiv	3
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	:Rikti_GunArm_ScrollingLightEnds01.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		Type	local
		At	Hookup
		altpiv	4
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	:Rikti_GunArm_ScrollingLightEnds01.part
		PMagnet	BlastNode
	End

	Event
		Ename	BlastNode
		Type	local
		At	Hookup
		altpiv	5
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	:Rikti_GunArm_ScrollingLightEnds02.part

	End
End

#########################################################

End