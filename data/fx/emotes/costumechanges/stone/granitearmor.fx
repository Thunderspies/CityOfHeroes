#########################################################
##
##
FxInfo
Lifespan 60
#Flags	IsArmor

#################################################################################


condition
	On	Time
	Time	12

	Event
		Type	Local
		At	origin
	#	Sound	stone_loop 60 40 .26
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	FootL
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.5 1.5 1.5
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_FootL
	#	Sound	granite3 90 70 .93
		LifeSpan	60

	End

	Event
		Type	Local
		At	FootR
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.5 1.5 1.5
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_FootR
		LifeSpan	60

	End

End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	2.0 2.0 2.0
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_LLEGL
	#	Part	:StoneArmorRocks.part
	#	Part	:StoneArmorDirt01a.part
	#	Part	:StoneArmorDirt02a.part
		LifeSpan	60

	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	2.0 2.0 2.0
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_LLEGR
	#	Part	:StoneArmorRocks.part
	#	Part	:StoneArmorDirt01a.part
	#	Part	:StoneArmorDirt02a.part
		LifeSpan	60
	End

End

###################################################################


Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.8 1.8 1.8
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_ULEGR
		LifeSpan	60

	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.8 1.8 1.8
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_ULEGL
		LifeSpan	60
	End

#	Event
#		Type	Local
#		At	Hips
#		Bhvr 	behaviors/StoneArmor.bhvr
#		BhvrOverride
#			EndScale	1.6 1.6 1.6
#		#	PyrRotate	0 0 90
#		End
#		geom	FX_StoneArmor_UARML
#		LifeSpan	60
#	End

End

#################################################################


Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.3 1.3 1.3
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_LARMR
		LifeSpan	60


	End

		Event
		Type	Local
		At	LARML
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.3 1.3 1.3
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_LARML
		LifeSpan	60


	End

End

###############################################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.4 1.4 1.4
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_CHEST
		LifeSpan	60

	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.25 1.25 1.25
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_UARML
		LifeSpan	60
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.4 1.4 1.4
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_UARMR
	#	Part	:StoneArmorRocks.part
	#	Part	:StoneArmorDirt01a.part
	#	Part	:StoneArmorDirt02a.part
		LifeSpan	60
	End
End


##########################################################



Condition
	On 	Time
	Time 	12

	Event
		ENAME	HeadEvent
		Type	Local
		At	Head
		Bhvr 	behaviors/StoneArmor.bhvr
		BhvrOverride
			EndScale	1.25 1.25 1.25
		End
		Part	:RocksScatter.part
		Part	:DustExplosion.part
		Part	:DirtExplosion.part
		geom	FX_StoneArmor_HEAD
		LifeSpan	60

	End

End

Condition
	On	Time
	Time	30

	Event
		ENAME	Explosion
		Type	Start
		At	Hips
		Part	Emotes/CostumeChanges/BlastFlash.part
		Part	:LargeBlast.part
		Part	:BlastRing.part
		Part	:Sparks.part
	End
End
#################################################################################

###############################################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	chest
	#	Part1	:RocksScatter2.part
	#	Part2	:RocksScatter.part
	#	Part2	:RocksScatter.part
		Sound Graniteexplo 80 80 .88
	#	LifeSpan	19

	End



	Event
		Ename	FinalDust
		Type	start
		At	chest
	#	Part3	:StoneArmordirtExplosionNew.part
	#	Part1	:StoneArmordirtExplosion1new.part
	#	Part3	:StoneArmordirtExplosionNew.part
	#	Part1	:StoneArmordirtExplosion1new.part
	#	Part3	:StoneArmordirtExplosionNew.part
	#	Part1	:StoneArmordirtExplosion1new.part
	#	LifeSpan	7
	End

End

Condition

	On	Time
	Time	10

	Event
		Ename	FinalRocks
		Type	start
		At	head
	#	Part1	POWERS/EarthControl/RocksScatter2.part
	#	Part2	POWERS/EarthControl/RocksScatter.part
	#	Part2	POWERS/EarthControl/RocksScatter.part
	#	LifeSpan	19

	End



	Event
		Ename	FinalDust
		Type	start
		At	head
#		Part3	:StoneArmordirtExplosionNew.part
#		Part1	:StoneArmordirtExplosion1new.part
#		Part3	:StoneArmordirtExplosionNew.part
#		Part1	:StoneArmordirtExplosion1new.part
#		Part3	:StoneArmordirtExplosionNew.part
#		Part1	:StoneArmordirtExplosion1new.part
	#	LifeSpan	7
	End

End

Condition

	On	Time
	Time	15

	Event
		Ename	FinalDust
		Type	start
		At	chest
#		Part3	:StoneArmordirtExplosionNew.part
#		Part1	:StoneArmordirtExplosion1new.part
#		Part3	:StoneArmordirtExplosionNew.part
#		Part1	:StoneArmordirtExplosion1new.part
#		Part3	:StoneArmordirtExplosionNew.part
#		Part1	:StoneArmordirtExplosion1new.part
	#	LifeSpan	7
	End

End


End