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
		bhvr	behaviors\soundFade3.bhvr
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

		part2	CustomizeablePowers\RadiationBlast\ChockingCloudDotsHitx.part
		Part3	CustomizeablePowers\RadiationBlast\ChockingCloudBubblesHitx.part
		Part4	CustomizeablePowers\RadiationBlast\ChockingCloudBubblesSlow.part
		part5	CustomizeablePowers\RadiationBlast\ChockingCloudSmoke.part
		part1	CustomizeablePowers\RadiationBlast\ChockingCloudMist.part
		#Sound	goo_loop 60 30 .3
		PMagnet	Head

	End

	Event
		ENAME	LocalSystems
		Type	Posit
		At	Head

		part2	CustomizeablePowers\RadiationBlast\ChockingCloudDotsTrail.part
		Part3	CustomizeablePowers\RadiationBlast\ChockingCloudBubblesTrail.part
		Part4	CustomizeablePowers\RadiationBlast\ChockingCloudBubblesSlowTrail.part
		part5	CustomizeablePowers\RadiationBlast\ChockingCloudSmokeTrail.part
		part1	CustomizeablePowers\RadiationBlast\ChockingCloudMistTrail.part
		Sound goohiss 60 60 .22
		PMagnet	Head

	End

End


End