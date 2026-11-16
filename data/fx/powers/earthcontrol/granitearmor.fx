#############################################################
## GraniteArmor.fx
#############################################################

FxInfo

Flags IsArmor

#############################################################

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	origin
		Sound stone_loop 60 60 0.26
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	FootL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_FootL
		Sound granite3 90 90 0.93
		LifeSpan 15

	End

	Event
		Type	Local
		At	FootR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_FootR
		LifeSpan 15

	End

End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LLEGL
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01a.part
		Part	:StoneArmorDirt02a.part
		LifeSpan 15
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LLEGR
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01a.part
		Part	:StoneArmorDirt02a.part
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_ULEGR
		LifeSpan 15
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_ULEGL
		LifeSpan 15
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LARMR
		LifeSpan 7
	End

		Event
		Type	Local
		At	LARML
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_LARML
		LifeSpan 7
	End
End

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_CHEST
		LifeSpan 7
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_UARML
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01a.part
		Part	:StoneArmorDirt02a.part
		LifeSpan 3
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_UARMR
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01a.part
		Part	:StoneArmorDirt02a.part
		LifeSpan 3
	End
End

#############################################################

Condition
	On 	Time
	Time 	12

	Event
		ENAME	Heada
		Type	Local
		At	Head
		Bhvr 	behaviors/StoneArmor.bhvr
		geom	FX_StoneArmor_HEAD
		LifeSpan 4

	End

End

#############################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		Part2	:RocksScatter.part
		Sound Graniteexplo 80 80 0.88
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

	On	Time
	Time	10

	Event
		Ename	FinalRocks
		Type	start
		At	head
		Part1	:RocksScatter2.part
		Part2	:RocksScatter.part
		Part2	:RocksScatter.part
		LifeSpan	19

	End



	Event
		Ename	FinalDust
		Type	start
		At	head
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
	Time	15

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

#############################################################

End