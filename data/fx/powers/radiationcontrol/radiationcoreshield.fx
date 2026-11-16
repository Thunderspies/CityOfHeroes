#########################################################
##	RadiationAura
FxInfo

##################################

Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound particle_loop 80.0 80.0 .66
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 490
	End

End



Condition
	On	Time
	Time	0

	Event
		EName 	Prime2
		Type	Posit 
		At	HIPS
		Bhvr 	behaviors/powers/forcefield/RadiationShield.bhvr
		
		Geom	RadiationShield
				
	End

End

End				