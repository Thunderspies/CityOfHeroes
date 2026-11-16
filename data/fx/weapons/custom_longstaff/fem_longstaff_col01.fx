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
		EName 	Prime
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-0.1 0 0.8
			Scale 		1.2 1.2 1.2
			TintGeom	1
		End
		Flags	Oneatatime
                While   WEAPON
		Geom	GEO_WepR_CoL_Staff_3
//		Bhvr	Behaviors/StaffFade.bhvr
		Sound Crystal2_loop 40 40 .21
	End


	Event
		EName 	KeetsiePwns3
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	-0.1 0 0.8
			Scale 		1.2 1.2 1.2
			TintGeom	1
		End
		Geom	GEO_WepR_CoL_Staff_Crystal_1
//		Bhvr	Behaviors/StaffFade.bhvr
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	KeetsiePwns1
		Type	Local
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Geom	CoL_Staff_01
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	KeetsiePwns2
		Type	Local
		At	KeetsiePwns1
		BhvrOverride
			PositionOffset	-0.1 0 1.0
		End
		altpiv	1
		Part1	Weapons\CrystalStaffGlow.part
		Part1	Weapons\CrystalStaffGlow2.part
		Part2	Weapons\CrystalStaffGlowFlash.part
		Part3	Weapons\CrystalStaffSparklies.part
		Flags	Oneatatime
                While   WEAPON
	End
End

#############################################################

End