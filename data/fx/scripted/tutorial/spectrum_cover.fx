#############################################################
## Spectrum_Cover.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PYRRotate	270 180 0
			Scale		6.0 1.5 6.0
			PositionOffset	0.0 -0.25 0.0
			StartColor	0 50 255
		End
		Part	:Glow_Core.part
		Part	:Glow_Rays.part
		Part	:Glow_Rays_Noise.part

		Part	:Glow_Ring.part
		Part	:Glow_Streaks.part
		Part	:Glow_Streaks_Attached.part
		Geom	Tintable_MegaCore

	End
End

#############################################################

End