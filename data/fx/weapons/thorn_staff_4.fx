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
		Geom	GEO_WepR_Staff_4
		Bhvr	Behaviors/StaffFade.bhvr
		Sound Crystal2_loop 40 40 .21
	End

	Event
		EName 	node
		Type	Local
		At	WepR
		Geom	Staff_04
	End

	Event
		Type	Local
		At	node
		altpiv	1
		Part1	:CrystalStaffGlow.part
		Part2	:CrystalStaffGlowFlash.part
		Part3	:CrystalStaffSparklies.part
	End
End

End