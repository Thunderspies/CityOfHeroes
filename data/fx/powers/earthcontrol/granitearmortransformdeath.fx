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
		Part	POWERS\EarthControl\RocksScatter2.part
		Part	POWERS\EarthControl\RocksScatter.part
		Part	POWERS\EarthControl\RocksScatter.part
		Lifespan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Lifespan 7
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Lifespan 7
	End
End

#############################################################

Condition
	On 	DEATH

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part	POWERS\EarthControl\RocksScatter2.part
		Part	POWERS\EarthControl\RocksScatter.part
		Part	POWERS\EarthControl\RocksScatter.part
		Lifespan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Part	POWERS\EarthControl\StoneArmordirtExplosionNew.part
		Part	POWERS\EarthControl\StoneArmordirtExplosion1new.part
		Lifespan 7
	End

	Event
		Type	Destroy
		EName	RockSuit
	End
End

#############################################################

End