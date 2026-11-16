#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

#############################################################

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	hips
		Sound fry_loop 55 55 .4
		bhvr	CustomizeablePowers\RadiationEmission\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 380
	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Posit
		At	Hips
		part2	CustomizeablePowers\RadiationEmission\RadiationBubbleBlastDotsHit.part
		Part3	CustomizeablePowers\RadiationEmission\RadiationBubblesHit.part
		Part4	CustomizeablePowers\RadiationEmission\RadiationBubblesSlow.part
		part5	CustomizeablePowers\RadiationEmission\RadiationAuraYellowHit2.part
		PMagnet	Hips
		#Sound	fry_loop 55 22 .4
	End

	Event
		Type	Posit
		At	Hips
		part4	CustomizeablePowers\RadiationEmission\RadiationRedCloud3.part
		Sound goohiss 60 60 .18
		PMagnet	Hips
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		part2	CustomizeablePowers\RadiationEmission\RadiationBubbleBlastDotsHitx.part
		Part3	CustomizeablePowers\RadiationEmission\RadiationBubblesHitx.part
		Part4	CustomizeablePowers\RadiationEmission\RadiationBubblesSlowx.part
		PMagnet	Hips
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Hips
		part4	CustomizeablePowers\RadiationEmission\RadiationRedCloud.part
		PMagnet	Hips
	End
End

#############################################################

End