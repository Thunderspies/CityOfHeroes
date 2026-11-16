#########################################################
##	Fire_Ball
##
FxInfo

Lifespan 1000

Condition
	Event
		Type	Local
		At		Origin
		Geom	FX_IceWeapon__Ice
		Sound targetdrone_loop 30 30 .3
		bhvr	behaviors/soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End


End	


			