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
		At	WepL
		BhvrOverride
			PositionOffset	-0.14 -0.05 1.3
			Scale 		1.8 1.8 1.8
			TintGeom	1
		End
		Geom	GEO_WepR_CoL_Staff_3
//		Bhvr	Behaviors/StaffFade.bhvr
		Sound Crystal2_loop 40 40 .21
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	Prime5
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-0.14 -0.05 1.3
			Scale 		1.8 1.8 1.8
			TintGeom	1
		End
		Geom	GEO_WepR_CoL_Staff_Crystal_1
//		Bhvr	Behaviors/StaffFade.bhvr
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	Prime2
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-0.14 -0.05 1.3
			Scale 		1.8 1.8 1.8
			PyrRotate 	0 0 90
			TintGeom	1
		End
		Geom	GEO_WepR_CoV_Staff_Addon_1
//		Bhvr	Behaviors/StaffFade.bhvr
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	Prime3
		Type	Local
		At	WepL
		Geom	CoL_Staff_01
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName	Prime4
		Type	Local
		At	Prime3
		BhvrOverride
			PositionOffset	-0.14 -0.05 2.2
		End
		altpiv	1
		Part1	Weapons\CrystalStaffGlow.part
		Part2	Weapons\CrystalStaffGlowFlash.part
		Part3	Weapons\CrystalStaffSparklies.part
		Flags	Oneatatime
                While   WEAPON
	End
End

End