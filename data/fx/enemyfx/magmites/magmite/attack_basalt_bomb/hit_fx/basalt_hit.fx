#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On	Time
	Time	0
			
	Event
		Ename	FireBallHit
		Type	local
		At	Chest
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Explosion.part
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Explosion_02.part
		#Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Lava_01.part
		#Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Rocks_01.part
		#Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Rocks_02.part
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Rocks_03.part
		Sound basalthit 66 66 .78
		Lifespan	30
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			