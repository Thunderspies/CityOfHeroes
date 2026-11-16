#########################################################
## Ouroboros Portal
#########################################################

FxInfo

## CREATION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound flamebolt4 50 50 .46
	End

End

Condition
	On	Time
	Time	0

	Event
		EName 	GroundSplat
		Type	Start
		At	Root
		BHVR	:OuroborosCreationSplat.bhvr
		Splat	PlayerShadowCircle.tga # FadeBorder
		Lifespan	30
	End

	Event
		EName 	PortalOffset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0.0 6.5 0.0
		End
	End

	Event
		Type	Start
		At	PortalOffset
		Part	:BigGlow02.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Start
		At	PortalOffset
		Part	:BigGlow01.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	20

	Event

		Type	Local
		At	Chest
		sound	oroborosportal_loop 50 40 .81
		bhvr	behaviors/soundFade1.bhvr
		End
end

## SIGIL #######################################################

Condition
	On	Time
	Time	10

	Event
		EName 	GroundSplat
		Type	Start
		At	Root
		BHVR	:OuroborosSigil.bhvr
		Splat	OuroborosSymbol_Splat
	End

	Event
		EName	OuroborosSigil
		Type	Start
		At	Root
		BHVR	:OuroborosGeoBeams.bhvr
		Geom	OuroborosSigil_Beam
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	SigilPulse
		Type	Start
		At	Root
		part	:SigilPulse01.part
		Lifespan	5
	End
End

Condition
	On	Time
	Time	35

	Event
		EName	Portal
		Type	Start
		At	PortalOffset
		Part	:Door1.part
		Part	:Door2.part
		Part	:Door3.part
	End
End

## SIGIL PULSE #######################################################

Condition
	On	Cycle
	Time	31
	Chance	0.25

	Event
		ENAME	TrailOffsetLeft
		Type	Start
		At	OuroborosSigil
		AltPiv	8
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0.0 0.25 0.0
			InitialVelocity	0.0 0.25 0.0
		End
		Lifespan	30
	End

	Event
		ENAME	TrailsOffset1
		Type	Local
		At	TrailOffsetLeft
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
		End
		part	:Trails01.part
		part	:Trails02.part
		Lifespan	20
		LifespanJitter	10
	End

	Event
		ENAME	TrailsOffset2
		Type	Local
		At	TrailOffsetLeft
		BhvrOverride
			PositionOffset	0.0 0.0 -1.0
		End
		part	:Trails01.part
		part	:Trails02.part
		Lifespan	20
		LifespanJitter	10
	End

	Event
		ENAME	TrailOffsetRight
		Type	Start
		At	OuroborosSigil
		AltPiv	9
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
			Spin		0.0 0.25 0.0
			InitialVelocity	0.0 0.25 0.0
		End
		Lifespan	30
	End

	Event
		ENAME	TrailsOffset1
		Type	Local
		At	TrailOffsetRight
		BhvrOverride
			PositionOffset	0.0 0.0 1.00
		End
		part	:Trails01.part
		part	:Trails02.part
		Lifespan	20
		LifespanJitter	10
	End

	Event
		ENAME	TrailsOffset2
		Type	Local
		At	TrailOffsetRight
		BhvrOverride
			PositionOffset	0.0 0.0 -1.00
		End
		part	:Trails01.part
		part	:Trails02.part
		Lifespan	20
		LifespanJitter	10
	End
End

#########################################################

End