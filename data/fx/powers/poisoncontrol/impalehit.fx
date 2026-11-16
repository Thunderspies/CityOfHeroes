#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

Lifespan 5400

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	Chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	quill01
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	Chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	quill03
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	Chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	quill07
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	Chest
		Bhvr	Behaviors\QuillProjectile4.bhvr
		Geom	quill06
		Sound poisonhit2 50 50 .7
	End
End

#############################################################

End