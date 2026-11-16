#############################################################
## StoneArmor.fx
#############################################################

FxInfo

Flags    InheritAlpha InheritGeoScale

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone6 100 100 0.95
	End
End

Condition
	On	Time
	Time 	5

	Event
		Type	Local
		At 	origin
		Sound Stone_loop 100 100 0.3
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	FootL
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_FootL02

	End

	Event
		Type	Local
		At	FootR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_FootR02
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_LLEGL02
		Part	:StoneArmorRocks03.part
		Part	:StoneArmorDirt01.part
		Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_LLEGR02
		Part	:StoneArmorRocks03.part
		Part	:StoneArmorDirt01.part
		Part	:StoneArmorDirt02.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	UlegR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_ULEGR02
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_ULEGL02
	End
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_LARMR02
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_LARML02
	End
End

#############################################################


Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	CHEST
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_CHEST02
	End

	Event
		Type	Local
		At	UARML
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_UARML02
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local
		At	UARMR
		Bhvr 	behaviors\StoneArmor.bhvr
		geom	StoneArmor_UARMR02
		Part	:StoneArmorRocks.part
		Part	:StoneArmorDirt01.part
		Part	:StoneArmorDirt02.part
	End
End

#############################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part	:RocksScatter2.part
		Part	:RocksScatter.part
		Sound graniteexplo 70 70 0.66
		Lifespan 19
	End

	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	:StoneArmordirtExplosion.part
		Part	:StoneArmordirtExplosion1.part
		Lifespan 7
	End
End

#############################################################

End