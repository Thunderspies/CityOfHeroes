#########################################################
##	
##
FxInfo

Flags	IsArmor

LifeSpan	30
#################################################################################

Condition

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		Part2	:RocksScatter.part
		LifeSpan	19
		
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
		LifeSpan	7
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
		LifeSpan	7
	End

End

End	