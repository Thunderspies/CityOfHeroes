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
		At	Origin
		Bhvr	Behaviors\Weapon_Fade_UP.bhvr
		Anim	FX_gearstaff
	End

	Event
		EName 	hook
		Type	Local
		At	Origin
		Geom	FX_NemesisStaff
	End

	Event
		EName 	sparks
		Type	Local
		At	hook
		altpiv	1
		Part1	:ElectricityStaffGlow2.part
		Part2	:CrystalStaffGlowFlash2.part
		Part3	:MaceElectricity2.part
		Part4	:MaceElectricityInvert2.part
		Part	WeaponFX\NemesisSpark.part
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Origin
		Sound NemesisStaff_loop 50 50 0.22
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Origin
		Sound WoodenGears2_loop 50 50 0.3
	End
End

#############################################################

End