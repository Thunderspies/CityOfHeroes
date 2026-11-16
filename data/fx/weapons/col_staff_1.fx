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
		At	WepR
		BhvrOverride
			TintGeom	1
		End
		Flags	Oneatatime
                While   WEAPON
		Geom	GEO_WepR_CoL_Staff_1
		Bhvr	Behaviors/StaffFade.bhvr
		Sound Crystal2_loop 40 40 .21
	End


	Event
		EName 	KeetsiePwns3
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
		EName 	KeetsiePwns1
		Type	Local
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Geom	CoL_Staff_01
		Flags	Oneatatime
                While   WEAPON
	End

	Event
		EName 	KeetsiePwns2
		Type	Local
		At	KeetsiePwns1
		altpiv	1
		Part1	:CrystalStaffGlow.part
		Part1	:CrystalStaffGlow2.part
		Part2	:CrystalStaffGlowFlash.part
		Part3	:CrystalStaffSparklies.part
		Flags	Oneatatime
                While   WEAPON
	End
End

#############################################################

End