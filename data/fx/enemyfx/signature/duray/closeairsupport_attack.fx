#############################################################
## CloseAirSupport_Attack.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Condition
	On 	Cycle
	Time 	4


	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			StartJitter	7.0 0.5 7.0
		End
		ChildFX	:CHILD_CloseAirSupport.fx
		Lifespan	5
	End
End

#Condition
#	On	Time
#	Time	10
#
#	Event
#		EName	RootAnchor
#		At	Root
#		Type	Local
#	End
#
#	Event
#		EName	TargetAnchor
#		At	RootAnchor
#		Type	Local
#		BhvrOverride
#			StartColor	255 255 100
#			PositionOffset	0.0 0.0 60.0
#		End
#		Geom	GEO_FX_TestSphere
#	End
#End
#
#Condition
#	On	Time
#	Time	25
#	Event
#		At	RootAnchor
#		Type	Local
#		BhvrOverride
#			PositionOffset	0.0 0.0 55.0
#		End
#		Part	:Targeting_Arrows.part
#		Magnet	TargetAnchor
#		LookAt	TargetAnchor
#	#	Lifespan	1
#	End
#End
#
#Condition
#	On	Time
#	Time	25
#	Event
#		EName	Prime
#		At	RootAnchor
#		Type	Start
#		BhvrOverride
#			TrackRate	3.0
#		End
#	#	Geom	GEO_FX_TestSphere
#		LookAt	TargetAnchor
#		Magnet	TargetAnchor
#		Lifespan	30
#	End
#End
#
#Condition
#	On 	Cycle
#	Time 	4
#
#	Event
#		Type	Local
#		At	Prime
#		Part	:Targeting_Arrows.part
#		LookAt	TargetAnchor
#		Geom	GEO_FX_TestSphere
#		Lifespan	1
#	End
#
#	Event
#		Type	StartPositOnly
#		At	Prime
#		BhvrOverride
#			StartJitter	5.0 0.0 5.0
#		End
#		ChildFX	:CHILD_CloseAirSupport.fx
#		Lifespan	300
#	End
#End

#############################################################

End