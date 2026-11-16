#############################################################
## Inferno.fx
#############################################################

FxInfo
LIfespan 250

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	RockAnchor
		Type	StartPositOnly
		At	Origin
		Bhvr	behaviors/CameraShakeSoft.bhvr
		BhvrOverride
			PositionOffset	0 -2.5 0
		End
		Part	:Dust_Pillar_Large.part
		POther	DustAnchor
		Lifespan	5
	End
End




Condition
	On	Time
	Time	1
	Random 	1

	Event
		EName	RootAnchor
		Type	Local
		At	Origin
		BhvrOverride
			PYRRotateJitter		0 180 0
			PositionOffset		0.0 -37.0 0.0
			InitialVelocity		0.0 6.5 0.0
			Drag			0.15
			Scale			3.0 3.0 3.0
			TintGeom		1
			Gravity			0.04
		End
		Anim	FX_CreeperVine
	End

	Event
		EName	RootAnchor
		Type	Local
		At	Origin
		BhvrOverride
			PYRRotateJitter		0 180 0
			PositionOffset		0.0 -37.0 0.0
			InitialVelocity		0.0 6.5 0.0
			Drag			0.15
			Scale			2.0 2.0 2.0
			TintGeom		1
			Gravity			0.04
		End
		Anim	FX_CreeperVine
	End

	Event

		EName	RootAnchor2
		Type	Local
		At	Origin
		BhvrOverride
			PYRRotateJitter		0 180 0
			PositionOffset		0.0 -20.0 0.0
			InitialVelocity		0.0 6.5 0.0
			Drag			0.25
			Scale			1.25 1.25 1.25
			TintGeom		1
			Gravity			0.04
		End
		Anim	FX_CreeperVine
	End
End




#############################################################

End