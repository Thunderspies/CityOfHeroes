#############################################################
## VolcanicGasses.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Root
		Part	Powers\EarthControl\VolcanicGasses.part
		Sound rumble2_loop 80 80 0.5
	End

	Event
		Type	Start
		At	Mystic
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		25
			PhysForcePowerJitter	20
		End
	End
End

#############################################################

End