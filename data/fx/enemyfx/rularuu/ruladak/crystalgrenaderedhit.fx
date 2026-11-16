#########################################################
##	
##

FxInfo
LifeSpan  100



##################################

Condition
	
	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:FireExplosion.part
		
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	hips
		Part1	:GernadeLauncherSparks.part
		
		Sound BlackGrExplo 100.0 100.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	hips
		Part	:FireExplosionFlash.part
		
		Lifespan 5
		
	End

	
End


End							