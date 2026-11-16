#############################################################
## Blood Widow "Mind Link" FX
#############################################################

FxInfo

Lifespan 150

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound NW_Mind_Link_01 70 70 .65
	End
End

## HEAD FX ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Emitter
		Type	Local
		At	Head
		BhvrOverride
			PYRRotate		-75 0 0
			PositionOffset		0 0.25 0.5
		End
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	core2
		Type	Local
		At	Emitter
		Bhvr	behaviors\GenericParticleFade.bhvr
		part1	:PsionicRingsSmall.part
		part2	:PsionicRingzSmall.part
		Sound mental11 80 80 .8
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	core3
		Type	Local
		At	Emitter
		Bhvr	behaviors\GenericParticleFade.bhvr
		part2	:PsionicGlow2.part
		Lifespan 45
	End
End

### AOE EFFECT ############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	burst
		Type	PositOnly
		At	Head
		bhvr    V_COV\Powers\BloodWidow\MindLink\MindLink_AOE.bhvr
		Part1	:PitBust1.part
		Part2	:PitBust2.part
		Part3	:PitBustRings.part
		Part4	:PitBustBits.part
		Part5	:flash.part
	End

	Event
		EName 	burst
		Type	PositOnly
		At	Head
		bhvr    V_COV\Powers\BloodWidow\MindLink\MindLink_Sphere.bhvr
		Geom	GEO_WhiteSphere
		Lifespan 1
	End
End

##############################################################

End