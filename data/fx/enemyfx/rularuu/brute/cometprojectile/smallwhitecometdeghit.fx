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
		Part	:SMALLDegredationRings.part
		#Part	:SMALLDarkHitspark.part
		#Part	:SMALLDarkHitspark2.part
		Sound Rularuuhit 70 70 .7
	End

	
End

End	


			