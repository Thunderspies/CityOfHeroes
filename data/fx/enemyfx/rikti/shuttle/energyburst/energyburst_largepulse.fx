#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	TargetNode
		Type	local
		At	T_Chest
	End

	Event
		Ename	Prime
		Type	Start
		At	Origin
		bhvr	:LargeBeam_Pulse.bhvr
		Part	:Energy_Glow1.part
#		Part	:EnergyStreak.part
#		Part	:Energy_Plasma1.part
#		Part	:Energy_Plasma2.part
		Magnet	TargetNode
		LookAt	TargetNode
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	ALL
		Type	Destroy
	End
End

#############################################################

End