#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		#Part	:Hands_Lava_01.part
		Part	:Hands_Rocks_01.part
		Part	:Hands_Rocks_02.part
		Sound lavablast2 80 80 .8
		LifeSpan 45
	End
End

Condition
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		#Part	:Hands_Lava_01.part
		Part	:Hands_Rocks_01.part
		Part	:Hands_Rocks_02.part
		LifeSpan 45
	End
End

Condition
	On 	Time
	Time 	10
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:Hands_Flash_01.part
		LifeSpan 20
	End
	Event
		EName	SwordFlash2
		Type	Local
		At	WepL
		Part1	:Hands_Flash_01.part
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part	:Lava_Head.part
		Part	:Lava_Head_Glow.part
		Part	:Lava_Tail.part
		Part	:Lava_Drops_01.part
		Part	:Lava_Drops_02.part
		Part	:SparkTrail.part
	End
End

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	target
		#Part	:Lava_Hit_Rocks_03.part
		Lifespan	30
	End

	Event
		Ename	FireBallHit
		Type	local
		At	target
		#Part	:Lava_Hit_Glow_01.part
		#Part	:Lava_Hit_Explosion.part
		Part	:Lava_Hit_Explosion_03.part
		Lifespan	60
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			