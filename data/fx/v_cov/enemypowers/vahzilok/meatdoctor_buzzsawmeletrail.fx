#########################################################
##	
##		Meat Doctor Spinning Saw
##
#########################################################################################

FxInfo
	
LifeSpan	100

###############################################################################
#################################  s a w   ####################################
###############################################################################

Condition
	On	Time
	Time	12

	Event	
		EName	SawSound
		Type	Local
		At	C_ToeR

		Sound motor2_loop 80.0 80.0 .8
		LifeSpan	45
	End
End


Condition
	On	Time
	Time	15

	Event
		EName	BuzzSaw
		Type	Local
		At	C_ToeR

		bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_MeleSawTrailOri.bhvr
		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_MeleSawTrail.part
#		Geom	GEO_Saw
		LifeSpan 34
	End

	Event
		EName	BuzzSaw01
		Type	Local
		At	C_ToeR

		bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_MeleSawTrailOri01.bhvr
		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_MeleSawTrail.part
#		Geom	GEO_Saw
		LifeSpan 34
	End

End

Condition
	On	Time
	Time	21

	Event
		EName	BuzzSawGeoTrail
		TYPE	Local
		At	C_ToeR

#		Childfx	V_COV\EnemyPowers\Vahzilok\MeatDoctor_BuzzSawTrailChild.fx
		LifeSpan	32
	End
End

End