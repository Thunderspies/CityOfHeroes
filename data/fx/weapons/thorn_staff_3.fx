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
		EName 	node
		Type	Local
		At	WepR
		Geom	Staff_02
	End

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Geom	GEO_WepR_Staff_3
		Bhvr	Behaviors/StaffFade.bhvr
		Sound Electhornstaff_loop 40 40 .25
	End

	Event
		Type	Local
		At	node
		altpiv	1
		Part1	:ElectricityStaffGlow.part
		Part2	:CrystalStaffGlowFlash.part
		Part3	:MaceElectricity.part
		Part4	:MaceElectricityInvert.part
	End

End

End