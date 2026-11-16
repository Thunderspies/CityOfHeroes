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

		Part	:BigBlueCoreExplosion.part
		Part	:BlueExplosionStreaks.part
		Part	:BigBlueExplosion.part
		Part	:DarkHitspark.part
		Part	:DarkHitspark2.part

		
	End

	Event
		Ename	FireBallHit
		Type	local
		At	Root
		Bhvr	Behaviors/OffsetHips.bhvr
		Part	:BigBlueExplosionRing.part
		Part	:BigBlueExplosionRing2.part

	End
	
End

End	


			