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
		At	Hips
		BhvrOverride
			PositionOffset	64 -24 12
		End
		#Geom	GEO_FX_TestSphere
	End

	Event
		Ename	Prime
		Type	Start
		At	Origin
		bhvr	:SmallBeam_Pulse.bhvr
#		BhvrOverride
#			Scale	10 10 10
#		End
#		Geom	GEO_FX_TestSphere
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