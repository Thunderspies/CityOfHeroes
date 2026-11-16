#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	GlowAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-0.6 0.0 0.75
		End
		GEOM	FX_SonicPylon_Glows
	End

	Event
		Ename	BeamAnchor
		Type	Local
		At	GlowAnchor
		AltPiv	3
		BhvrOverride
			PositionOffset	0.1 0.0 0.0
		End
		Part	:BeamGlow01_Small.part
		Part	:BeamGlow02_Small.part
		POther	BeamAnchorEnd
		Lifespan 1
	End

	Event
		Ename	BeamAnchorEnd
		Type	Local
		BhvrOverride
			PositionOffset	0.1 0.0 0.0
		End
		At	GlowAnchor
		AltPiv	4
		Lifespan 1
	End

	Event
		Type	Local
		At	GlowAnchor
		AltPiv	5
		BhvrOverride
			PositionOffset	0.0 0.0 -0.6
		End
		Part	:BeamGlow01_Small.part
		Part	:BeamGlow02_Small.part
		POther	BeamAnchorEnd2
		Lifespan 1
	End

	Event
		EName	BeamAnchorEnd2
		Type	Local
		At	GlowAnchor
		BhvrOverride
			PositionOffset	0.0 0.0 -0.6
		End
		AltPiv	6
		Lifespan 1
	End

	Event
		Ename	BeamAnchor
		Type	Local
		At	GlowAnchor
		AltPiv	7
		BhvrOverride
			PositionOffset	0.0 0.0 -0.6
		End
		Part	:BeamGlow01_Small.part
		Part	:BeamGlow02_Small.part
		POther	BeamAnchorEndL
		Lifespan 1
	End

	Event
		Ename	BeamAnchorEndL
		Type	Local
		At	GlowAnchor
		AltPiv	8
		BhvrOverride
			PositionOffset	0.0 0.0 -0.6
		End
		Lifespan 1
	End

	Event
		Ename	BeamAnchor
		Type	Local
		At	GlowAnchor
		AltPiv	9
		BhvrOverride
			PositionOffset	0.1 0.0 0.0
		End
		Part	:BeamGlow01_Small.part
		Part	:BeamGlow02_Small.part
		POther	BeamAnchorEndL2
		Lifespan 1
	End

	Event
		Ename	BeamAnchorEndL2
		Type	Local
		At	GlowAnchor
		BhvrOverride
			PositionOffset	0.1 0.0 0.0
		End
		AltPiv	10
		Lifespan 1
	End


End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	GlowAnchor
		AltPiv	1
		Part	:SparkGlow01_Small.part
		Part	:SparkGlow02_Small.part
		Lifespan 1
	End

	Event
		Type	Local
		At	GlowAnchor
		AltPiv	2
		Part	:SparkGlow01_Small.part
		Part	:SparkGlow02_Small.part

		Lifespan 1

	End
End

#############################################################

End