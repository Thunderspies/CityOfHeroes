#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Staff_5
		Bhvr	Behaviors/StaffFade.bhvr
	End

	Event
		Type	Local
		At	Prime
		altpiv	1
		Part1	:staffPupil.part
	End

	Event
		Type	Local
		At	Prime
		altpiv	2
		Part1	:staffPupil2.part
	End

End

End