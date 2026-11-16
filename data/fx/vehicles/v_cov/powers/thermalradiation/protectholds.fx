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
		At	WepR
		Sound Firehand_loop 100.0 100.0 .45
		
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Sound Ignite2 100.0 100.0 .8
		
	
	End

	Event
		Type	Local
		At	root
		geom	HeatWaveSphere2
		bhvr	:ScaleUp.bhvr
		LifeSpan	25
	End

End

##################################

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	root
		geom	HeatWaveSphere
		bhvr	:Scale.bhvr
	End

End


Condition
	On	Time
	Time	10

	Event
		EName	InitialSwordFire
		Type	Local 
		At	Head
		bhvr	:Fadeinandout.bhvr
		Part	:FireHands.part
		Part	:FireHandzSparks.part
	
	End


End


End				