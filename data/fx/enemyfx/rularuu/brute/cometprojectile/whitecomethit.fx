#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	20

Condition
	On	Time
			
	Event
		Ename	FireBallHit
		Type	local
		At	chest

		Part	:BigWhiteCoreExplosionLight.part
		Part	:WhiteExplosionStreaks.part
		Part	:BigDarkExplosion.part
		Part	:DarkHitspark.part
		Part	:DarkHitspark2.part
	End

	
	Event
		Ename	FireBallHit
		Type	local
		At	Root
		Bhvr	Behaviors/OffsetHips.bhvr
		Part	:BigWhiteExplosionRing.part
		Part	:BigWhiteExplosionRing2.part

	End

End

End	


			