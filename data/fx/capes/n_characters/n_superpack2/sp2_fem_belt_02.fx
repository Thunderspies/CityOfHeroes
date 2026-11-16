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
			PositionOffset	-0.02 0 -0.07
			TintGeom	1
		End
		GEOM	FX_Hips_SuperPack2_01_fem
	End

End

Condition

	Event
		EName	CapeTest
		Type	Local
		Cape	:SP2_Fem_Belt_02.cape
	End

End

End
