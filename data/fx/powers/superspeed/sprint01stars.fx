#############################################################
## Sprint01Stars.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		part1	:ArmMotionGlowsx.part
	End

	Event
		Type	Local
		At	Hips
		PMagnet	UlegL
	End

	Event
		Type	Local
		At	Hips
		part1	:ArmMotionGlows2x.part
		PMagnet	UlegR
	End

	Event
		Type	Local
		At	LlegL
		part1	:ArmMotionGlowsx.part
		part1	:ArmMotionGlows2x.part
		PMagnet	ULEGL
	End

	Event
		Type	Local
		At	LlegR
		part1	:ArmMotionGlowsx.part
		part1	:ArmMotionGlows2x.part
		PMagnet	ULEGR
	End

	Event
		Type	Local
		At	LLEGL
		part1	:ArmMotionGlowsx.part
		PMagnet	FootL
	End

	Event
		Type	Local
		At	LLEGR
		part1	:ArmMotionGlowsx.part
		PMagnet	FootR
	End

	Event
		Type	Local
		At	FootR
		part1	:ArmMotionGlowsx.part
		part1	:ArmMotionGlowsx.part
	End

	Event
		Type	Local
		At	FootL
		part1	:ArmMotionGlowsx.part
		part1	:ArmMotionGlowsx.part
	End

	Event
		Type	Local
		At	FootR
		part1	:ArmMotionGlows2.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	ULEGL
		part1	:ArmMotionGlows2.part
		part2	:ArmMotionGlows2x.part
		PMagnet LlegL
	End

	Event
		Type	Local
		At	LLEGL
		part1	:ArmMotionGlows2.part
		PMagnet UlegL
	End

	Event
		Type	Local
		At	FootL
		part1	:ArmMotionGlows2.part
		PMagnet LlegL
	End

	Event
		Type	Local
		At	ULEGR
		part1	:ArmMotionGlows2.part
		part2	:ArmMotionGlows2x.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	LLEGR
		part1	:ArmMotionGlows2.part
		PMagnet UlegL
	End

	Event
		Type	Local
		At	Head
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	UArmL
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LArmL
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	UArmR
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LArmR
		part1	:ArmMotionGlows2.part
	End

	Event
		Type	Local
		At	LlegL
		part3	:SprintStarsLegs.part
	End

	Event
		Type	Local
		At	chest
		part3	:SprintStarsLegs.part
	End

	Event
		Type	Local
		At	LlegR
		part3	:SprintStarsLegs.part
	End
End

#############################################################
## Ground & Physics FX
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	fx\POWERS\SuperSpeed\Sprint.fx
	End
End

#############################################################

End