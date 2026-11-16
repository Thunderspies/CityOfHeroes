#############################################################
## Temp_Power_VoidSkiff.fx
#############################################################

FxInfo
	Flags InheritAlpha DontSuppress IsShield

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	VoidSkiff_01_Loop 100 100 .1
	End
	End



Condition
	On TriggerBits
	TriggerBits HOVERBOARD
	DoMany 1

	Event
		At Mystic
		Type Local
		EName ShieldOffset
		Bhvr :Skiff.bhvr
		Flags OneAtATime
		While HOVERBOARD
		BhvrOverride
			PositionOffset 0.1 0 0.3
			PyrRotate 0 180 0
			StartColor 80 70 12
		End
		Geom GEO_VoidSkiff
	End

	Event
		At ShieldOffset
		Type Local
		ChildFx :Temp_Power_VoidSkiff_Child.fx
		EName EnergyShield
		Flags OneAtATime
		While HOVERBOARD
		BhvrOverride
			PositionOffset 0 -0.35 0.3
		End
	End
End

#############################################################
End
