#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	LarmR
#		Geom	GEO_WepR_Rikti_Gun_02
		Sound Riktigun_loop 60 60 .3
	End

	Event
		Ename	Hookup
		Type	local
		At	LarmR
		geom	RiktiNode02		
		bhvroverride
			PositionOffset		-1.5 0 -.5
			Scale 1.7 1.05 1
		end

	End

	Event
		Ename	node1
		Type	local
		At	Hookup
		altpiv	1
		Part1	WeaponFX/GreenLightPulsing2.part
		PMagnet	node4
	End

	Event
		Ename	node2
		Type	local
		At	Hookup
		altpiv	2
		Part1	WeaponFX/GreenLightPulsing2.part
		PMagnet	node3
	End

	Event
		Ename	node3
		Type	local
		At	Hookup
		altpiv	3
		Part1	WeaponFX/GreenLightEnds2.part
		PMagnet	BlastNode
	End

	Event
		Ename	node4
		Type	local
		At	Hookup
		altpiv	4
		Part1	WeaponFX/GreenLightEnds2.part
		PMagnet	BlastNode
	End


	Event
		Ename	BlastNode
		Type	local
		At	Hookup
		altpiv	5
		Part1	WeaponFX/GreenLightEnds25.part
		
	End

End

End		