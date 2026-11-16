#########################################################

FxInfo

Flags    InheritAlpha InheritGeoScale
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0 0 -0.06
			TintGeom	1
		End
		GEOM	FX_Hips_SuperPack2_01_huge
	End

End

Condition

	Event
		EName	CapeTest
		Type	Local
		Cape	:SP2_Huge_Belt_02.cape
	End

End

End
