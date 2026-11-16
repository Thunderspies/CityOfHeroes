#########################################################
##	FireAura
FxInfo

##################################

Condition
	On	Time
	Time	0
	
########################################################################
##SOUND
##########################################


	Event
		EName	InitialSwordFire
		Type	Local 
		At	T_Root
		Sound Firehand_loop 100.0 100.0 .45
		
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	T_Root
		Sound Ignite2 100.0 100.0 .8
		
	
	End

	Event
		Type	Local
		At	T_Root
		geom	HeatWave
		bhvr	:WaveScale.bhvr
#		LifeSpan	25
	End

End

Condition
	On	Cycle
	Time	20

	Event
		Type	Local
		At	T_Root
		BHVR	:splat.bhvr
		Splat	thinRing
		LifeSpan	10
	End

End

##################################

Condition
	On	Time
	Time	10

	Event
		EName	InitialSwordFire
		Type	Local 
		At	T_Head
		bhvr	:Fadeinandout.bhvr
		Part	:FireHands.part
		Part	:FireHandzSparks.part
	
	End


End


End				