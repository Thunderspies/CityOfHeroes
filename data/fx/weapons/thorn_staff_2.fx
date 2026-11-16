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
		Geom	GEO_WepR_Staff_2
		Bhvr	Behaviors/StaffFade.bhvr
		Sound axeout 50 50 .3
	End

	Event
		Type	Local
		At	Prime
		altpiv	1
		Part1	:DaemonPupil2.part
	End

	Event
		Type	Local
		At	Prime
		altpiv	2
		Part1	:DaemonPupil.part
	End

End

End