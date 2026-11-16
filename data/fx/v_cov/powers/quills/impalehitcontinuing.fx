#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	chest
		Sound Quills6 70 70 .7
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		At	chest
		Bhvr	:QuillHit01.bhvr
		Geom	Spine01
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		At	chest
		Bhvr	:QuillHit01.bhvr
		Geom	Spine03
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		At	chest
		Bhvr	:QuillHit01.bhvr
		Geom	Spine07
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
		At	chest
		At	chest
		Bhvr	:QuillHit01.bhvr
		Geom	Spine06
		Sound poisonhit2 50 50 .7
	End
End


#############################################################

End