#############################################################
## GraniteArmorTransformDeath.fx
#############################################################

FxInfo

Flags IsArmor

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part	:RocksScatter2.part
		Part	:RocksScatter.part
		Part	:RocksScatter.part
		Lifespan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	:StoneArmordirtExplosionNew.part
		Part	:StoneArmordirtExplosion1new.part
		Part	:StoneArmordirtExplosionNew.part
		Part	:StoneArmordirtExplosion1new.part
		Part	:StoneArmordirtExplosionNew.part
		Part	:StoneArmordirtExplosion1new.part
		Lifespan 7
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part3	:StoneArmordirtExplosionNew.part
		Part1	:StoneArmordirtExplosion1new.part
		Part3	:StoneArmordirtExplosionNew.part
		Part1	:StoneArmordirtExplosion1new.part
		Part3	:StoneArmordirtExplosionNew.part
		Part1	:StoneArmordirtExplosion1new.part
		LifeSpan 7
	End
End

#############################################################

Condition
	On 	DEATH

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		Part2	:RocksScatter.part
		LifeSpan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part3	:StoneArmordirtExplosionNew.part
		Part1	:StoneArmordirtExplosion1new.part
		Part3	:StoneArmordirtExplosionNew.part
		Part1	:StoneArmordirtExplosion1new.part
		Part3	:StoneArmordirtExplosionNew.part
		Part1	:StoneArmordirtExplosion1new.part
		LifeSpan 7
	End

	Event
		Type	Destroy
		EName	RockSuit
	End
End

#############################################################

End