#########################################################
##	
##

FxInfo
LifeSpan  200

##################################

Condition
	
	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	:PurpleFireExplosion.part
		
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	hips
		Part1	:PurpleGernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound PurpleGrExplo 100.0 100.0 1.0
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