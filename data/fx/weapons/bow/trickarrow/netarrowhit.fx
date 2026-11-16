#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	posit
		At	root
		BhvrOverride
			FadeInLength		30
			FadeOutLength		160
			Scale	1.5 1 1.5
		End
		Geom	NetArrowWeb
		Sound netarrowhit 80 80 .7
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	WebcageHookup1
		Type	posit
		At	root
		BhvrOverride
			Alpha		1
		End
		Geom	NetHookup
	End

	Event
		EName	WebcageHookup2
		Type	posit
		At	root
		BhvrOverride
			Alpha		1
		End
		Geom	NetHookup02
	End

	Event
		EName	WebcageHookup3
		Type	posit
		At	root
		BhvrOverride
			PyrRotate	0 60 0
			Alpha		1
		End
		Geom	NetHookup02
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	1
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	1
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	2
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	3
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	4
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	5
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	6
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	7
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	8
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	9
		Part	:Net.part
		POther	Chest
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	10
		Part	:Net.part
		POther	Chest
	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	1
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	1
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	2
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	3
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	4
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	5
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	6
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	7
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	8
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	9
#		Part	:Net.part
#		POther	Chest
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	10
#		Part	:Net.part
#		POther	Chest
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	1
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	6
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	7
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	8
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	9
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	10
		Part	:Net.part
		POther	LarmR
	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	1
#		Part	:Net.part
#		POther	LarmL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	2
#		Part	:Net.part
#		POther	LarmL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	3
#		Part	:Net.part
#		POther	LarmL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	4
#		Part	:Net.part
#		POther	LarmL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	5
#		Part	:Net.part
#		POther	LarmL
#	End
#End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	1
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	2
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	3
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	4
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	5
		Part	:Net.part
		POther	LarmL
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	1
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	6
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	7
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	8
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	9
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	10
		Part	:Net.part
		POther	LarmR
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	1
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	1
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	2
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	3
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	4
		Part	:Net.part
		POther	LarmL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	5
		Part	:Net.part
		POther	LarmL
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	6
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	7
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	8
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	9
		Part	:Net.part
		POther	LarmR
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	10
		Part	:Net.part
		POther	LarmR
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	1
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	10
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	3
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	4
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	5
#		Part	:Net.part
#		POther	UlegL
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	1
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	2
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	8
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	7
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	6
#		Part	:Net.part
#		POther	UlegR
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	1
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	10
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	3
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	4
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	5
#		Part	:Net.part
#		POther	UlegL
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	1
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	2
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	9
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	8
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	7
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	6
#		Part	:Net.part
#		POther	UlegR
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	1
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	10
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	3
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	4
#		Part	:Net.part
#		POther	UlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	5
#		Part	:Net.part
#		POther	UlegL
#	End
#End
#
#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	1
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	2
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	9
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	8
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	7
#		Part	:Net.part
#		POther	UlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	6
#		Part	:Net.part
#		POther	UlegR
#	End
#End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	1
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	10
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	3
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	4
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup1
		Altpiv	5
		Part	:Net.part
		POther	LlegL
	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	1
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	2
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	9
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	8
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	7
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup1
#		Altpiv	6
#		Part	:Net.part
#		POther	LlegR
#	End
#End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	1
#		Part	:Net.part
#		POther	LlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	10
#		Part	:Net.part
#		POther	LlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	3
#		Part	:Net.part
#		POther	LlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	4
#		Part	:Net.part
#		POther	LlegL
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup2
#		Altpiv	5
#		Part	:Net.part
#		POther	LlegL
#	End
#End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	1
		Part	:Net.part
		POther	LlegR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	2
		Part	:Net.part
		POther	LlegR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	9
		Part	:Net.part
		POther	LlegR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	8
		Part	:Net.part
		POther	LlegR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	7
		Part	:Net.part
		POther	LlegR
	End

	Event
		Type	local
		At	WebcageHookup2
		Altpiv	6
		Part	:Net.part
		POther	LlegR
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	1
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	10
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	3
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	4
		Part	:Net.part
		POther	LlegL
	End

	Event
		Type	local
		At	WebcageHookup3
		Altpiv	5
		Part	:Net.part
		POther	LlegL
	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	1
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	2
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	9
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	8
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	7
#		Part	:Net.part
#		POther	LlegR
#	End
#
#	Event
#		Type	local
#		At	WebcageHookup3
#		Altpiv	6
#		Part	:Net.part
#		POther	LlegR
#	End
#End

#############################################################

End