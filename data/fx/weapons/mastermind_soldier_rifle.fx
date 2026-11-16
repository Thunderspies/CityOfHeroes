#############################################################
## Mastermind Robotics (Pulse Rifle) - Activation FX
#############################################################

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
		Geom	GEO_WepR_Mastermind_Robot_Rifle
		Sound axeout 40 40 .6
	End
End

#############################################################

End