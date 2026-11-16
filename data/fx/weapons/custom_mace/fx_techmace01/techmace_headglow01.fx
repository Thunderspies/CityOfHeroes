#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

## OFFSETS #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MaceOffsets #1 = Center of Head, 2-9 = Tines, 10-15 = Laser Offsets
		Type	Local
		At	Origin
		Geom	TechMaceOffsets
	End

	Event
		EName	HeadOffset
		Type	Local
		At	MaceOffsets
		AltPiv	1
	End

	Event
		EName	Tine1Offset
		Type	Local
		At	MaceOffsets
		AltPiv	2
	End

	Event
		EName	Tine2Offset
		Type	Local
		At	MaceOffsets
		AltPiv	3
	End

	Event
		EName	Tine3Offset
		Type	Local
		At	MaceOffsets
		AltPiv	4
	End

	Event
		EName	Tine4Offset
		Type	Local
		At	MaceOffsets
		AltPiv	5
	End

	Event
		EName	Tine5Offset
		Type	Local
		At	MaceOffsets
		AltPiv	6
	End

	Event
		EName	Tine6Offset
		Type	Local
		At	MaceOffsets
		AltPiv	7
	End

	Event
		EName	Tine7Offset
		Type	Local
		At	MaceOffsets
		AltPiv	8
	End

	Event
		EName	Tine8Offset
		Type	Local
		At	MaceOffsets
		AltPiv	9
	End
End

## MACE HEAD GLOW #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MaceHeadGlow
		Type	Local
		At	HeadOffset
		Part	:TechMace_HeadGlow01.part
	End
End

## LIGHTNING - CENTER TO TINES #######################################################

Condition
	On 	Cycle
	Time 	10
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine1Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine2Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine1Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	10.5
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine2Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine3Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine2Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	11
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine3Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine4Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine4Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	11.5
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine4Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine5Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine4Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	12
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine5Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine6Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine5Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	12.5
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine6Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine7Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine6Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	13
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine7Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine8Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine7Offset
		Lifespan 5
	End
End

Condition
	On 	Cycle
	Time 	13.5
	Chance	0.125

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine8Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	HeadOffset
		Lifespan 5
	End

	Event
		EName	MaceHeadGlow
		Type	Local
		At	Tine1Offset
		Bhvr	behaviors\GenericParticleFadeQuick.bhvr
		Part	:TechMace_LightningCore01.part
		Part	:TechMace_LightningGlow01.part
		Part	:TechMace_TineGlow01.part
		pOther	Tine8Offset
		Lifespan 5
	End
End

#########################################################

End