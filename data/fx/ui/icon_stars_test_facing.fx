#############################################################
## Icon_MissionInProgress.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On 	Time
	Time	 0

	Event
		Type	Start
		At	Origin
		ChildFX	:Icon_MissionInProgress.fx
	End

	Event
		Ename	GlowAnchor
		Type	Start
		At	Origin
		ChildFX	:Icon_MissionCompleted.fx
	End

	Event
		Ename	GlowAnchor
		Type	Start
		At	Origin
		ChildFX	:Icon_MissionAvailable.fx
	End

	Event
		Ename	GlowAnchor
		Type	Start
		At	Origin
		ChildFX	:Icon_MissionAvailableSoon_FACING.fx
	End

End

#############################################################

End