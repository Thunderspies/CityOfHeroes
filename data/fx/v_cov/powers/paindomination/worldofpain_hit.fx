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
		Sound NW_Mind_Link_01 70 70 .4
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
		part1	POWERS\Psionics\PsionicRingsSmall.part
		part2	POWERS\Psionics\PsionicRingzSmall.part
		Sound mental11 80 80 .4
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
		#part1	POWERS\Psionics\CryRays.part
		part2	POWERS\Psionics\PsionicGlow2.part
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
		Part1	POWERS\Psionics\PitBust1.part
		Part2	POWERS\Psionics\PitBust2.part
		Part3	POWERS\Psionics\PitBustRings.part
		Part4	POWERS\Psionics\PitBustBits.part
		Part5	POWERS\Psionics\flash.part
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