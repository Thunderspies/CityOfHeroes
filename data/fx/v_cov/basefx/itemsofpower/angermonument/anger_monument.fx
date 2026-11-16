#############################################################
## Anger_Monument.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 5.75 0
		End
		Part	:Penumbra.part
		Part	:PenumbraDark.part
		Part	:MagicSparkles.part
		Part	:LightRays.part
		Part	:RadiantPower.part
		Part	:GlowWisps.part
		Part	:ShadowWisps.part
		Pmagnet	Center
	End

	Event
		EName	Base
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0
		End
		Part	:LightRays.part
		Pmagnet	Center
	End

	Event
		EName	Theomeros01
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 0
			Scale		1.8 1.75 1.8
			SpinJitter	0 .001 0
			Alpha		105
			StartColor	255 0 100
		End
		Geom	EarthGraspHit
	End

	Event
		EName	Theomeros02
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 0
			Scale		1.8 1.75 1.8
			SpinJitter	0 .001 0
			Alpha		155
			StartColor	200 20 0
		End
		Geom	CosmicPeaceBringerShield
	End

	Event
		EName	Theomeros03
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 0
			Scale		1.8 1.75 1.8
			SpinJitter	0 .001 0
			Alpha		255
			StartColor	250 50 150
		End
		Geom	FX_ForceBubble
	End
End

###############################################################

End