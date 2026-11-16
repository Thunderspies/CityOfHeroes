#########################################################
## Rikti Heavy Assault Suit - Fusion Pulse	
#########################################################

FxInfo

Lifespan	24

#########################################################
	
Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	local
		At	chest
		geom	centerdummy
		bhvr	behaviors/spin4.bhvr
		Sound ricktigunhit 90 90 .93
	End
End

Condition
	On	Time
	Time	16

	Event
		EName	HitSound
		Type	local
		At	chest
		Sound ricktigunhit 90 90 .93
	End
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	ENEMYFX\Rikti\RiktiRandomEllipseSmall.part
		Part1	ENEMYFX\Rikti\RiktiRandomEllipseSmall.part
	End
End

Condition
	On	Cycle
	Time	8

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	ENEMYFX\Rikti\RiktiRandomEllipseSmall.part
		Part1	ENEMYFX\Rikti\RiktiRandomEllipseSmall.part
		Lifespan	16
	End
End

Condition
	On	Time
	Time	2

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part2	ENEMYFX\Rikti\RiktiEnergyBubblingSmall.part
		Part3	ENEMYFX\Rikti\RiktiEnergyBubblingSmallPurple.part
		Lifespan	12
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	spit
		Type	start
		At	Ring
		altpiv	1
		
		Part2	ENEMYFX\Rikti\RiktiPortalSpecksSmall2.part
		Lifespan	6
	End
End


#########################################################

Condition
	On	Time
	Time	40

	Event
		EName	all
		Type	destroy
	End
End

#########################################################

End