#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	20

Condition
	On	Time
	Time	3		
	Event
		Ename	FireBallHit
		Type	local
		At	chest

		Part	:BigBlueCoreExplosion.part
		Part	:SMALLBlueExplosionStreaks.part
		
	End

End

Condition
	On	Time
	Time	0

	Event
		Ename	FireBallHit
		Type	local
		At	Root
		
		Part	:HitRingUp.part
		LifeSpan	10
	End

End

End	


			