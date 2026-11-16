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
//			Scale		0.7 0.7 0.7
			TintGeom	1
		End
		GEOM	FX_Hips_SuperPack2_01_huge
	End

End

Condition

	Event
		EName	CapeTest
		Type	Local
		Cape	:SP2_huge_Belt_01.cape
	End

End

End
