#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	MissileOffset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
			StartJitter	0.0 0.0 0.0
		End
		ChildFX	:Child_OrbitalLance_Attack.fx
		Lifespan 60
	End



End


Condition
	On	Time
	Time	5

	Event
		EName 	MissileOffset2
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
			StartJitter	25.0 0.0 25.0
		End
		ChildFX	:Child_OrbitalLance_Attack.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	18

	Event
		EName 	MissileOffset2
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
			StartJitter	25.0 0.0 25.0
		End
		ChildFX	:Child_OrbitalLance_Attack.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	21

	Event
		EName 	MissileOffset2
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
			StartJitter	25.0 0.0 25.0
		End
		ChildFX	:Child_OrbitalLance_Attack.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	25

	Event
		EName 	MissileOffset2
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
			StartJitter	25.0 0.0 25.0
		End
		ChildFX	:Child_OrbitalLance_Attack.fx
		Lifespan 60
	End
End

Condition
	On	Time
	Time	35

	Event
		EName 	MissileOffset2
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0.0 -3.0 0.0
			StartJitter	25.0 0.0 25.0
		End
		ChildFX	:Child_OrbitalLance_Attack.fx
		Lifespan 60
	End
End
#############################################################

End