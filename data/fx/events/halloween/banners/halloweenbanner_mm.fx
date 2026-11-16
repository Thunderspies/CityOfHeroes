#############################################################
## HalloweenBanner_MM.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Offsets
		Type	Local
		At	Root
		Geom	GEO_HalloweenSigil_Offsets
	End

	Event
		Type	Local
		At	Offsets
		AltPiv	4
		Anim	FX_Banner_Halloween_MM
		Sound Flag6_loop 40.0 40.0 0.33
	End

	Event
		Type	Local
		At	Offsets
		AltPiv	5
		Anim	FX_Banner_Halloween_MM
		Sound Flag6_loop 40.0 40.0 0.33
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		-8.625 5 -3.325
		End
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Base.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Glow.part
		Part3	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Sparks.part
		Sound Torch1_loop 22.0 22.0 1.0
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		-4.0 4.5 7.25
		End
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Base.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Glow.part
		Part3	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Sparks.part
		Sound Torch1_loop 22.0 22.0 1.0
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		6.75 7 2
		End
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Base.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Glow.part
		Part3	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Sparks.part
		Sound Torch1_loop 22.0 22.0 1.0
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		12.5 6.5 -7.5
		End
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Base.part
		Part1	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Glow.part
		Part3	Events\Halloween\GhostlyTorches\Violet_Flame_Ghostly_Sparks.part
		Sound Torch1_loop 22.0 22.0 1.0
	End
End

#############################################################

End