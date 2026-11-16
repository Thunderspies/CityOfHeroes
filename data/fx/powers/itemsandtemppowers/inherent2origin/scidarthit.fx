#############################################################
## HEADER
#############################################################

FxInfo

#ClampMinScale 0.75 0.75 0.75
#ClampMaxScale 1.25 1.25 1.25

LifeSpan  90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Neck
		Sound trangdarthit 70 70 .87
	End

	Event
		Type	Create
		Inherit	ALL
		Update	Position Rotation
#		Type	Local
		At	Neck
		Bhvr	:SciDartHit.bhvr
		Part2	:DartHitflashStar.part
		Part1	WEAPONFX\Pistolflash.part
		geom	FX_Science_syringe
	End

	Event
		EName 	Glow
		Type	Local
		At	linger
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 0 -.525
		End
		Part	:SciDartGlow.part
		Part	:SciDartTrail.part
	End
End

#############################################################

End