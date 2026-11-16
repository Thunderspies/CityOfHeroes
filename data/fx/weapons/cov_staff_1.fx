#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

        On		Triggerbits
        Triggerbits	WEAPON
        DoMany		1

	Event
		EName 	Prime1
		Type	Local
		At	WepR
		BhvrOverride
			TintGeom	1
		End
		Geom	GEO_WepR_CoL_Staff_1
		Bhvr	Behaviors/StaffFade.bhvr
		Sound Crystal2_loop 40 40 .21
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	Prime5
		Type	Local
		At	WepR
		BhvrOverride
			TintGeom	1
		End
		Geom	GEO_WepR_CoL_Staff_Crystal_1
		Bhvr	Behaviors/StaffFade.bhvr
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	Prime2
		Type	Local
		At	WepR
		BhvrOverride
			TintGeom	1
		End
		Geom	GEO_WepR_CoV_Staff_Addon_1
		Bhvr	Behaviors/StaffFade.bhvr
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	Prime3
		Type	Local
		At	WepR
		Geom	CoL_Staff_01
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName	Prime4
		Type	Local
		At	Prime3
		altpiv	1
		Part1	:CrystalStaffGlow.part
		Part2	:CrystalStaffGlowFlash.part
		Part3	:CrystalStaffSparklies.part
		Flags	Oneatatime
                While   WEAPON
	End
End

End