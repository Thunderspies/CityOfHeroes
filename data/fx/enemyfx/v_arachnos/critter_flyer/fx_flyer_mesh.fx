#############################################################
## Header
#############################################################

FxInfo

Flags  InheritAlpha   InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ename	off
		Type	Local
		At	emblem
		bhvroverride
			scale .1 .1 .1
		End
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	spin
		type	local
		at	off
		bhvroverride
			PositionOffset		0.0 0 200
			spin			0 0 5
			scale			.1 .1 .1
		End
	End

	Event
		EName	trackA
		type	local
		at	spin
		bhvroverride
			PositionOffset		150 0.0 0.0
		End
	End

	Event
		EName	trackB
		type	local
		at	spin
		bhvroverride
			PositionOffset		-150 0.0 0.0
		End
	End

	Event
		EName	posA
		type	start
		at	spin
		bhvroverride
			trackrate	10
		End
		magnet	trackA
	End

	Event
		EName	posB
		type	start
		at	spin
		bhvroverride
			trackrate	10
			stretch 1
		End
		magnet	trackB
		lookat	posA
	End

	Event
		EName	targ
		type	local
		at	posB
		bhvroverride
			PositionOffset		0.0 0.0 .5
			scale			2 2 2
		End
	End

	Event
		EName	viso
		type	positonly
		at	emblem
		lookat targ
	End

	Event
		type	local
		at	emblem #viso
		Anim	V_Arachnos_flyer_Critter_Mesh
	End
End

#############################################################

End