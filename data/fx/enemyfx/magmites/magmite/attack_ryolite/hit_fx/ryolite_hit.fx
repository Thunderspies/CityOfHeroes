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
		At	Root
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Explosion_04.part
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Explosion_03.part
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Rocks_01.part
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Rocks_05.part
		Part	ENEMYFX\Magmites\Magmite\Attack_Lava_Blast\Lava_Hit_Rocks_04.part
		Bhvr	behaviors\CameraShake02.bhvr
		Sound basalthit 66 66 .78
		Lifespan	30
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			