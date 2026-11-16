#############################################################
## EnvFX_WaterTrickle_Long.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Cycle
	Time	18
	Chance 	.1

	Event
		EName	DropletAnchor2
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	1.0 0.0 1.0
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor2
		AltPiv	1
		ChildFX	:ChildFX_Droplet_Group_Falling.fx
		Lifespan 60

	End
End

Condition
	On	Cycle
	Time	16
	Chance 	.1

	Event
		EName	DropletAnchor2
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	1.0 0.0 1.0
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End


	Event
		Type	Start
		At	DropletAnchor2
		AltPiv	2
		ChildFX	:ChildFX_Droplet_Falling.fx
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
			StartJitter	1.0 0.0 1.0
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
			StartJitter	1.0 0.0 1.0
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor4
		AltPiv	4
		ChildFX	:ChildFX_Droplet_Group_Falling.fx
		Lifespan 60

	End

End

Condition
	On	Cycle
	Time	12
	Chance 	.05

	Event
		EName	DropletAnchor5
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	1.0 0.0 1.0
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor5
		AltPiv	5
		ChildFX	:ChildFX_Droplet_Group_Falling.fx
		Lifespan 60

	End


End

Condition
	On	Cycle
	Time	10
	Chance 	.05

	Event
		EName	DropletAnchor6
		Type	Start
		At	Root
		BhvrOverride
			StartJitter	1.0 0.0 1.0
		End
		Geom	FX_WaterDroplet_Short
		Lifespan 60
	End

	Event
		Type	Start
		At	DropletAnchor6
		AltPiv	6
		ChildFX	:ChildFX_Droplet_Group_Falling.fx
		Lifespan 60

	End

End

#########################################################

End