#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

#	Event
#		EName	mine
#		Type	start
#		At	root
#		Geom	TurrentBase
#		Bhvr	Behaviors\TurrentScale.bhvr
#		Sound	turretactivate	60 50 .66
#	End

	Event
		Type	local
		At	Hips
		Bhvr	Behaviors\TurrentScale.bhvr
		Geom	turrent01
		Sound jetpack2_loop 70 70 .3
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Hookup
		Type	local
		At	Hips
		Sound gatgun3 80 80 .8
		Geom	turrentRotate
	End

	Event
		Type	local
		At	Hookup
		Altpiv	3
		BhvrOverride
			Spin			0.0 0.0 0.3
		End
		Geom	leftBarrell
	End

	Event
		Type	local
		At	Hookup
		Altpiv	4
		BhvrOverride
			Spin			0.0 0.0 -0.3
		End
		Geom	rightBarrell
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Hookup
		Altpiv	5
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet01.part
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet02.part
	End

	Event
		Type	local
		At	Hookup
		Altpiv	8
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet01.part
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet02.part
	End

	Event
		Type	local
		At	Hookup
		Altpiv	10
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet01.part
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet02.part
	End
End

Condition
	On 	Time
	Time 	0


	Event
		Type	local
		At	Hookup
		Altpiv	6
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet01.part
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet02.part
	End

	Event
		Type	local
		At	Hookup
		Altpiv	7
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet01.part
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet02.part
	End

	Event
		Type	local
		At	Hookup
		Altpiv	9
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet01.part
		Part2	POWERS\Gadgets\AutoTurret_Thruster_Jet02.part
	End
End

#############################################################

End