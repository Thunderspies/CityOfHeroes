#########################################################
##	RadiationAura
FxInfo

#LifeSpan	40

Input  
	InpName	Head
End

########################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	head
		Sound goo_loop 60 60 .27
		bhvr	CustomizeablePowers\RadiationEmission\soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 340
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Head
		
		part2	CustomizeablePowers\RadiationEmission\ChockingCloudDotsHitx.part
		Part3	CustomizeablePowers\RadiationEmission\ChockingCloudBubblesHitx.part
		Part4	CustomizeablePowers\RadiationEmission\ChockingCloudBubblesSlow.part
		part5	CustomizeablePowers\RadiationEmission\ChockingCloudSmoke.part
		part1	CustomizeablePowers\RadiationEmission\ChockingCloudMist.part
		#Sound	goo_loop 60 30 .3
		PMagnet	Head
		
	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Head
		
		part2	CustomizeablePowers\RadiationEmission\ChockingCloudDotsTrail.part
		Part3	CustomizeablePowers\RadiationEmission\ChockingCloudBubblesTrail.part
		Part4	CustomizeablePowers\RadiationEmission\ChockingCloudBubblesSlowTrail.part
		part5	CustomizeablePowers\RadiationEmission\ChockingCloudSmokeTrail.part
		part1	CustomizeablePowers\RadiationEmission\ChockingCloudMistTrail.part
		Sound goohiss 60 60 .22
		PMagnet	Head
		
	End

End


End				