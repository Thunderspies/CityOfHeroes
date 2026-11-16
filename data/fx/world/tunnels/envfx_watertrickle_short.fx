#############################################################
## EnvFX_WaterTrickle_Short.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	5
	Chance 	.1

	Event
		EName	DropletAnchor
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	0.5 0.0 0.5
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor
		AltPiv	1
		ChildFX	:ChildFX_Droplet_Falling.fx
		Lifespan 60

	End
End

Condition
	On	Cycle
	Time	6
	Chance 	.1

	Event
		EName	DropletAnchor2
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	0.5 0.0 0.5
		End

		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor2
		AltPiv	2
		ChildFX	:ChildFX_Droplet_Group_Falling.fx
		Lifespan 60

	End
End

Condition
	On	Cycle
	Time	20
	Chance 	.1

	Event
		EName	DropletAnchor3
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	0.5 0.0 0.5
		End

		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor3
		AltPiv	3
		ChildFX	:ChildFX_Droplet_Falling.fx
		Lifespan 60

	End
End

Condition
	On	Cycle
	Time	15
	Chance 	.1

	Event
		EName	DropletAnchor4
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	0.5 0.0 0.5
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor4
		AltPiv	4
		ChildFX	:ChildFX_Droplet_Falling.fx
		Lifespan 60

	End
End

#########################################################

End