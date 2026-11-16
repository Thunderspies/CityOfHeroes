#############################################################
## DimensionShift_Emote.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Condition
	On 	Time
	Time	0

	Event
		EName	MotionBlur_Male
		Type	Local
		At	Root
		ChildFX	:DimensionShift_Dummies.fx
	End
End

#############################################################

End