#########################################################
##	FireRing
##
FxInfo

LifeSpan	200	


Condition
	Event
		EName	looker
		Type	Posit
		At	chest
		LookAt	Target
	End

	Event
		EName	focus
		Type	Local
		At	looker
		Bhvr	POWERS/mentalpowers/HydraMindControl.bhvr
		Sound mental4 80 80 .75
	End
End

######################
##Hand effect
##########################
Condition
	On 	Time
	Time 	23

	Event
		EName	core
		Type	posit
		At	focus
		Geom	PsiTele02
		Sound WillDom2 80 80 .8
		
	End


	Event
		EName	core2
		Type	Local
		At	core
		Bhvr	POWERS/mentalpowers/HydraMindControl4.bhvr
		part1	:PsionicTeleRingsSmall.part
		part2	:PsionicTeleRingzSmall.part
		
		part1	:PsionicTeleRingsSmall2.part
		part2	:PsionicTeleRingzSmall2.part
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	core3
		Type	Local
		At	core
		
		part2	:PsionicGlow1.part
				
	
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
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		Bhvr	POWERS/mentalpowers/HydraMindControl4.bhvr
		Part2	:BlastStreakFront.part
		Part1	:BlastStreakBig.part
	End
	
End

Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End
End

############################################################################################

Condition
	On 	Time
	Time	45
	
	Event
		Ename 	core2
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	63
	
	Event
		Ename 	core3
		Type	Destroy
	End

End


End			