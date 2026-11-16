#############################################################
## CrystalArmor.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	0.2

	Event
		#EName	3
		Type	Local
		At	LarmL
		Part	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeDots.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.2

	Event
		#EName	3
		Type	Local
		At	LarmR
		Part	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeDots.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.2

	Event
		#EName	3
		Type	Local
		At	UarmL
		Part	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeDots.part
		Lifespan 25
	End
End


Condition
	On 	cycle
	Time 	22
	Chance	0.2

	Event
		#EName	3
		Type	Local
		At	UarmR
		Part	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeDots.part
		Lifespan 25
	End

End

Condition
	On 	cycle
	Time 	27
	Chance	0.2

	Event
		#EName	3
		Type	Local
		At	LlegL
		Part	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeDots.part
		Lifespan 25
	End
End

Condition
	On 	cycle
	Time 	28
	Chance	0.2

	Event
		#EName	3
		Type	Local
		At	LlegR
		Part	CustomizeablePowers\StoneArmor\FortitudeRaysSmall.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeRing.part
		Part	CustomizeablePowers\StoneArmor\FortitudeDots.part
		Lifespan 25
	End
End

#########################################################################
##########################################################
##Stones Rolling
##########################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At 	origin
		Sound Stone4 100 100 .88
	End
End

Condition
	On	Time
	Time 	20

	Event
		Type	Local
		At 	origin
		Sound Glow5_loop 100 100 .3
	End
End

Condition
	On	Time
	Time 	18

	Event
		Type	Local
		At 	origin
		Sound stone_loop 100 100 .6
	End
End


Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		At	LLEGL
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
		#Part	:StoneArmorDirt02.part
	End

	Event
		Type	Local
		At	LLEGL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalLLEGL
	End

	Event
		Type	Local
		At	LLEGR
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
	End

	Event
		Type	Local
		At	LLEGR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalLLEGR
	End
End
###################################################################


Condition
	On 	Time
	Time 	5


	Event
		Type	Local
		At	ULEGR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalUlegR
	End

	Event
		Type	Local
		At	ULEGL
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalUlegL
	End
End
##########################################################

Condition
	On 	Time
	Time 	8


	Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalHIPS
	End

End
#################################################################


Condition
	On 	Time
	Time 	11


	Event
		Type	Local
		At	chest
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalCHEST
	End
End
#################################################################


Condition
	On 	Time
	Time 	13


	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalLARMR

	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalLARML

	End
End
#################################################################


Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	UARML
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
	End

	Event
		Type	Local
		At	UARML
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalUARML
	End

	Event
		Type	Local
		At	UARMR
		Part	CustomizeablePowers\StoneArmor\StoneArmorRocks.part
		Part	CustomizeablePowers\StoneArmor\StoneArmorDirt01.part
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\StoneArmor\StoneArmor.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_CrystalUARMR
	End
End
###############################################################################

Condition
	On 	death

	Event
		Ename	FinalRocks
		Type	start
		At	chest
		Part	CustomizeablePowers\StoneArmor\RocksScatter2.part
		Part	CustomizeablePowers\StoneArmor\RocksScatter.part
		Part	CustomizeablePowers\StoneArmor\CrystalSparkles.part
		Sound graniteexplo 70 70 .66
		Lifespan 19
	End


	Event
		Ename	FinalDust
		Type	start
		At	chest
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion.part
		Part	CustomizeablePowers\StoneArmor\StoneArmordirtExplosion1.part
		Lifespan 7
	End
End
End