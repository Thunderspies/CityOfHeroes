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

		Part	:BigWhiteCoreExplosion.part
		Part	:SMALLWhiteExplosionStreaks.part
		#Part	:BigDarkExplosion.part
		Part	:SMALLDarkHitspark.part
		Part	:SMALLDarkHitspark2.part
		Sound energysalvo 100.0 100.0 1.0
	End

	
End

End	


			