#############################################################
## WillDomination.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		EName	core
		Type	start
		At	WepL
		Geom	PsiTele02
		Sound WillDom2 80 80 .8
	End

	Event
		EName	core2
		Type	Local
		At	core
		Altpiv	1
		part1	:PsionicTeleRingsSmall.part
		part2	:PsionicTeleRingzSmall.part
		part1	:PsionicTeleRingsSmall2.part
		part2	:PsionicTeleRingzSmall2.part
		Lifespan 22
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName	core3
		Type	Local
		At	core
		Altpiv	1
		part2	:PsionicGlow1.part
		Lifespan 38
	End
End

###########################################################################################3

Condition
	On	Time
	Time	50

	Event
		EName	prime
		Type	Start
		At	Core
		Altpiv	1
		Bhvr	:MedianProjectile3.bhvr
		BhvrOverride
			TrackRate		1.7
		End
		Geom	FrontFacingLocalPivot
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		Part2	:BlastStreakFront.part
		Part1	:BlastStreakBig.part
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

#############################################################

End