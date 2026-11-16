#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On	Time
			
	Event
		Ename	FireBallHit
		Type	local
		At	chest

		Part	:BigRedCoreExplosion.part
		Part	:SMALLRedExplosionStreaks.part
		#Part	:BigDarkExplosion.part
		Part	:SMALLDarkHitspark.part
		Part	:SMALLDarkHitspark2.part
		Sound BlackGrExplo 100.0 100.0 1.0
	End

	
End

End	


			