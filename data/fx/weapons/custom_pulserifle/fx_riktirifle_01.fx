#########################################################
## FX Geo - Pulse Rifle - 02
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################
Condition
	On	Time
	Time	0

	Event
		Ename	Hookup
		Type	Local
		At	Origin
		BhvrOverride
			Scale		0.625 0.625 0.625
		End
		geom	RiktiNode02
	End
End

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
		BhvrOverride
			Scale		0.625 0.625 0.625
		End
		Geom	GEO_WepR_Rikti_Gun_02
	End

	Event
		Ename	node1
		While	GUN
		Flags	OneAtATime
		Type	local
		At	Hookup
		altpiv	1
		Part1	WeaponFX/GreenLightPulsing2.part
		PMagnet	node4
	End

	Event
		Ename	node2
		While	GUN
		Flags	OneAtATime
		Type	local
		At	Hookup
		altpiv	2
		Part1	WeaponFX/GreenLightPulsing2.part
		PMagnet	node3
	End

	Event
		Ename	node3
		While	GUN
		Flags	OneAtATime
		Type	local
		At	Hookup
		altpiv	3
		Part1	WeaponFX/GreenLightEnds2.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		While	GUN
		Flags	OneAtATime
		Type	local
		At	Hookup
		altpiv	4
		Part1	WeaponFX/GreenLightEnds2.part
		PMagnet	BlastNode
	End

	Event
		Ename	BlastNode
		While	GUN
		Flags	OneAtATime
		Type	local
		At	Hookup
		altpiv	5
		Part1	WeaponFX/GreenLightEnds25.part
	End
End

#########################################################

End