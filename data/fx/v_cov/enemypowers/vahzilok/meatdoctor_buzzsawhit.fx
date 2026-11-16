#########################################################
##	
##		Meat Doctor Spinning Saw
##
#########################################################################################

FxInfo
	
LifeSpan	30

###############################################################################
#################################  s a w   ####################################
###############################################################################

Condition
	On	Time
	Time	0

	Event
		EName	BuzzSawGeoTrail
		TYPE	Local
		At	Chest #T_C_ToeR

		ChildFX	V_COV\EnemyPowers\Vahzilok\MeatDoctor_BuzzSawTrailChild.fx
		Lifespan 30
	End
End

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		
		bhvr	V_COV\EnemyPowers\Vahzilok\MeatDoctor_B_MeleSawHit.bhvr
		Part1	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_EmbersSpray.part
		Part2	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_EmbersSpray01.part
		Part3	Powers/Claws/Clawflash.part	
		Part4	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_EmbersBroad.part
		Part5	V_COV\EnemyPowers\Vahzilok\MeatDoctor_P_MeleHitStar.part
		Lifespan 30
	End

End

End