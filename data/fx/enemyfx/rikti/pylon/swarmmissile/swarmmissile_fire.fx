#########################################################
## Rikti Pylon - Swarm Missiles
#########################################################

FxInfo

LifeSpan  200

#########################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	origin
		Sound rocketlaunch 200 200 .69
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	Local
		At	origin
		Sound rocketlaunch2 200 200 .66
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	origin
		Sound rocketlaunch3 200 200 .55
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	Local
		At	origin
		Sound rocketlaunch2 200 200 .69
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	origin
		Sound rocketlaunch 200 200 .69
	End
End

#########################################################

Condition
	On 	Time
	Time 	20
	
	Event
		EName	prime
		Type	startpositonly
		At 	origin
		bhvroverride
			trackrate 2.7
		End
		magnet	target
	End

	Event
		EName	Launcher
		Type	local
		At 	Chest
		bhvroverride
			positionoffset	0.0 8.0 0.0
			pyrrotate	0.0 0.0 0.0
		End
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	SwarmMissile01
		Type	local
		At 	Launcher
		bhvroverride
			pyrrotate 0 0 -90
		End
		childfx	:SwarmMissile_single.fx
	End

	Event
		EName	SwarmMissile01
		Type	local
		At 	Launcher
		bhvroverride
			pyrrotate 0 0 -80
		End
		childfx	:SwarmMissile_single.fx
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName	SwarmMissile02
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -60
		End
	End

	Event
		EName	SwarmMissile02
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -50
		End
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	SwarmMissile03
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -30
		End
	End

	Event
		EName	SwarmMissile03
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -20
		End
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName	SwarmMissile04
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -75
		End
	End

	Event
		EName	SwarmMissile04
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -65
		End
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	SwarmMissile05
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -45
		End
		
	End

	Event
		EName	SwarmMissile05
		Type	local
		At 	Launcher
		childfx	:SwarmMissile_single.fx
		bhvroverride
			pyrrotate 0 0 -35
		End
		
	End
End

#########################################################

End