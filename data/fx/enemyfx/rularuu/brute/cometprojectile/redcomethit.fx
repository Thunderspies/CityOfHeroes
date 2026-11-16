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

		Part	:BigRedCoreExplosion.part
		Part	:RedExplosionStreaks.part
		Part	:BigRedExplosion.part
		Part	:DarkHitspark.part
		Part	:DarkHitspark2.part
	End

	Event
		Ename	FireBallHit
		Type	local
		At	Root
		Bhvr	Behaviors/OffsetHips.bhvr
		Part	:BigRedExplosionRing.part
		Part	:BigRedExplosionRing2.part

	End

End

End	


			