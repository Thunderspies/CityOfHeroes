#############################################################
## NeuronsTower_Portal.fx
#############################################################



FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

#############################################################
###	PORTAL		  ###################################
#############################################################

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Oval.part
		Part	:Oval_Fill_Alpha.part
		ChildFX	:Portal_Child.fx
	End
End

Condition
	On	Cycle
	Time	330

	Event
		EName	Offset
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		ChildFX	:Portal_Child.fx
		Lifespan 330
	End
End
#############################################################

End