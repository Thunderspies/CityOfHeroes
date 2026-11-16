#############################################################
## Costume Anim Entity - Shield - Roman 1
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	OriginOffset
		Type	Local
		At	Origin
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part1	:EnergyShield_Spots1.part
		Part2	:EnergyShield_Stars1.part
		Part3   :EnergyShield_Rings.part
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	12
	Chance	0.1

	Event
		Type	Local
		At	OriginOffset
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 -0.275 0
		End
		Part3   :EnergyShield_ExpandingRings1.part
		PMagnet	OriginOffset
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	13
	Chance	0.1

	Event
		Type	Local
		At	OriginOffset
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 -0.275 0
		End
		Part3   :EnergyShield_ExpandingRings2.part
		PMagnet	OriginOffset
		Lifespan 1
	End
End

#############################################################

End