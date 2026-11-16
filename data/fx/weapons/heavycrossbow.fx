#############################################################
## Header
#############################################################

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
		Geom	GEO_WepR_Crossbow_1
	End

	Event
		EName 	Point1
		Type	Local
		At	Prime
		altpiv	1
		Part1	:CrossbowGlow.part
		Magnet	Point2
		lookat	Point2
	End

	Event
		EName 	Point2
		Type	Local
		At	Prime
		altpiv	2
	End

	Event
		EName 	Point2
		Type	Local
		At	Prime
		altpiv	3
		Part1	:CrossbowPulse.part
	End

	Event
		EName 	Point2
		Type	Local
		At	Prime
		altpiv	4
		Part1	:CrossbowPulse.part

	End
End

#############################################################

End