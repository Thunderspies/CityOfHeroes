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
			Scale		0.875 0.875 0.875
		End
		Geom	GEO_FX_FlamingHalo
		Part	:FlamingHalo_GlowRing1.part
		Part	:FlamingHalo_FlameRing1.part
		Part	:FlamingHalo_FlameRing2.part
	End
End

#############################################################

End