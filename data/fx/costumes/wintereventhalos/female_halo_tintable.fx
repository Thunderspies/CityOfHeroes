#############################################################
## Male_Halo.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	HaloOffset
		Type	Create
		At	Hair
		Inherit	Position Scale
		Update	Position
	End

	Event
		Type	Local
		At	HaloOffset
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
			Scale		0.75 0.75 0.75
			TintGeom	1

		End
		Geom	Tintable_GEO_FX_Halo
		Part	:Halo_Glow1_Tintable.part
		Part	:Halo_GlowRing1_Tintable.part
		Part	:Halo_GlowRing2_Tintable.part
	End
End

#############################################################

End