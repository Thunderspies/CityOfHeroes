#############################################################
## Fault.fx
#############################################################

FxInfo
Lifespan 150

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_HypnoThorn_Hit_01 100 100 .8
	End

	Event
		Type	local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:Hypno_PsyHit_Circles.part
	eND
End

Condition
	On	Cycle
	Time	10

	Event
		Type	Local
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 -0.5 0
		End
		Part	:Toxic_Splatter.part
		part4	:Toxic_BlobHits.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:HeadDizzies.part
		Part2	:HeadDizzyRings.part
		part3	:MindGlowHit1.part
		part3	:MindGlowHit2.part
	End
End

#############################################################

End